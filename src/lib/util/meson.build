libutil_src = [
  'bin-config.c',
  'bin-config.h',
  'bit-map.c',
  'bit-map.h',
  'buffer.c',
  'buffer.h',
  'config.c',
  'config.h',
  'debug.c',
  'debug.h',
  'elf-encode.c',
  'elf-encode.h',
  'elf-format.c',
  'elf-format.h',
  'file.c',
  'file.h',
  'graph.c',
  'graph.h',
  'hash-table.c',
  'hash-table.h',
  'heap.c',
  'heap.h',
  'list.c',
  'list.h',
  'linked-list.c',
  'linked-list.h',
  'misc.c',
  'misc.h',
  'matrix.c',
  'matrix.h',
  'repos.c',
  'repos.h',
  'stringutil.c',
  'stringutil.h',
  'timer.c',
  'timer.h',
]

libutil_lib = static_library('util',
  sources: libutil_src,
  include_directories: m2s_inc)
libutil = declare_dependency(
  link_with: libutil_lib,
  include_directories: m2s_inc)
