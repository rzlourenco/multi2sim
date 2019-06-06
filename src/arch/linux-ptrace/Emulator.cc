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
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


extern "C" char **environ;

namespace linux_ptrace
{

misc::Debug Emulator::syscall_debug;
std::string Emulator::syscall_debug_file;

namespace {
// From strace:mem.c[get_pagesize]
unsigned long
get_pagesize()
{
    static unsigned long pagesize = 0;
    if (!pagesize) {
        long ret = sysconf(_SC_PAGESIZE);
        if (ret < 0)
            err(EXIT_FAILURE, "sysconf(_SC_PAGESIZE)");
        if (ret == 0)
            err(EXIT_FAILURE, "sysconf(_SC_PAGESIZE) = 0");
        pagesize = ret;
    }
    return pagesize;
}
}

int
FileTable::allocate()
{
    int next_fd = -1;
    for (size_t ix = 0; ix < fd_table.size(); ++ix) {
        auto& fd = fd_table[ix];
        if (fd.is_real_fd && fd.real_fd == -1) {
            next_fd = (int)ix;
            assert(ix == (size_t)next_fd);
        }
    }

    if (next_fd == -1) {
        next_fd = (int)fd_table.size();
        assert((size_t)next_fd == fd_table.size());
        fd_table.emplace_back();
    }

    return next_fd;
}

int
FileTable::open(int real_fd)
{
    assert(real_fd >= 0);

    int fd = allocate();
    fd_table[fd].is_real_fd = true;
    fd_table[fd].real_fd = real_fd;
    return fd;
}

int
FileTable::openDriver(comm::Driver *driver)
{
    assert(driver);

    int fd = allocate();
    fd_table[fd].is_real_fd = false;
    fd_table[fd].real_fd = -1;
    fd_table[fd].driver = driver;
    return fd;
}

comm::Driver *
FileTable::getDriver(int virtual_fd)
{
    if (virtual_fd < 0 || (size_t)virtual_fd >= fd_table.size())
        return nullptr;

    return fd_table[virtual_fd].driver;
}

int
FileTable::getRealFd(int virtual_fd)
{
    if (virtual_fd < 0 || (size_t)virtual_fd >= fd_table.size())
        return -1;
    
    return fd_table[virtual_fd].real_fd;
}

bool
FileTable::close(int fd)
{
    if (fd < 0 && (size_t)fd >= fd_table.size())
        return true;

    fd_table[fd].is_real_fd = true;
    fd_table[fd].real_fd = -1;
    fd_table[fd].driver = nullptr;
    return false;
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
    Invocation invocation {args, env};
    pending_invocations.emplace_back(std::move(invocation));
}

Context&
Emulator::GetOrCreateContext(pid_t pid)
{
    for (auto& ctx : running_programs)
        if (ctx->pid == pid)
            return *ctx;

    // Open stdin, stdout, stderr
    // XXX(rzl): this may be untrue! It's possible these file descriptors
    // have been closed...
    auto ctx = std::make_unique<Context>(pid);
    ctx->fd_table.open(0);
    ctx->fd_table.open(1);
    ctx->fd_table.open(2);
    running_programs.emplace_back(std::move(ctx));
    return *running_programs.back();
}

bool
Emulator::Run()
{
    if (pending_invocations.empty() && running_programs.empty())
        return false;
    if (esim->hasFinished()) {
        // TODO(rzl): kill running programs
        return false;
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
            if (!invocation.envp.empty()) {
                envp.reserve(invocation.envp.size() + 1);
                for (auto& s : invocation.envp)
                    envp.push_back(const_cast<char *>(s.c_str()));
                envp.push_back(nullptr);
            }

            int ret = ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace");

            ret = execve(argv[0], argv.data(), envp.empty() ? environ : envp.data());
            err(EXIT_FAILURE, "execve");
        } else {
            int ret = waitpid(pid, 0, 0);
            if (ret == -1)
                err(EXIT_FAILURE, "waitpid(%d)", pid);
            ret = ptrace(PTRACE_SETOPTIONS, pid, 0,
                    PTRACE_O_EXITKILL|PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACEEXEC|
                    PTRACE_O_TRACECLONE|PTRACE_O_TRACEFORK|PTRACE_O_TRACEVFORK);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace(PTRACE_SET_OPTIONS, %d)", pid);
            ret = ptrace(PTRACE_SYSCALL, pid, 0, 0);
            if (ret == -1)
                err(EXIT_FAILURE, "ptrace(PTRACE_SYSCALL, %d, 0, 0)", pid);

            (void)GetOrCreateContext(pid);
        }
    }
    pending_invocations.clear();

    if (suspended_process) {
        auto &ctx = GetOrCreateContext(suspended_process);
        if (ctx.suspended)
            return true;

        // FIXME(rzl): What about restart_signal?
        int ret = ptrace(PTRACE_SYSCALL, suspended_process, 0, 0);
        assert(ret != -1);
        suspended_process = 0;
    }

    do {
        int wstatus;
        int pid = waitpid(0, &wstatus, __WALL|WNOHANG);
        if (pid < 0) {
            if (errno == EINTR)
                continue;
            if (errno == ECHILD) {
                finished_programs = std::move(running_programs);
                assert(running_programs.empty());
                break;
            }
            err(EXIT_FAILURE, "waitpid");
        }
        if (pid == 0)
            break;

        auto& ctx = GetOrCreateContext(pid);
        ctx.wstatus = wstatus;

        if (WIFEXITED(ctx.wstatus)) {
            Emulator::syscall_debug <<
                misc::fmt("[%d] exited with status %d\n", ctx.pid, WEXITSTATUS(ctx.wstatus));
            continue;
        }
        if (WIFSIGNALED(ctx.wstatus)) {
            Emulator::syscall_debug <<
                misc::fmt("[%d] exited due to signal %d\n", ctx.pid, WTERMSIG(ctx.wstatus));
            continue;
        }
        assert(WIFSTOPPED(ctx.wstatus));
        unsigned int stop_signal = WSTOPSIG(ctx.wstatus);
        unsigned int stop_event = (unsigned int)ctx.wstatus >> 16;
        unsigned int restart_signal = 0;
        auto restart_action = PTRACE_SYSCALL;
        int ret;

        switch (stop_event) {
        case 0: {
            if (stop_signal != (SIGTRAP|0x80)) {
                Emulator::syscall_debug <<
                    misc::fmt("[%d] stopped due to signal %d\n", ctx.pid, WSTOPSIG(ctx.wstatus));
                restart_signal = stop_signal;
            } else if (ctx.state == PROCESS_STATE_SYSCALL_ENTRY) {
                ctx.state = PROCESS_STATE_SYSCALL_EXIT;
                struct user_regs_struct regs;
                ret = ptrace(PTRACE_GETREGS, ctx.pid, 0, &regs);
                assert(ret != -1);
                bool modified = HandleSyscall_i386(ctx, regs);
                if (modified) {
                    ret = ptrace(PTRACE_SETREGS, ctx.pid, 0, &regs);
                    assert(ret != -1);
                }
            } else if (ctx.state == PROCESS_STATE_SYSCALL_EXIT) {
                ctx.state = PROCESS_STATE_SYSCALL_ENTRY;
                struct user_regs_struct regs;
                ret = ptrace(PTRACE_GETREGS, ctx.pid, 0, &regs);
                // Process died, nothing we can do here
                if (ret == -1 && errno == ESRCH)
                    continue;
                assert(ret != -1);
                if (ctx.on_return) {
                    ctx.on_return(ctx, regs);
                    ret = ptrace(PTRACE_SETREGS, ctx.pid, 0, &regs);
                    assert(ret != -1);
                    ctx.on_return = {};
                }
            } else if (ctx.state == PROCESS_STATE_EXEC_STOP) {
                ctx.state = PROCESS_STATE_SYSCALL_ENTRY;
                assert(!ctx.on_return);
            }
            break;
        }

        case PTRACE_EVENT_FORK:
        case PTRACE_EVENT_VFORK:
        case PTRACE_EVENT_CLONE: {
            // FIXME(rzl): This doesn't handle CLONE_FILES or any of the other
            // weird clone flags.
            unsigned long pid;
            ret = ptrace(PTRACE_GETEVENTMSG, ctx.pid, 0, &pid);
            assert(ret != -1);

            auto &child_ctx = GetOrCreateContext((pid_t)pid);
            child_ctx.fd_table = ctx.fd_table;
            break;
        }

        default:
            break;
        }

        // XXX(rzl): If this context gets suspended, don't process more
        // events until it gets unsuspended. This is so that we are still
        // in the appropriate syscall-stop when we resume it.
        // XXX(rzl): Is there a better way to do it?
        if (ctx.suspended) {
            assert(restart_signal == 0);
            assert(suspended_process == 0);
            suspended_process = ctx.pid;
            break;
        } else {
            ret = ptrace(restart_action, ctx.pid, 0, restart_signal);
            assert(ret != -1);
        }
    } while (true);

    return !running_programs.empty();
}

Emulator *
Emulator::getInstance()
{
    static std::unique_ptr<Emulator> instance;
    if (!instance)
        instance = std::make_unique<Emulator>();
    return instance.get();
}

std::string
Memory::ReadString(unsigned address, int max_length)
{
    assert(max_length > 0);
    if (!address)
        return "";

    auto page_size = get_pagesize();
    auto page_mask = page_size - 1;
    auto buffer = std::make_unique<char[]>(max_length);

    // Check if we have already found a \0 before crossing a page boundary
    // otherwise we will get a SEGV.
    int total_read = 0;
    while (total_read < max_length) {
        unsigned page_boundary = (address + page_size - 1) & ~page_mask;
        if (page_boundary - address == 0)
            page_boundary += page_size;
        int read_size = std::min<int>(page_boundary - address, max_length - total_read);
        Read(address, read_size, buffer.get() + total_read);

        if (auto p = memchr(buffer.get() + total_read, '\0', read_size)) {
            total_read = (char *)p - buffer.get();
            break;
        }

        total_read += read_size;
        address += read_size;
    }

    return std::string(buffer.get(), buffer.get() + total_read);
}

void
Memory::Access(unsigned address, unsigned size, char *buffer, AccessType access)
{
    //dprintf(256, "Memory::Access(0x%x, %u, %u)\n", address, size, access);

    struct iovec local[1], remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = (void *)(uintptr_t)address;
    remote[0].iov_len = size;

    int ret;
    switch (access) {
    case AccessRead:
        ret = process_vm_readv(ctx.pid, local, 1, remote, 1, 0);
        if (ret == -1)
            err(EXIT_FAILURE, "process_vm_readv(%p, %u, %p)", (void *)(uintptr_t)address, size, buffer);
        break;
    case AccessWrite:
        ret = process_vm_writev(ctx.pid, local, 1, remote, 1, 0);
        if (ret == -1)
            err(EXIT_FAILURE, "process_vm_writev(%p, %u, %p)", (void *)(uintptr_t)address, size, buffer);
        break;
    default:
        abort();
        break;
    }

    assert((unsigned)ret == size);
}

void
Memory::WriteString(unsigned address, std::string const &s)
{
    abort();
}

bool
Context::isSuspended()
{
    return suspended;
}

void
Context::Suspend()
{
    suspended = true;
}

void
Context::Wakeup()
{
    suspended = false;
}

} // namespace linux_ptrace



