#include "llvm.h"

#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Config/config.h>
#include <clang/Driver/DriverDiagnostic.h>
#include <clang/Driver/Options.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DiagnosticInfo.h>
#include <llvm/IR/DiagnosticPrinter.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/Internalize.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <elf.h>
#include <err.h>
#include <sys/mman.h>
#include <fstream>
#include <gelf.h>
#include <iostream>
#include <libelf.h>
#include <memory>
#include <sstream>


#include "debug.h"
#include "mhandle.h"

namespace {

struct InvalidBuildOptions {};
struct FailedToEmitBitcode {};
struct BuildError {};
struct TargetLookupFailure {
    TargetLookupFailure(std::string Log) : Log{std::move(Log)} {}

    std::string Log;
};
struct CouldNotCreateTargetMachine {};
struct TargetCannotEmitFile {};
struct FailedToParseBitcode {
    FailedToParseBitcode(std::string File, std::string Message)
        : File{std::move(File)}, Message{std::move(Message)} {}

    std::string File;
    std::string Message;
};
struct FailedToLinkBitcode {
    FailedToLinkBitcode(std::string File) : File{std::move(File)} {}

    std::string File;
};

enum {
    CAL_GPU_BASE = 1001,
    CAL_CPU_BASE = 2001,
    EM_ATI_CALIMAGE_BINARY = 125,
    ELFOSABI_CALIMAGE = 100,
};

enum ElfNote {
    ELF_NOTE_ATI_PROGRAM_INFO = 1,
};

enum CALtargetEnum {
    CAL_TARGET_600 = 0,         /**< R600 GPU ISA */
    CAL_TARGET_610 = 1,         /**< RV610 GPU ISA */
    CAL_TARGET_630 = 2,         /**< RV630 GPU ISA */
    CAL_TARGET_670 = 3,         /**< RV670 GPU ISA */
    CAL_TARGET_7XX = 4,         /**< R700 class GPU ISA */
    CAL_TARGET_770 = 5,         /**< RV770 GPU ISA */
    CAL_TARGET_710 = 6,         /**< RV710 GPU ISA */
    CAL_TARGET_730 = 7,         /**< RV730 GPU ISA */
    CAL_TARGET_CYPRESS = 8,     /**< CYPRESS GPU ISA */
    CAL_TARGET_JUNIPER = 9,     /**< JUNIPER GPU ISA */
    CAL_TARGET_REDWOOD = 10,    /**< REDWOOD GPU ISA */
    CAL_TARGET_CEDAR = 11,      /**< CEDAR GPU ISA */
    CAL_TARGET_SUMO = 12,       /**< SUMO GPU ISA */
    CAL_TARGET_SUPERSUMO = 13,  /**< SUPERSUMO GPU ISA */
    CAL_TARGET_WRESTLER = 14,   /**< WRESTLER GPU ISA */
    CAL_TARGET_CAYMAN = 15,     /**< CAYMAN GPU ISA */
    CAL_TARGET_KAUAI = 16,      /**< KAUAI GPU ISA */
    CAL_TARGET_BARTS = 17,      /**< BARTS GPU ISA */
    CAL_TARGET_TURKS = 18,      /**< TURKS GPU ISA */
    CAL_TARGET_CAICOS = 19,     /**< CAICOS GPU ISA */
    CAL_TARGET_TAHITI = 20,     /**< TAHITI GPU ISA*/
    CAL_TARGET_PITCAIRN = 21,   /**< PITCAIRN GPU ISA*/
    CAL_TARGET_CAPEVERDE = 22,  /**< CAPE VERDE GPU ISA*/
    CAL_TARGET_DEVASTATOR = 23, /**< DEVASTATOR GPU ISA*/
    CAL_TARGET_SCRAPPER = 24,   /**< SCRAPPER GPU ISA*/
    CAL_TARGET_OLAND = 25,      /**< OLAND GPU ISA*/
    CAL_TARGET_BONAIRE = 26,    /**< BONAIRE GPU ISA*/
    CAL_TARGET_KALINDI = 29,    /**< KALINDI GPU ISA*/
};

struct CALEncodingDictionaryEntry {
    Elf32_Word d_machine;
    Elf32_Word d_type;
    Elf32_Off d_offset;
    Elf32_Word d_size;
    Elf32_Word d_flags;
};
struct CALNoteHeader {
    Elf32_Word namesz = 8;
    Elf32_Word descsz;
    Elf32_Word type;
    char name[8] = "ATI CAL";
};
struct CALProgramInfoEntry {
    Elf32_Word address;
    Elf32_Word value;
};
struct BinaryComputePgmRsrc2 {
    unsigned int scratch_en     : 1;
    unsigned int user_sgpr      : 5;
    unsigned int trap_present   : 1;
    unsigned int tgid_x_en      : 1;
    unsigned int tgid_y_en      : 1;
    unsigned int tgid_z_en      : 1;
    unsigned int tg_size_en     : 1;
    unsigned int tidig_comp_cnt : 2;
    unsigned int excp_en_msb    : 2;
    unsigned int lds_size       : 9;
    unsigned int excp_en        : 7;
    unsigned int                : 1;
};
static_assert(sizeof(BinaryComputePgmRsrc2) == sizeof(uint32_t), "pgm rsrc2 should be 4 bytes");


unsigned OptLevel = 0;

const std::string MIAOW_CPU{"tahiti"};
const std::string MIAOW_TARGET{"amdgcn--"};
const std::string MIAOW_TRIPLE{"tahiti-amdgcn--"};
const std::string OPENCL_VERSION_DEFINE{"__OPENCL_VERSION_=110"};

void
InitializeTargets()
{
    static bool TargetsInitialized = false;
    if (TargetsInitialized)
        return;
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

#ifdef LINK_POLLY_INTO_TOOLS
    llvm::PassRegistry &Registry = *llvm::PassRegistry::getPassRegistry();
    polly::initializePollyPasses(Registry);
#endif

    TargetsInitialized = true;
}

void
ClangDiagnosticHandler(llvm::DiagnosticInfo const &DI, void *Context)
{
    if (DI.getSeverity() == llvm::DS_Error) {
        std::string *Data = reinterpret_cast<std::string *>(Context);
        llvm::raw_string_ostream OS { *Data };
        llvm::DiagnosticPrinterRawOStream Printer {OS};
        DI.print(Printer);
        llvm::errs() << *Data << "\n";
        throw BuildError();
    }
}

std::unique_ptr<clang::CompilerInstance>
CreateCompiler(llvm::ArrayRef<const char *> Options, std::string &BuildLog)
{
    auto Compiler = std::make_unique<clang::CompilerInstance>();
    auto DiagsBuffer = new clang::TextDiagnosticBuffer;
    auto DiagID = new clang::DiagnosticIDs;
    auto DiagOpts = new clang::DiagnosticOptions;
    clang::DiagnosticsEngine Diags{DiagID, DiagOpts, DiagsBuffer};

    bool Success = clang::CompilerInvocation::CreateFromArgs(
            Compiler->getInvocation(), Options.data(),
            Options.data() + Options.size(), Diags);
    if (!Success)
        throw InvalidBuildOptions{};

    DiagsBuffer->FlushDiagnostics(Diags);
    if (Diags.hasErrorOccurred())
       throw InvalidBuildOptions{};

    Compiler->getTargetOpts().CPU = MIAOW_CPU;
    Compiler->getTargetOpts().Triple = MIAOW_TARGET;
    Compiler->getLangOpts().NoBuiltin = true;

    Compiler->getInvocation().setLangDefaults(
            Compiler->getLangOpts(), clang::InputKind::OpenCL,
            llvm::Triple(MIAOW_TARGET), Compiler->getPreprocessorOpts(),
            clang::LangStandard::lang_opencl11);
    Compiler->createDiagnostics(new clang::TextDiagnosticPrinter(
             *new llvm::raw_string_ostream(BuildLog),
             &Compiler->getDiagnosticOpts(), /*OwnsOutputStream=*/true));
    Compiler->setTarget(clang::TargetInfo::CreateTargetInfo(
                Compiler->getDiagnostics(), Compiler->getInvocation().TargetOpts));
    return Compiler;
}

std::unique_ptr<llvm::Module>
Compile(llvm::LLVMContext &Ctx, clang::CompilerInstance &CC, llvm::StringRef
        FileName, llvm::StringRef Source, std::string &BuildLog)
{
    CC.getFrontendOpts().ProgramAction = clang::frontend::EmitLLVMOnly;
    CC.getHeaderSearchOpts().UseBuiltinIncludes = true;
    CC.getHeaderSearchOpts().UseStandardSystemIncludes = true;
    CC.getHeaderSearchOpts().ResourceDir = CLANG_RESOURCE_DIR;

    CC.getHeaderSearchOpts().AddPath(LIBCLC_INCLUDEDIR, clang::frontend::Angled,
            /*IsFramework=*/false,
            /*IgnoreSysRoot=*/false);
    CC.getPreprocessorOpts().Includes.push_back("clc/clc.h");

    CC.getPreprocessorOpts().addMacroDef(OPENCL_VERSION_DEFINE);
    CC.getPreprocessorOpts().addMacroDef("cl_clang_storage_class_specifiers");
    CC.getPreprocessorOpts().addRemappedFile(FileName,
            llvm::MemoryBuffer::getMemBuffer(Source).release());

    // Link in libclc before optimization
    clang::CodeGenOptions::BitcodeFileToLink LibCLC;
    LibCLC.Filename = std::string{LIBCLC_LIBEXECDIR} + "/" + MIAOW_TRIPLE + ".bc";
    LibCLC.PropagateAttrs = true;
    LibCLC.LinkFlags = llvm::Linker::Flags::None;
    CC.getCodeGenOpts().LinkBitcodeFiles.emplace_back(std::move(LibCLC));

    clang::EmitLLVMOnlyAction Action(&Ctx);
    if (!CC.ExecuteAction(Action)) {
        llvm::errs() << BuildLog;
        throw FailedToEmitBitcode{};
    }

    return Action.takeModule();
}

std::vector<const llvm::Function *>
GetKernelList(const llvm::Module &M)
{
    std::vector<const llvm::Function *> Kernels;
    for (const auto &F : M) {
        if (F.getMetadata("kernel_arg_type"))
            Kernels.emplace_back(&F);
    }
    return Kernels;
}

std::string
GetKernelMetadata(const llvm::Function &F)
{
    std::string Metadata;
    llvm::raw_string_ostream OS{Metadata};
    OS << ";ARGSTART:__OpenCL_" << F.getName() << "_kernel\n";
    OS << ";version:3:1:104\n";
    OS << ";device:tahiti\n";

    // TODO(rzl): there are missing tags, such as (non-exhaustive?):
    // - uniqueid
    // - memory
    // - function
    // - privateid
    // - reflection

    // XXX(rzl): this seems to increase by 16 per argument. What and why?
    size_t Off = 0;
    for (const auto &Arg : F.args()) {
        auto Type = Arg.getType();
        // TODO(rzl): pointer type data is not completely specified, being treated
        // as if it were a value.
        if (Arg.getType()->isPointerTy()) {
            OS << ";pointer";
            Type = Type->getPointerElementType();
        } else {
            OS << ";value";
        }
        OS << ":" << Arg.getName();
        if (Type->isIntegerTy(32) || Type->isIntOrIntVectorTy(32))
            OS << ":u32";
        else if (Type->isIntegerTy(16) || Type->isIntOrIntVectorTy(16))
            OS << ":u16";
        else if (Type->isIntegerTy(8) || Type->isIntOrIntVectorTy(8))
            OS << ":u8";
        else if (Type->isFloatTy() || Type->getScalarType()->isFloatTy())
            OS << ":float";
        else {
            //Type->dump();
            llvm_unreachable("unhandled type");
        }
        // XXX(rzl): these seem to be always the same
        OS << ":1:1:" << Off;
        if (const auto *PT = llvm::dyn_cast<llvm::PointerType>(Arg.getType())) {
            // Memory scope
            if (PT->getAddressSpace() == 3 /* AMDGPUAS::LOCAL_MEMORY */) {
                OS << ":hl";
            } else if (PT->getAddressSpace() == 1 /* AMDGPUAS::GLOBAL_ADDRESS */) {
                OS << ":uav";
            } else {
                //PT->dump();
                llvm_unreachable("unhandled address space");
            }
            // XXX(rzl): Buffer number. Does this need to differ per pointer?
            OS << ":10";
            // Alignment
            OS << ":" << F.getParent()->getDataLayout().getABITypeAlignment(Type);
            // TODO(rzl): Access type. What about const?
            OS << ":RW";
            OS << ":0";
            OS << ":0";
        }
        OS << "\n";
        Off += 16;
    }
    OS << ";ARGEND:__OpenCL_" << F.getName() << "_kernel\n";
    return OS.str();
}

template <typename Func>
void
OptimizeModule(llvm::Module &Module,
        Func SkipInternalization = [](const auto &GV) { return true; })
{
    llvm::legacy::PassManager PM;
    auto Kernels = GetKernelList(Module);
    PM.add(llvm::createInternalizePass(SkipInternalization));

    llvm::PassManagerBuilder PMB;
    PMB.OptLevel = OptLevel;
    PMB.LibraryInfo =
        new llvm::TargetLibraryInfoImpl(llvm::Triple{Module.getTargetTriple()});
    PMB.populateModulePassManager(PM);
    PM.run(Module);
}

std::string
ReplaceExtension(llvm::StringRef Path, llvm::StringRef Extension)
{
    llvm::SmallVector<char, 32> Buffer{Path.begin(), Path.end()};
    llvm::sys::path::replace_extension(Buffer, Extension);
    return std::string{Buffer.begin(), Buffer.end()};
}

void
EmitCode(llvm::Module &Module, llvm::TargetMachine::CodeGenFileType FT,
        llvm::raw_pwrite_stream &OS)
{
    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(MIAOW_TARGET, Error);
    if (!Target)
        throw TargetLookupFailure(Error);

    std::unique_ptr<llvm::TargetMachine> TargetMachine{
        Target->createTargetMachine(MIAOW_TARGET, MIAOW_CPU,
                /*Features=*/"", /*Options=*/{}, llvm::None,
                llvm::None, llvm::CodeGenOpt::Default)};
    if (!TargetMachine)
        throw CouldNotCreateTargetMachine();

    llvm::legacy::PassManager PM;
    Module.setDataLayout(TargetMachine->createDataLayout());
    if (FT == llvm::TargetMachine::CGFT_AssemblyFile)
        TargetMachine->Options.MCOptions.AsmVerbose = true;

    if (TargetMachine->addPassesToEmitFile(PM, OS, nullptr, FT))
        throw TargetCannotEmitFile();

    PM.run(Module);
}

std::unique_ptr<Elf, int (*)(Elf *)>
GetElfFromMemory(const llvm::SmallVectorImpl<char> &Code)
{
    return {elf_memory(const_cast<char *>(Code.data()), Code.size()), elf_end};
}

struct SymbolInformation {
    std::string Name;
    decltype(GElf_Sym::st_value) Value;
    decltype(GElf_Sym::st_size) Size;
    decltype(GElf_Sym::st_shndx) SectionIndex;
};

Elf_Scn *
FindSectionByName(Elf *TheElf, llvm::StringRef Name)
{
    size_t SectionStringsIndex;
    elf_getshdrstrndx(TheElf, &SectionStringsIndex);

    Elf_Scn *Target = nullptr;
    for (Elf_Scn *Section = elf_nextscn(TheElf, nullptr); Section;
            Section = elf_nextscn(TheElf, Section)) {
        GElf_Shdr Header;
        if (gelf_getshdr(Section, &Header) != &Header) {
            std::cerr << "gelf_getshdr: " << elf_errmsg(-1) << std::endl;
            exit(1);
        }

        auto SectionName = elf_strptr(TheElf, SectionStringsIndex, Header.sh_name);
        if (Name == SectionName) {
            Target = Section;
            break;
        }
    }

    return Target;
}

std::vector<char>
GetTextSection(Elf *TheElf)
{
    auto Text = FindSectionByName(TheElf, ".text");
    if (!Text) {
        // TODO(rzl): proper error handling
        std::cerr << "ELF file has no .text section\n";
        exit(1);
    }

    GElf_Shdr Header;
    if (gelf_getshdr(Text, &Header) != &Header) {
        std::cerr << "gelf_getshdr: " << elf_errmsg(-1) << std::endl;
        exit(1);
    }
    Elf_Data *TextData = elf_getdata(Text, nullptr);
    return {static_cast<char *>(TextData->d_buf),
        static_cast<char *>(TextData->d_buf) + TextData->d_size};
}

std::unordered_map<std::string, SymbolInformation>
GetSymbols(Elf *TheElf)
{
    auto SymTab = FindSectionByName(TheElf, ".symtab");

    if (!SymTab)
        return {};

    std::unordered_map<std::string, SymbolInformation> Symbols;
    Elf_Data *SymTabData = elf_getdata(SymTab, nullptr);
    GElf_Shdr Header;
    if (gelf_getshdr(SymTab, &Header) != &Header)
        return {};

    GElf_Sym Symbol;
    unsigned Idx = 0;
    while (GElf_Sym *Sym = gelf_getsym(SymTabData, Idx++, &Symbol)) {
        std::string Name = elf_strptr(TheElf, Header.sh_link, Sym->st_name);
        auto It = Symbols.find(Name);
        assert(It == Symbols.end());

        SymbolInformation Info;
        Info.Name = Name;
        Info.Value = Sym->st_value;
        Info.Size = Sym->st_size;
        Info.SectionIndex = Sym->st_shndx;
        Symbols.insert(It, {Name, Info});
    }

    return Symbols;
}

std::vector<char>
PrepareOpenCLInnerELF(llvm::StringRef InputName, llvm::StringRef KernelName,
        Elf *GeneratedELF)
{
    int FD;
    FD = memfd_create("/tmp", 0);
    assert(FD != -1);

    std::array<CALEncodingDictionaryEntry, 1> DictEntries = {};
    std::vector<char> Notes;

    Elf *NewELF = elf_begin(FD, ELF_C_WRITE, nullptr);
    Elf32_Ehdr *Ehdr = elf32_newehdr(NewELF);
    Ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
    Ehdr->e_ident[EI_OSABI] = ELFOSABI_CALIMAGE;
    Ehdr->e_ident[EI_ABIVERSION] = 1;
    Ehdr->e_type = ET_EXEC;
    Ehdr->e_machine = EM_ATI_CALIMAGE_BINARY;
    Ehdr->e_version = EV_CURRENT;
    Ehdr->e_flags = EM_ATI_CALIMAGE_BINARY;

    {
        Elf32_Phdr *Phdr = elf32_newphdr(NewELF, 1 + 2 * DictEntries.size());
        // Encoding Dictionary
        Phdr[0].p_type = PT_LOPROC + 0x02;
        Phdr[0].p_filesz = sizeof(DictEntries);
        for (size_t Ix = 0; Ix < DictEntries.size(); ++Ix) {
            {
                std::vector<CALProgramInfoEntry> Entries;

                Elf_Scn *AMDGPUConfig = FindSectionByName(GeneratedELF, ".AMDGPU.config");
                Elf_Data *AMDGPUConfigData = elf_getdata(AMDGPUConfig, nullptr);
                assert(elf_getdata(AMDGPUConfig, AMDGPUConfigData) == nullptr);

                struct {
                    uint32_t const1;        
                    uint32_t ComputePgmRsrc1;        
                    uint32_t const2;        
                    struct BinaryComputePgmRsrc2 ComputePgmRsrc2;        
                } AMDGPUConfigValues;
                memcpy(&AMDGPUConfigValues, AMDGPUConfigData->d_buf, sizeof(AMDGPUConfigValues));
                assert(AMDGPUConfigValues.const1 == 47176);
                assert(AMDGPUConfigValues.const2 == 47180);

                CALProgramInfoEntry Entry;
                Entry.address = 0x00002e13;  // COMPUTE_PGM_RSRC2
                {
                    union {
                        unsigned int val;
                        struct BinaryComputePgmRsrc2 rsrc2;
                    } t = {0u};
                    t.rsrc2 = AMDGPUConfigValues.ComputePgmRsrc2;
                    Entry.value = t.val;
                }
                Entries.push_back(Entry);

                // User element count
                Entries.push_back({0x80001000u, 0u});
                // // User element 0
                // Entries.push_back({0x80001001u, 4u});  // class
                // Entries.push_back({0x80001002u, 0u});  // slot
                // Entries.push_back({0x80001003u, 0u});  // sgpr start
                // Entries.push_back({0x80001004u, 2u});  // sgpr count

                CALNoteHeader Header{};
                Header.type = ELF_NOTE_ATI_PROGRAM_INFO;
                Header.descsz = Entries.size() * sizeof(Entries[0]);
                Notes.insert(Notes.end(),
                        reinterpret_cast<char *>(&Header),
                        reinterpret_cast<char *>(&Header) + sizeof(Header));
                Notes.insert(Notes.end(),
                        reinterpret_cast<char *>(&*Entries.begin()),
                        reinterpret_cast<char *>(&*Entries.end()));
            }

            Phdr[1 + Ix * 2].p_type = PT_NOTE;
            Phdr[1 + Ix * 2].p_filesz = Notes.size();

            Phdr[2 + Ix * 2].p_type = PT_LOAD;
            Phdr[2 + Ix * 2].p_filesz = 0;
        }
    }

    Elf_Scn *ShStrtab = elf_newscn(NewELF);
    Elf32_Shdr *ShStrtabHdr = elf32_getshdr(ShStrtab);
    ShStrtabHdr->sh_name = 1;
    ShStrtabHdr->sh_type = SHT_STRTAB;
    char const ShStrtabStrings[] = {
        /*  0 */ '\0',
        /*  1 */ '.',  's', 'h', 's', 't', 'r',  't', 'a',  'b', '\0',
        /* 11 */ '.',  's', 't', 'r', 't', 'a',  'b', '\0',
        /* 19 */ '.',  's', 'y', 'm', 't', 'a',  'b', '\0',
        /* 27 */ '.',  'd', 'a', 't', 'a', '\0',
        /* 33 */ '.',  't', 'e', 'x', 't', '\0',
        /* 39 */ '.',  'e', 'n', 'c', 'd', 'i', 'c', 't', '\0',
        /* 48 */ '.',  'c', 'a', 'l', 'n', 'o', 't', 'e', '\0'};
    Elf_Data *ShStrtabData = elf_newdata(ShStrtab);
    ShStrtabData->d_align = 1;
    ShStrtabData->d_buf = (void *)ShStrtabStrings;
    ShStrtabData->d_size = sizeof(ShStrtabStrings);

    Elf_Scn *EncDict = elf_newscn(NewELF);
    Elf32_Shdr *EncDictHdr = elf32_getshdr(EncDict);
    EncDictHdr->sh_name = 39;
    EncDictHdr->sh_type = SHT_LOPROC+0x02;
    Elf_Data *EncDictData = elf_newdata(EncDict);
    EncDictData->d_align = 4;
    EncDictData->d_buf = &DictEntries;
    EncDictData->d_size = sizeof(DictEntries);

    Elf_Scn *CalNote0 = elf_newscn(NewELF);
    Elf32_Shdr *CalNote0Hdr = elf32_getshdr(CalNote0);
    CalNote0Hdr->sh_name = 48;
    CalNote0Hdr->sh_type = SHT_LOPROC+0x02;
    Elf_Data *CalNote0Data = elf_newdata(CalNote0);
    CalNote0Data->d_align = 4;
    CalNote0Data->d_buf = (void *)Notes.data();
    CalNote0Data->d_size = Notes.size();

    Elf_Scn *Text = elf_newscn(NewELF);
    Elf32_Shdr *TextHdr = elf32_getshdr(Text);
    TextHdr->sh_name = 33;
    TextHdr->sh_type = SHT_PROGBITS;
    auto GeneratedText = GetTextSection(GeneratedELF);
    Elf_Data *TextData = elf_newdata(Text);
    TextData->d_align = 4;
    TextData->d_buf = GeneratedText.data();
    TextData->d_size = GeneratedText.size() * sizeof(GeneratedText[0]);

    Elf_Scn *Data = elf_newscn(NewELF);
    Elf32_Shdr *DataHdr = elf32_getshdr(Data);
    DataHdr->sh_name = 27;
    DataHdr->sh_type = SHT_PROGBITS;
    Elf_Data *DataData = elf_newdata(Data);
    auto DataBytes = std::make_unique<std::array<char, 0x1280>>();
    DataBytes->fill(0);
    DataData->d_buf = DataBytes->data();
    // XXX(rzl): size must be 0x1280 bytes, I have no idea why.
    DataData->d_size = sizeof(*DataBytes);

    Elf_Scn *Symtab = elf_newscn(NewELF);
    Elf32_Shdr *SymtabHdr = elf32_getshdr(Symtab);
    SymtabHdr->sh_name = 19;
    SymtabHdr->sh_type = SHT_SYMTAB;
    SymtabHdr->sh_entsize = sizeof(Elf32_Sym);
    Elf32_Sym SymtabSyms[] = {{}};
    Elf_Data *SymtabData = elf_newdata(Symtab);
    SymtabData->d_align = sizeof(Elf32_Sym);
    SymtabData->d_type = ELF_T_SYM;
    SymtabData->d_buf = SymtabSyms;
    SymtabData->d_size = sizeof(SymtabSyms);

    Elf_Scn *Strtab = elf_newscn(NewELF);
    Elf32_Shdr *StrtabHdr = elf32_getshdr(Strtab);
    StrtabHdr->sh_name = 11;
    StrtabHdr->sh_type = SHT_STRTAB;
    char StrtabStrings[] = {/* 0 */ '\0'};
    Elf_Data *StrtabData = elf_newdata(Strtab);
    StrtabData->d_align = 1;
    StrtabData->d_buf = StrtabStrings;
    StrtabData->d_size = sizeof(StrtabStrings);

    // Link string tables
    Ehdr->e_shstrndx = elf_ndxscn(ShStrtab);
    SymtabHdr->sh_link = elf_ndxscn(Strtab);

    // Recalculate ELF layout so we have a good starting point and set manual
    // layout.
    if (elf_update(NewELF, ELF_C_NULL) < 0)
        ::errx(1, "elf_update(, ELF_C_NULL): %s", elf_errmsg(-1));

    elf_flagelf(NewELF, ELF_C_SET, ELF_F_LAYOUT);

    ShStrtab = elf_getscn(NewELF, 1);
    ShStrtabHdr = elf32_getshdr(ShStrtab);
    Text = elf_getscn(NewELF, 4);
    TextHdr = elf32_getshdr(Text);
    Strtab = elf_getscn(NewELF, 7);
    StrtabHdr = elf32_getshdr(Strtab);

    Elf32_Phdr *DictPhdr = &elf32_getphdr(NewELF)[0];
    Elf32_Phdr *Note0Phdr = &elf32_getphdr(NewELF)[1];
    Elf32_Phdr *Load0Phdr = &elf32_getphdr(NewELF)[2];

    Ehdr = elf32_getehdr(NewELF);
    size_t SectionCount;
    elf_getshdrnum(NewELF, &SectionCount);

    DictPhdr->p_offset = EncDictHdr->sh_offset;
    Note0Phdr->p_offset = CalNote0Hdr->sh_offset;
    Load0Phdr->p_offset = TextHdr->sh_offset;
    // The sections are laid out thus: .text .data .symtab .strtab
    // We don't care about gaps so just compute the size from the beginning of
    // .text to the end of .strtab
    Load0Phdr->p_filesz =
        StrtabHdr->sh_offset - TextHdr->sh_offset + StrtabHdr->sh_size;
    Load0Phdr->p_memsz = Load0Phdr->p_filesz;

    // XXX(rzl): where did this come from?
    DictEntries[0].d_machine = 9;
    DictEntries[0].d_offset = Note0Phdr->p_offset;
    DictEntries[0].d_size =
        StrtabHdr->sh_offset - Note0Phdr->p_offset + StrtabHdr->sh_size;

    if (elf_update(NewELF, ELF_C_WRITE) < 0)
        ::errx(1, "elf_update(, ELF_C_WRITE): %s", elf_errmsg(-1));

    // TODO(rzl): Don't forget to reacquire the pointers because they're
    // invalidated by elf_update(3).
    // TODO(rzl): Move all offsets forward by the size of our encoding
    // dictionary to make room for the encoding dictionary. For each encoding,
    // move forward all sections to make room for the notes. Probaby should be
    // done backwards to avoid quadratic behaviour.
    // TODO(rzl): Finally set the offset and size for all program headers.
    size_t FinalSize;
    char *RawELFBytes = elf_rawfile(NewELF, &FinalSize);
    if (!FinalSize)
        ::errx(1, "elf_rawfile: %s", elf_errmsg(-1));

    std::vector<char> RawELF{RawELFBytes, RawELFBytes + FinalSize};

    // Fill the Encoding Dictionary
    Elf32_Phdr *Phdr = &elf32_getphdr(NewELF)[0];
    memcpy(RawELF.data() + Phdr->p_offset,
            reinterpret_cast<const char *>(DictEntries.data()), sizeof(DictEntries));
    Phdr = &elf32_getphdr(NewELF)[1];
    memcpy(RawELF.data() + Phdr->p_offset,
            reinterpret_cast<const char *>(Notes.data()), Notes.size());

    elf_end(NewELF);

    // TODO(rzl): Fill in the encoding dictionary and notes.
    return RawELF;
}

std::pair<char *, size_t>
CreateOpenCLElf(const std::vector<std::string> &KernelNames, const
        std::vector<std::vector<char>> &InnerBinaries, const
        std::vector<std::string> &KernelMetadata)
{
    assert(KernelNames.size() == InnerBinaries.size());
    assert(KernelNames.size() == KernelMetadata.size());

    int FD;
    FD = memfd_create("/tmp", 0);
    assert(FD != -1);

    Elf *NewElf = elf_begin(FD, ELF_C_WRITE, nullptr);
    Elf32_Ehdr *EHdr = elf32_newehdr(NewElf);
    EHdr->e_ident[EI_DATA] = ELFDATA2LSB;
    EHdr->e_ident[EI_OSABI] = ELFOSABI_NONE;
    EHdr->e_ident[EI_ABIVERSION] = 0;
    EHdr->e_type = ET_NONE;
    EHdr->e_version = EV_CURRENT;
    EHdr->e_machine = 1001 + CAL_TARGET_TAHITI;

    Elf_Scn *ShStrTab = elf_newscn(NewElf);
    Elf_Scn *StrTab = elf_newscn(NewElf);
    Elf_Scn *SymTab = elf_newscn(NewElf);
    Elf_Scn *Rodata = elf_newscn(NewElf);
    Elf_Scn *Text = elf_newscn(NewElf);
    EHdr->e_shstrndx = elf_ndxscn(ShStrTab);

    // Process kernels and generate symbols and section data
    llvm::SmallVector<char, 64> SectionStrings = {'\0'};
    llvm::SmallVector<char, 64> SymbolStrings = {'\0'};
    llvm::SmallVector<Elf32_Sym, 4> Symbols = {{}};
    llvm::SmallVector<char, 4096> TextSectionData;
    llvm::SmallVector<char, 64> RodataSectionData;

    for (size_t Ix = 0; Ix < KernelNames.size(); ++Ix) {
        Elf32_Sym Sym = {};
        {
            Sym.st_name = SymbolStrings.size_in_bytes();
            const std::string SymbolName = "__OpenCL_" + KernelNames[Ix] + "_kernel";
            SymbolStrings.append(SymbolName.begin(), SymbolName.end());
            SymbolStrings.push_back('\0');
        }
        {
            Sym.st_value = TextSectionData.size_in_bytes();
            TextSectionData.insert(std::end(TextSectionData),
                    std::begin(InnerBinaries[Ix]),
                    std::end(InnerBinaries[Ix]));
            Sym.st_size = InnerBinaries[Ix].size();
        }
        Sym.st_info = STT_FUNC;
        Sym.st_other = STV_DEFAULT;
        Sym.st_shndx = elf_ndxscn(Text);
        Symbols.emplace_back(std::move(Sym));

        Sym = {};
        {
            Sym.st_name = SymbolStrings.size_in_bytes();
            const std::string SymbolName = "__OpenCL_" + KernelNames[Ix] + "_metadata";
            SymbolStrings.append(SymbolName.begin(), SymbolName.end());
            SymbolStrings.push_back('\0');
        }
        {
            Sym.st_value = RodataSectionData.size_in_bytes();
            RodataSectionData.insert(std::end(RodataSectionData),
                    std::begin(KernelMetadata[Ix]),
                    std::end(KernelMetadata[Ix]));
            Sym.st_size = KernelMetadata[Ix].size();
        }
        Sym.st_info = STT_OBJECT;
        Sym.st_other = STV_DEFAULT;
        Sym.st_shndx = elf_ndxscn(Rodata);
        Symbols.emplace_back(std::move(Sym));

        Sym = {};
        {
            Sym.st_name = SymbolStrings.size_in_bytes();
            const std::string SymbolName =
                "__OpenCL_" + KernelNames[Ix] + "_header";
            SymbolStrings.append(SymbolName.begin(), SymbolName.end());
            SymbolStrings.push_back('\0');
        }
        {
            Sym.st_value = RodataSectionData.size_in_bytes();
            Sym.st_size = 0;
        }
        Sym.st_info = STT_OBJECT;
        Sym.st_other = STV_DEFAULT;
        Sym.st_shndx = elf_ndxscn(Rodata);
        Symbols.emplace_back(std::move(Sym));
    }

    // .symtab
    Elf32_Shdr *SymTabHeader = elf32_getshdr(SymTab);
    SymTabHeader->sh_name = SectionStrings.size_in_bytes();
    {
        char const s[] = ".symtab";
        SectionStrings.append(s, s + sizeof(s));
    }
    SymTabHeader->sh_type = SHT_SYMTAB;
    SymTabHeader->sh_flags = SHF_ALLOC;
    SymTabHeader->sh_entsize = sizeof(Elf32_Sym);
    SymTabHeader->sh_link = elf_ndxscn(StrTab);

    Elf_Data *SymTabData = elf_newdata(SymTab);
    SymTabData->d_type = ELF_T_SYM;
    SymTabData->d_buf = (void *)Symbols.data();
    SymTabData->d_size = Symbols.size_in_bytes();

    // ========== TODO(rzl): .rodata
    Elf32_Shdr *RodataHeader = elf32_getshdr(Rodata);
    RodataHeader->sh_name = SectionStrings.size_in_bytes();
    {
        char const s[] = ".rodata";
        SectionStrings.append(s, s + sizeof(s));
    }
    RodataHeader->sh_type = SHT_PROGBITS;
    RodataHeader->sh_flags = SHF_ALLOC;
    RodataHeader->sh_link = elf_ndxscn(StrTab);

    Elf_Data *RodataData = elf_newdata(Rodata);
    RodataData->d_buf = (void *)RodataSectionData.data();
    RodataData->d_size = RodataSectionData.size_in_bytes();

    // ========== TODO(rzl)?: .data
    // ========== .text

    Elf32_Shdr *TextHeader = elf32_getshdr(Text);
    TextHeader->sh_name = SectionStrings.size_in_bytes();
    {
        char const s[] = ".text";
        SectionStrings.append(s, s + sizeof(s));
    }
    TextHeader->sh_type = SHT_PROGBITS;
    TextHeader->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
    TextHeader->sh_link = elf_ndxscn(StrTab);

    Elf_Data *TextData = elf_newdata(Text);
    TextData->d_buf = TextSectionData.data();
    TextData->d_size = TextSectionData.size_in_bytes();

    // ========== Write ELF

    // .strtab
    Elf32_Shdr *StrTabHeader = elf32_getshdr(StrTab);
    StrTabHeader->sh_name = SectionStrings.size_in_bytes();
    {
        char const s[] = ".strtab";
        SectionStrings.append(s, s + sizeof(s));
    }
    StrTabHeader->sh_type = SHT_STRTAB;
    StrTabHeader->sh_flags = SHF_STRINGS;

    Elf_Data *StrTabData = elf_newdata(StrTab);
    StrTabData->d_buf = SymbolStrings.data();
    StrTabData->d_size = SymbolStrings.size_in_bytes();

    // .shstrtab
    Elf32_Shdr *ShStrTabHeader = elf32_getshdr(ShStrTab);
    ShStrTabHeader->sh_name = SectionStrings.size_in_bytes();
    {
        char const s[] = ".shstrtab";
        SectionStrings.append(s, s + sizeof(s));
    }
    ShStrTabHeader->sh_type = SHT_STRTAB;
    ShStrTabHeader->sh_flags = SHF_STRINGS;

    Elf_Data *ShStrTabData = elf_newdata(ShStrTab);
    ShStrTabData->d_buf = SectionStrings.data();
    ShStrTabData->d_size = SectionStrings.size_in_bytes();

    if (elf_update(NewElf, ELF_C_WRITE) < 0) {
        std::cerr << "elf_update: " << elf_errmsg(-1) << "\n";
        exit(1);
    }

    size_t sz;
    char *data = elf_rawfile(NewElf, &sz);
    assert(data);

    auto ElfBytes = (char *)xmalloc(sz);
    memcpy(ElfBytes, data, sz);

    (void)elf_end(NewElf);
    llvm::sys::fs::closeFile(FD);

    return {ElfBytes, sz};
}

} // namespace

cl_uint
opencl_llvm_compile_kernel(
    char const *source,
    char const *options,
    char **output_binary,
    unsigned int *output_binary_size) try
{
    InitializeTargets();

    static bool InitializedELF = false;
    if (!InitializedELF) {
        if (elf_version(EV_CURRENT) == EV_NONE) {
            return CL_OUT_OF_HOST_MEMORY;
        }
        InitializedELF = true;
    }


    // This is just a name for the driver, we won't actually touch the disk.
    // The source code is mapped to that name.
    std::string File = "input.cl";
    std::vector<char const *> Options;
    if (options) {
        char *options_copy = xstrdup(options);
        char *saveptr;
        char *tok = strtok_r(options_copy, " \t\b\n\r", &saveptr);
        while (tok) {
            Options.emplace_back(tok);
            tok = strtok_r(nullptr, " \t\b\n\r", &saveptr);
        }
    }

    for (size_t ix = 0; ix < Options.size();) {
        auto &opt = Options[ix];
        if (strcmp(opt, "-D")) {
            if (ix + 1 >= Options.size())
                return CL_INVALID_BUILD_OPTIONS;
            else
                ix += 2;
        }
        if (strcmp(opt, "-I") == 0) {
            if (ix + 1 >= Options.size())
                return CL_INVALID_BUILD_OPTIONS;
            else
                ix += 2;
        }
        if (strcmp(opt, "-cl-single-precision-constant") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-denorms-are-zero") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-opt-disable") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-mad-enable") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-no-signed-zeros") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-unsafe-math-optimizations") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-finite-math-only") == 0)
            ix += 1;
        else if (strcmp(opt, "-cl-fast-relaxed-math") == 0)
            ix += 1;
        else if (strcmp(opt, "-w") == 0)
            ix += 1;
        else if (strcmp(opt, "-Werror") == 0)
            ix += 1;
        else if (strncmp(opt, "-cl-std=", 8) == 0)
            ix += 1;
        else
            return CL_INVALID_BUILD_OPTIONS;
    }
    Options.push_back(File.c_str());

    std::string BuildLog;
    auto Context = std::make_unique<llvm::LLVMContext>();
    Context->setDiagnosticHandlerCallBack(ClangDiagnosticHandler, &BuildLog);
    auto Compiler = CreateCompiler(Options, BuildLog);
    auto FileModule = Compile(*Context, *Compiler, File, source, BuildLog);
    auto AllKernels = std::make_unique<llvm::Module>(File, *Context);
    auto Linker = std::make_unique<llvm::Linker>(*AllKernels);
    Linker->linkInModule(std::move(FileModule));

    std::vector<std::string> KernelNames;
    std::vector<std::vector<char>> InnerBinaries;
    std::vector<std::string> KernelMetadata;
    for (auto *Kernel : GetKernelList(*AllKernels)) {
        auto KernelModule = llvm::CloneModule(*AllKernels);

        // Internalize everything except current kernel
        OptimizeModule(*KernelModule, [&Kernel](const auto &GV) {
            return GV.getName() == Kernel->getName();
        });

        llvm::SmallVector<char, 4096> Binary;
        {
            llvm::raw_svector_ostream OS{Binary};
            auto Cloned = llvm::CloneModule(*KernelModule);
            EmitCode(*Cloned, llvm::TargetMachine::CGFT_ObjectFile, OS);
        }

        auto InnerElf = GetElfFromMemory(Binary);
        auto InnerElfBytes = PrepareOpenCLInnerELF(File, Kernel->getName(), InnerElf.get());
        auto Metadata = GetKernelMetadata(*Kernel);
        KernelNames.emplace_back(Kernel->getName());
        InnerBinaries.emplace_back(std::move(InnerElfBytes));
        KernelMetadata.emplace_back(std::move(Metadata));
    }

    auto Elf = CreateOpenCLElf(KernelNames, InnerBinaries, KernelMetadata);
    *output_binary = Elf.first;
    *output_binary_size = (unsigned int)Elf.second;
    return CL_SUCCESS;
} catch (InvalidBuildOptions &e) {
    return CL_INVALID_BUILD_OPTIONS;
    //} catch (BuildError &e) {
    //   return CL_BUILD_PROGRAM_FAILURE;
    //} catch (FailedToEmitBitcode &e) {
    //   return CL_BUILD_PROGRAM_FAILURE;
}

