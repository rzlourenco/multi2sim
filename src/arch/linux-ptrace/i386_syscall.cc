#include "Emulator.h"

#include "lib/cpp/String.h"

#include <cstring>
#include <linux/limits.h>
#include <sys/prctl.h>

namespace linux_ptrace {

namespace {

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

}

bool
HandleSyscall_i386(Context& ctx, struct user_regs_struct &regs)
{
    bool modified = false;
    auto const syscall = regs.orig_rax;
    std::reference_wrapper<unsigned long long> args[6] = {
        std::ref(regs.rbx),
        std::ref(regs.rcx),
        std::ref(regs.rdx),
        std::ref(regs.rsi),
        std::ref(regs.rdi),
        std::ref(regs.rbp),
    };

    Emulator::syscall_debug << misc::fmt("[%d] %s(", ctx.pid, syscall_names[syscall]);
    switch (syscall) {
    case SYS_access: {
        std::string path = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
        int mode = (int)args[1];
        Emulator::syscall_debug << misc::fmt("\"%s\", %d", path.c_str(), mode);

        auto *driver_pool = comm::DriverPool::getInstance();
        if (path == "/dev/multi2sim" || driver_pool->isPathRegistered(path)) {
            ctx.on_return = [](auto &, auto &regs) {
                regs.rax = 0;
            };
        }
        break;
    }

    case SYS_open:
    case SYS_openat: {
        int dirfd;
        std::string pathname;
        int flags;
        mode_t mode;

        if (syscall == SYS_open) {
            pathname = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
            flags = args[1];
            mode = args[2];
            Emulator::syscall_debug << misc::fmt("\"%s\", %d, %o",
                    pathname.c_str(), flags, mode);
        } else if (syscall == SYS_openat) {
            dirfd = args[0];
            pathname = ctx.memory->ReadString((unsigned)args[1], PATH_MAX);
            flags = args[2];
            mode = args[3];
            Emulator::syscall_debug << misc::fmt("%d, \"%s\", %d, %o",
                    dirfd, pathname.c_str(), flags, mode);
        } else {
            assert(false);
        }

        auto *driver_pool = comm::DriverPool::getInstance();
        if (driver_pool->isPathRegistered(pathname)) {
            comm::Driver *driver = driver_pool->getDriverByPath(pathname);
            ctx.on_return = [driver](auto &ctx, auto &regs) {
                // Invent a new fd that maps to the respective driver
                int new_fd = ctx.fd_table.openDriver(driver);
                regs.rax = new_fd;
            };
        } else {
            ctx.on_return = [](auto &ctx, auto &regs) {
                auto ret = (long long)regs.rax;
                if (ret >= 0) {
                    // Invent a new fd that maps to the returned file descriptor
                    int real_fd = (int)ret;
                    int new_fd = ctx.fd_table.open(real_fd);
                    regs.rax = new_fd;
                }
            };
        }

        // FIXME(rzl): printing
        goto default_printing;
    }

    case SYS_memfd_create: {
        std::string pathname = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
        int flags = args[1];

        Emulator::syscall_debug << misc::fmt("\"%s\", %d", pathname.c_str(), flags);
        ctx.on_return = [](auto &ctx, auto &regs) {
            auto ret = (long long)regs.rax;
            if (ret >= 0) {
                int real_fd = (int)ret;
                int new_fd = ctx.fd_table.open(real_fd);
                regs.rax = new_fd;
            }
        };
        break;
    }

    case SYS_read:
    case SYS_write: {
        int virtual_fd = (int)args[0];
        void *buf = (void *)args[1].get();
        size_t count = (size_t)args[2];

        Emulator::syscall_debug << misc::fmt("%d, %p, %zd",
                virtual_fd, buf, count);

        int fd = ctx.fd_table.getRealFd(virtual_fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_pread64:
    case SYS_pwrite64: {
        int virtual_fd = (int)args[0];
        void *buf = (void *)args[1].get();
        size_t count = (size_t)args[2];
        off_t offset = (size_t)args[3];

        Emulator::syscall_debug << misc::fmt("%d, %p, %zd, %zd",
                virtual_fd, buf, count, offset);

        int fd = ctx.fd_table.getRealFd(virtual_fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_ioctl: {
        auto virtual_fd = (int)args[0];
        auto command = (unsigned)args[1];
        auto argument = (unsigned)args[2];

        Emulator::syscall_debug << misc::fmt("%d, %p, %lld", virtual_fd,
                (void *)args[1].get(), (long long)args[2]);

        if (auto *driver = ctx.fd_table.getDriver(virtual_fd)) {
            Emulator::getInstance()->StopTimer();
            int ret = driver->Call(&ctx, ctx.memory.get(), command, argument);
            Emulator::getInstance()->StartTimer();
            ctx.on_return = [ret](auto &, auto &regs) {
                regs.rax = ret;
            };
        } else {
            int fd = ctx.fd_table.getRealFd(virtual_fd);
            args[0].get() = fd;
            modified = true;
        }

        break;
    }

    case SYS_close: {
        auto virtual_fd = (int)args[0];
        Emulator::syscall_debug << misc::fmt("%d", virtual_fd);

        int fd = ctx.fd_table.getRealFd(virtual_fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_prctl: {
        int option = (int)args[0];
        int arg2 = (int)args[1];
        int arg3 = (int)args[2];
        int arg4 = (int)args[3];
        int arg5 = (int)args[4];

        Emulator::syscall_debug << misc::fmt("%d, %d, %d, %d, %d",
                option, arg2, arg3, arg4, arg5);

        if (option == PR_SET_MM) {
            if (arg2 == PR_SET_MM_EXE_FILE) {
                int fd = ctx.fd_table.getRealFd(arg3);
                args[2].get() = fd;
                modified = true;
            }
        }
        break;
    }

    case SYS_mmap:
    case SYS_mmap2: {
        void *addr {(void *)args[0].get()};
        size_t length {args[1].get()};
        int prot {(int)args[2].get()};
        int flags {(int)args[3].get()};
        int fd {(int)args[4].get()};
        off_t offset {(int)args[5].get()};
        Emulator::syscall_debug << misc::fmt("%p, %zu, %u, %u, %d, %zd",
                addr, length, prot, flags, fd, offset);

        fd = ctx.fd_table.getRealFd(fd);
        args[4].get() = fd;
        modified = true;
        break;
    }

    case SYS_llseek: {
        int fd {(int)args[0].get()};
        unsigned long offset_high {(unsigned long)args[1].get()};
        unsigned long offset_low {(unsigned long)args[2].get()};
        loff_t *result {(loff_t *)args[3].get()};
        unsigned int whence {(unsigned int)args[4].get()};
        Emulator::syscall_debug << misc::fmt("%d, %lu, %lu, %p, %u",
                fd, offset_high, offset_low, result, whence);

        fd = ctx.fd_table.getRealFd(fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_fstat:
    case SYS_fstat64: {
        int fd {(int)args[0].get()};
        void *statbuf {(void*)args[1].get()};
        
        Emulator::syscall_debug << misc::fmt("%d, %p", fd, statbuf);
        fd = ctx.fd_table.getRealFd(fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_fcntl:
    case SYS_fcntl64: {
        int fd {(int)args[0].get()};
        int cmd {(int)args[1].get()};
        
        Emulator::syscall_debug << misc::fmt("%d, %d", fd, cmd);
        fd = ctx.fd_table.getRealFd(fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_ftruncate:
    case SYS_ftruncate64: {
        int fd = (int)args[0].get();
        off_t length = (off_t)args[1].get();
        
        Emulator::syscall_debug << misc::fmt("%d, %d", fd, (int)length);
        fd = ctx.fd_table.getRealFd(fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    case SYS_readv:
    case SYS_writev: {
        int fd {(int)args[0].get()};
        void *iov {(void *)args[1].get()};
        int iovcnt {(int)args[2].get()};

        Emulator::syscall_debug << misc::fmt("%d, %p, %d", fd, iov, iovcnt);
        fd = ctx.fd_table.getRealFd(fd);
        args[0].get() = fd;
        modified = true;
        break;
    }

    // FIXME(rzl): these need their paths adjusted if they are a device we emulate.
    case SYS_readlink: {
        std::string pathname = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
        void *buf = (void *)args[1].get();
        size_t bufsiz = args[1];
        Emulator::syscall_debug << misc::fmt("\"%s\", %p, %zu", pathname.c_str(), buf, bufsiz);
        break;
    }
    case SYS_stat:
    case SYS_stat64: {
        std::string pathname = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
        void *statbuf = (void *)args[1].get();
        Emulator::syscall_debug << misc::fmt("\"%s\", %p", pathname.c_str(), statbuf);
        break;
    }
    case SYS_lstat:
    case SYS_lstat64: {
        std::string pathname = ctx.memory->ReadString((unsigned)args[0], PATH_MAX);
        void *statbuf = (void *)args[1].get();
        Emulator::syscall_debug << misc::fmt("\"%s\", %p", pathname.c_str(), statbuf);
        break;
    }

    // Nothing to do in these syscalls
    case SYS_arch_prctl:
    case SYS_brk:
    case SYS_chdir:
    case SYS_clone:
    case SYS_exit:
    case SYS_exit_group:
    case SYS_fork:
    case SYS_futex:
    case SYS_get_thread_area:
    case SYS_getpid:
    case SYS_getrlimit:
    case SYS_gettid:
    case SYS_gettimeofday:
    case SYS_kill:
    case SYS_madvise:
    case SYS_mount:
    case SYS_mprotect:
    case SYS_munmap:
    case SYS_nanosleep:
    case SYS_newuname:
    case SYS_olduname:
    case SYS_remap_file_pages:
    case SYS_rt_sigaction:
    case SYS_rt_sigprocmask:
    case SYS_sched_get_priority_max:
    case SYS_sched_getaffinity:
    case SYS_sched_getparam:
    case SYS_sched_getscheduler:
    case SYS_sched_setaffinity:
    case SYS_sched_setscheduler:
    case SYS_sched_yield:
    case SYS_set_robust_list:
    case SYS_set_thread_area:
    case SYS_set_tid_address:
    case SYS_sigaction:
    case SYS_sigaltstack:
    case SYS_sigprocmask:
    case SYS_tgkill:
    case SYS_time:
    case SYS_tkill:
    case SYS_uname:
    case SYS_vfork:
    case SYS_wait4:
        goto default_printing;

    default:
        // Disable ALL unhandled syscalls.
        regs.orig_rax = -1;
        modified = true;
        // fallthrough

    default_printing:
        Emulator::syscall_debug << "...";
        break;
    }
    Emulator::syscall_debug << ")";

    ctx.on_return = [old_fn=ctx.on_return](Context &ctx, struct user_regs_struct &regs) {
        if (old_fn)
            old_fn(ctx, regs);
        auto syscall_ret = (long long)regs.rax;
        Emulator::syscall_debug << " = " << syscall_ret;
        if (syscall_ret < 0 && syscall_ret > -ERR_MAX_ERRNO)
            Emulator::syscall_debug << " (" << errno_names[-syscall_ret] << ")";
        Emulator::syscall_debug << '\n';

        if (regs.orig_rax == SYS_execve)
            ctx.state = PROCESS_STATE_EXEC_STOP;
        else
            ctx.state = PROCESS_STATE_SYSCALL_ENTRY;
    };

    return modified;
}

} // namespace linux_ptrace
