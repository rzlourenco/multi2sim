#include "arch/common/Emulator.h"
#include "arch/common/Context.h"
#include "arch/common/Driver.h"
#include "lib/cpp/Debug.h"
#include "memory/Memory.h"

#include <functional>
#include <map>
#include <set>
#include <string>
#include <sys/user.h>
#include <vector>

namespace linux_ptrace
{

struct Context;

class Emulator: public comm::Emulator {
    struct Invocation {
        std::vector<std::string> argv;
        std::vector<std::string> envp;
        std::string cwd;

        Invocation(std::vector<std::string> argv,
                   std::vector<std::string> envp)
            : argv{std::move(argv)}, envp{std::move(envp)} {}
    };

    std::vector<Invocation> pending_invocations = {};
    std::vector<std::unique_ptr<Context>> running_programs = {};
    std::vector<std::unique_ptr<Context>> finished_programs = {};
    pid_t suspended_process = 0;

    Context& GetOrCreateContext(pid_t pid);

public:
    Emulator();
    static Emulator *getInstance();

    static void RegisterOptions();
    static void ProcessOptions();


    void LoadProgram(const std::vector<std::string> &args,
            const std::vector<std::string> &env = {},
            const std::string &cwd = "",
            const std::string &stdin_file_name = "",
            const std::string &stdout_file_name = "") override;

    bool Run() override;

public:
    static misc::Debug syscall_debug;
    static std::string syscall_debug_file;
};

struct FileDescriptor {
    comm::Driver *driver;
    int real_fd;
    bool is_real_fd : 1;

    FileDescriptor() : driver{nullptr}, real_fd{-1}, is_real_fd{false} {}
};

class FileTable {
    std::vector<FileDescriptor> fd_table;

    int allocate();

public:
    int open(int real_fd);
    int openDriver(comm::Driver *);
    comm::Driver *getDriver(int virtual_fd);
    int getRealFd(int virtual_fd);
    bool close(int fd);
};


class Memory final : public mem::BaseMemory {
    struct Context &ctx;

public:
    Memory(Context &ctx) : ctx{ctx} {}

    void setSafe(bool) override { abort(); }
    void setSafeDefault() override { abort(); }
    bool getSafe() const override { abort(); }
    void Clear() override { abort(); }
    Page *getPage(unsigned) override { abort(); }
    Page *getNextPage(unsigned) override { abort(); }
    void Map(unsigned, unsigned, unsigned) override { abort(); }
    void Unmap(unsigned, unsigned) override { abort(); }
    unsigned MapSpace(unsigned, unsigned) override { abort(); }
    unsigned MapSpaceDown(unsigned, unsigned) override { abort(); }
    void Protect(unsigned, unsigned, unsigned) override { abort(); }
    void Copy(unsigned, unsigned, unsigned) override { abort(); }

    void Access(unsigned address, unsigned size, char *buffer, AccessType access) override;
    std::string ReadString(unsigned address, int max_length = 1024) override;
    void WriteString(unsigned address, std::string const &s) override;

    void Zero(unsigned, unsigned) override { abort(); }
    char *getBuffer(unsigned, unsigned, AccessType) override { abort(); }
    void Save(const std::string &, unsigned, unsigned) override { abort(); }
    void Load(const std::string &, unsigned) override { abort(); }
    void setHeapBreak(unsigned) override { abort(); }
    void growHeapBreak(unsigned) override { abort(); }
    unsigned getHeapBreak() override { abort(); }

};

enum ProcessState {
    PROCESS_STATE_SYSCALL_ENTRY,
    PROCESS_STATE_SYSCALL_EXIT,
    PROCESS_STATE_EXEC_STOP,
};

struct Context : public comm::Context {
    pid_t pid;
    int wstatus = 0;
    bool finished = false;
    bool suspended = false;
    ProcessState state = PROCESS_STATE_SYSCALL_ENTRY;

    FileTable fd_table;
    std::unique_ptr<Memory> memory;
    std::function<void(Context&, struct user_regs_struct &)> on_return;

    Context(pid_t pid)
        : comm::Context(Emulator::getInstance()), pid{pid},
          memory{std::make_unique<Memory>(*this)} {}

    bool isSuspended() override;
    void Suspend() override;
    void Wakeup() override;
};

bool HandleSyscall_i386(Context &ctx, struct user_regs_struct &regs);

}
