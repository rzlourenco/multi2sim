libarch_mips_src = [
  'disassembler/Disassembler.cc',
  'disassembler/Disassembler.h',
  'disassembler/Instruction.cc',
  'disassembler/Instruction.inc.h',
  'disassembler/Instruction.h',
  'emulator/Context.cc',
  'emulator/Context.h',
  'emulator/ContextIsa.cc',
  'emulator/ContextLoader.cc',
  'emulator/ContextSignal.cc',
  'emulator/ContextSyscall.cc',
  'emulator/ContextSyscall.inc.h',
  'emulator/Emulator.cc',
  'emulator/Emulator.h',
  'emulator/Regs.cc',
  'emulator/Regs.h',
  'emulator/Signal.cc',
  'emulator/Signal.h',
]

libarch_mips_lib = static_library('arch-mips',
  sources: libarch_mips_src,
  include_directories: m2s_inc)
libarch_mips = declare_dependency(link_with: libarch_mips_lib,
  include_directories: m2s_inc)
