#include "arch/common/Emulator.h"
#include "arch/common/Driver.h"
#include "lib/cpp/Debug.h"

#include <functional>
#include <map>
#include <set>
#include <string>
#include <sys/user.h>
#include <vector>

namespace linux_ptrace
{

enum ProcessState {
    PROCESS_STATE_SYSCALL_ENTRY,
    PROCESS_STATE_SYSCALL_EXIT,
    PROCESS_STATE_SIGSTOP_ENTRY,
    PROCESS_STATE_SIGSTOP_EXIT,
    PROCESS_STATE_EXEC_STOP,
};

struct Context {
    pid_t pid;
    int wstatus = 0;
    bool finished = false;
    ProcessState state = PROCESS_STATE_SYSCALL_ENTRY;


    std::map<int, comm::Driver *> fake_fds = {};
    std::set<int> used_fds = {};
    int highest_fd = 2;
    std::function<void(Context&, struct user_regs_struct &)> on_return;

    Context(pid_t pid) : pid{pid} {}
};

class Emulator: public comm::Emulator {
    struct Invocation {
        std::vector<std::string> argv;
        std::vector<std::string> envp;
        std::string cwd;

        Invocation(std::vector<std::string> argv,
                   std::vector<std::string> envp)
            : argv{std::move(argv)}, envp{std::move(envp)} {}
    };

    std::vector<Invocation> pending_invocations;
    std::vector<Context> running_programs;
    std::vector<Context> finished_programs;

public:
    Emulator();
    static comm::Emulator *getInstance();

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

}
