libm2s_opencl_src = [
  'command.c',
  'command.h',
  'command-queue.c',
  'command-queue.h',
  'context.c',
  'context.h',
  'debug.c',
  'debug.h',
  'device.c',
  'device.h',
  'elf-format.c',
  'elf-format.h',
  'event.c',
  'event.h',
  'evg-device.c',
  'evg-device.h',
  'evg-kernel.c',
  'evg-kernel.h',
  'evg-program.c',
  'evg-program.h',
  'list.c',
  'list.h',
  'llvm.cc',
  'llvm.h',
  'mhandle.c',
  'mhandle.h',
  'misc.c',
  'misc.h',
  'program.c',
  'program.h',
  'kernel.c',
  'kernel.h',
  'mem.c',
  'mem.h',
  'object.c',
  'object.h',
  'opencl.c',
  'opencl.h',
  'platform.c',
  'platform.h',
  'sampler.c',
  'sampler.h',
  'si-device.c',
  'si-device.h',
  'si-kernel.c',
  'si-kernel.h',
  'si-program.c',
  'si-program.h',
  'string-util.c',
  'string-util.h',
  'thread-list.c',
  'thread-list.h',
  'x86-device.c',
  'x86-device.h',
  'x86-kernel.c',
  'x86-kernel.h',
  'x86-program.c',
  'x86-program.h',
  'union-device.c',
  'union-device.h',
  'union-kernel.c',
  'union-kernel.h',
  'union-program.c',
  'union-program.h',
  'x86-device-wi.s',
]

libm2s_opencl_lib = both_libraries('m2s-opencl',
  install: true,
  sources: libm2s_opencl_src,
  include_directories: m2s_inc,
  dependencies: [have_m32, threads, libdl, llvm, clang, libelf],
  c_args: ['-fno-sanitize=all'],
  cpp_args: ['-fno-sanitize=all',
    '-DLIBCLC_INCLUDEDIR="@0@/"'.format(libclc.get_pkgconfig_variable('includedir')),
    '-DLIBCLC_LIBEXECDIR="@0@/"'.format(libclc.get_pkgconfig_variable('libexecdir')),
  ],
  link_args: ['-fno-sanitize=all', '-Wl,-z,notext'])
