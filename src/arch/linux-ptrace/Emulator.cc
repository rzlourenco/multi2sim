#include "Emulator.h"

#include "arch/common/Driver.h"
#include "lib/cpp/CommandLine.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cstring>
#include <err.h>
#include <memory>
#include <linux/limits.h>
#include <sys/ptrace.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>


namespace linux_ptrace
{

misc::Debug Emulator::syscall_debug;
std::string Emulator::syscall_debug_file;

namespace {

static bool
ReadSyscallString(pid_t pid, void *address, size_t max_size, char *output);

enum {
#define DEFSYSCALL(name, nr) SYS_##name = nr,
#include "i386_syscall.inc.h"
#undef DEFSYSCALL
};
enum {
#define DEFERRNO(name, nr) ERR_##name = nr,
#include "i386_errno.inc.h"
#undef DEFERRNO
    ERR_MAX_ERRNO,
};

char const * const syscall_names[] = {
#define DEFSYSCALL(name, nr) [nr] = #name,
#include "i386_syscall.inc.h"
#undef DEFSYSCALL
};
char const * const errno_names[] = {
#define DEFERRNO(name, nr) [nr] = #name,
#include "i386_errno.inc.h"
#undef DEFERRNO
};

bool
HandleSyscall(Context& ctx, struct user_regs_struct &regs)
{
    auto const syscall = regs.orig_rax;
    uint64_t const args[7] = {
        regs.rbx,
        regs.rcx,
        regs.rdx,
        regs.rsi,
        regs.rdi,
        regs.rbp,
        0
    };

    Emulator::syscall_debug << misc::fmt("[%d] %s(...)", ctx.pid, syscall_names[syscall]);

    switch (syscall) {
    case SYS_access: {
        auto *driver_pool = comm::DriverPool::getInstance();

        char path[PATH_MAX] = {0};
        bool valid = ReadSyscallString(ctx.pid, (void*)args[0], sizeof(path)-1, path);

        if (valid) {
            if (!strcmp(path, "/dev/multi2sim") || driver_pool->isPathRegistered(path)) {
                ctx.on_return = [](Context &, struct user_regs_struct &regs) {
                    regs.rax = 0;
                };
            }
        }

        break;
    }
    case SYS_open:
    case SYS_openat: {
        auto *driver_pool = comm::DriverPool::getInstance();

        char path[PATH_MAX] = {0};
        bool valid;
        if (syscall == SYS_open)
            valid = ReadSyscallString(ctx.pid, (void*)args[0], sizeof(path)-1, path);
        else if (syscall == SYS_openat)
            valid = ReadSyscallString(ctx.pid, (void*)args[1], sizeof(path)-1, path);
        else
            assert(false);

        if (valid) {
            if (driver_pool->isPathRegistered(path)) {
                comm::Driver *driver = driver_pool->getDriverByPath(path);
                ctx.on_return = [](Context&, struct user_regs_struct &regs) {
                    regs.rax = -ERR_ENOENT;
                };
            } else {
                ctx.on_return = [](Context& ctx, struct user_regs_struct &regs) {
                    if (regs.rax >= 0)
                        ctx.used_fds.insert(regs.rax);
                };
            }
        }
        break;
    }
    default:
        break;
    }

    return false;
}

static bool
ReadSyscallString(pid_t pid, void *address, size_t max_size, char *output)
{
    if (!address)
        return false;

    struct iovec local[1], remote[1];

    local[0].iov_base = output;
    local[0].iov_len = max_size;
    remote[0].iov_base = address;
    remote[0].iov_len = max_size;

    // TODO(rzl): This might read a lot more memory than needed, leading to a
    // page (our) fault.
    int nread = process_vm_readv(pid, local, 1, remote, 1, 0);
    if (nread == -1)
        err(EXIT_FAILURE, "process_vm_readv");

    bool foundNul = false;
    for (int ix = 0; ix < nread && !foundNul; ++ix)
        if (output[ix] == '\0')
            foundNul = true;

    return foundNul;
}


}

Emulator::Emulator() : comm::Emulator("ptrace")
{ }

void
Emulator::RegisterOptions()
{
    auto *command_line = misc::CommandLine::getInstance();
    command_line->RegisterString("--ptrace-debug-syscall", syscall_debug_file,
            "Dump all traced syscalls, including code, arguments, and return value.");
}

void
Emulator::ProcessOptions()
{
    syscall_debug.setPath(syscall_debug_file);
}

void
Emulator::LoadProgram(
    const std::vector<std::string> &args,
    const std::vector<std::string> &env,
    const std::string &,
    const std::string &,
    const std::string &)
{
    assert(args.size() > 0);
    pending_invocations.emplace_back(args, env);
}

bool
Emulator::Run()
{
    if (pending_invocations.empty() && running_programs.empty())
        return false;
    if (esim->hasFinished()) {
        // TODO(rzl): kill running programs
        return true;
    }

    for (auto& invocation : pending_invocations) {
        pid_t pid = fork();
        if (pid == -1)
            throw misc::Panic(misc::fmt("Cannot fork: %s", strerror(errno)));
        if (pid == 0) {
            std::vector<char *> argv;
            argv.reserve(invocation.argv.size() + 1);
            for (auto& s : invocation.argv)
                argv.push_back(const_cast<char *>(s.c_str()));
            argv.push_back(nullptr);

            std::vector<char *> envp;
            envp.reserve(invocation.envp.size() + 1);
            for (auto& s : invocation.envp)
                envp.push_back(const_cast<char *>(s.c_str()));
            envp.push_back(nullptr);

            int ret = ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace");

            ret = execve(argv[0], argv.data(), envp.data());
            err(EXIT_FAILURE, "execve");
        } else {
            int ret = waitpid(pid, 0, 0);
            if (ret == -1)
                err(EXIT_FAILURE, "waitpid(%d)", pid);
            ret = ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace(PTRACE_SET_OPTIONS, %d)", pid);
            ret = ptrace(PTRACE_SYSCALL, pid, 0, 0);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace(PTRACE_SYSCALL, %d, 0, 0)", pid);

            running_programs.emplace_back(pid);
        }
    }
    pending_invocations.clear();

    for (auto& context : running_programs) {
        // Wait until just before a syscall.
        int ret;
        while ((ret = waitpid(context.pid, &context.wstatus, 0)) == -1) {
            if (errno == EINTR)
                continue;
            err(EXIT_FAILURE, "waitpid");
        }
        assert(ret == context.pid || ret == 0);

        if (WIFEXITED(context.wstatus)) {
            Emulator::syscall_debug <<
                misc::fmt("\nProcess %d exited with status %d\n", context.pid, WEXITSTATUS(context.wstatus));
            context.finished = true;
            continue;
        }
        if (WIFSIGNALED(context.wstatus)) {
            Emulator::syscall_debug <<
                misc::fmt("\nProcess %d exited due to signal %d\n", context.pid, WTERMSIG(context.wstatus));
            context.finished = true;
            continue;
        }

        assert(WIFSTOPPED(context.wstatus));

        if (WSTOPSIG(context.wstatus) != SIGTRAP) {
            Emulator::syscall_debug <<
                misc::fmt("Process %d stopped due to signal %d\n", context.pid, WSTOPSIG(context.wstatus));
        } else if (context.state == PROCESS_STATE_SYSCALL_ENTRY) {
            struct user_regs_struct regs;
            ret = ptrace(PTRACE_GETREGS, context.pid, 0, &regs);
            assert(ret != -1);

            bool modified = HandleSyscall(context, regs);
            if (modified) {
                ret = ptrace(PTRACE_SETREGS, context.pid, 0, &regs);
                assert(ret != -1);
            }

            context.state = PROCESS_STATE_SYSCALL_EXIT;
        } else if (context.state == PROCESS_STATE_SYSCALL_EXIT) {
            struct user_regs_struct regs;
            int ret = ptrace(PTRACE_GETREGS, context.pid, 0, &regs);
            assert(ret != -1);
            if (context.on_return) {
                context.on_return(context, regs);
                ret = ptrace(PTRACE_SETREGS, context.pid, 0, &regs);
                assert(ret != -1);
                context.on_return = {};
                assert(!context.on_return);
            }

            auto syscall_ret = (long long)regs.rax;
            Emulator::syscall_debug << " = " << syscall_ret;
            if (syscall_ret < 0 && syscall_ret > -ERR_MAX_ERRNO)
                Emulator::syscall_debug << " (" << errno_names[-syscall_ret] << ")";
            Emulator::syscall_debug << '\n';

            if (regs.orig_rax == SYS_execve)
                context.state = PROCESS_STATE_EXEC_STOP;
            else
                context.state = PROCESS_STATE_SYSCALL_ENTRY;
        } else if (context.state == PROCESS_STATE_EXEC_STOP) {
            struct user_regs_struct regs;
            int ret = ptrace(PTRACE_GETREGS, context.pid, 0, &regs);
            assert(ret != -1);
            auto syscall_ret = (long long)regs.rax;
            Emulator::syscall_debug << " = " << syscall_ret;
            if (syscall_ret < 0 && syscall_ret > -ERR_MAX_ERRNO)
                Emulator::syscall_debug << " (" << errno_names[-syscall_ret] << ")";
            Emulator::syscall_debug << '\n';
        }

        // FIXME(rzl): this is i386 specific!
        // TODO(rzl): special-case SI, Kepler, and HSA ioctls.
        // TODO(rzl): fake file descriptors when opening a driver's device file.
        // TODO(rzl): add support for PTRACE_GET_SYSCALL_INFO when the kernel
        // patch lands.

        // Run the possibly modified syscall.
        ret = ptrace(PTRACE_SYSCALL, context.pid, 0, 0);
        assert(ret != -1);
    }

    // NOTE(rzl): this is quadratic but it should be okay since we don't
    // expect to have many processes.
    auto it = std::remove_if(running_programs.begin(), running_programs.end(),
            [](const auto& running) { return running.finished; });
    finished_programs.insert(finished_programs.end(), it, running_programs.end());
    running_programs.erase(it, running_programs.end());

    return !running_programs.empty();
}

comm::Emulator *
Emulator::getInstance()
{
    static std::unique_ptr<comm::Emulator> instance;
    if (!instance)
        instance = std::make_unique<Emulator>();
    return instance.get();
}

} // namespace linux_ptrace



