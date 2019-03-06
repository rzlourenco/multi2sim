libarch_common_src = [
  'Arch.cc',
  'Arch.h',
  'CallStack.cc',
  'CallStack.h',
  'Context.cc',
  'Context.h',
  'Disassembler.cc',
  'Disassembler.h',
  'Driver.cc',
  'Driver.h',
  'Emulator.cc',
  'Emulator.h',
  'FileTable.cc',
  'FileTable.h',
  'Runtime.cc',
  'Runtime.h',
  'Timing.cc',
  'Timing.h',
]

libarch_common_lib = static_library('arch_common',
  sources: libarch_common_src,
  include_directories: m2s_inc)
libarch_common = declare_dependency(
  link_with: libarch_common_lib,
  include_directories: m2s_inc)


