libdram_src = [
  'Address.cc',
  'Address.h',
  'Bank.cc',
  'Bank.h',
  'Channel.cc',
  'Channel.h',
  'Command.cc',
  'Command.h',
  'Controller.cc',
  'Controller.h',
  'Rank.cc',
  'Rank.h',
  'Request.cc',
  'Request.h',
  'Scheduler.cc',
  'Scheduler.h',
  'System.cc',
  'System.h',
  'TimingParameters.cc',
  'TimingParameters.h',
]

libdram_lib = static_library('dram',
  sources: libdram_src,
  include_directories: m2s_inc)
libdram = declare_dependency(
  link_with: libdram_lib,
  include_directories: m2s_inc)
