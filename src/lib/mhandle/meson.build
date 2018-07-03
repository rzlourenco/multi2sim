libmhandle_src = ['mhandle.c', 'mhandle.h']

libmhandle_lib = static_library('mhandle',
  sources: libmhandle_src,
  include_directories: m2s_inc)
libmhandle = declare_dependency(
  link_with: libmhandle_lib,
  include_directories: m2s_inc)

