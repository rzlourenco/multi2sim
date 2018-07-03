libesim_src = [
  'Engine.cc',
  'Engine.h',
  'Event.cc',
  'Event.h',
  'Frame.cc',
  'Frame.h',
  'FrequencyDomain.cc',
  'FrequencyDomain.h',
  'Queue.cc',
  'Queue.h',
  'Trace.cc',
  'Trace.h',
]

libesim_lib = static_library('esim',
  sources: libesim_src,
  include_directories: m2s_inc)
libesim = declare_dependency(
  link_with: libesim_lib,
  include_directories: m2s_inc)
