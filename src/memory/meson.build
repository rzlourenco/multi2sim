libmemory_src = [
  'Cache.cc',
  'Cache.h',
  'Directory.cc',
  'Directory.h',
  'Frame.cc',
  'Frame.h',
  'Memory.cc',
  'Memory.h',
  'Manager.cc',
  'Manager.h',
  'Mmu.cc',
  'Mmu.h',
  'Module.cc',
  'Module.h',
  'SpecMem.cc',
  'SpecMem.h',
  'System.cc',
  'SystemConfig.cc',
  'SystemEvents.cc',
  'System.h',
]

libmemory_lib = static_library('memory',
  sources: libmemory_src,
  include_directories: m2s_inc)
libmemory = declare_dependency(
  link_with: libmemory_lib,
  include_directories: m2s_inc)

