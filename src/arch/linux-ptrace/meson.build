liblinux_ptrace_src = [
  'Emulator.cc',
  'Emulator.h',
  'i386_errno.inc.h',
  'i386_syscall.inc.h',
  'i386_syscall.cc',
]

liblinux_ptrace_lib = static_library('linux-ptrace',
  sources: liblinux_ptrace_src,
  include_directories: m2s_inc,
)

liblinux_ptrace = declare_dependency(link_with: liblinux_ptrace_lib)
