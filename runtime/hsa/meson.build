libm2s_hsa_src = [
  'debug.h',
  'debug.c',
  'hsa.h',
  'hsa.c',
  'hsa_agent.c',
  'hsa_code_object.c',
  'hsa_executable.c',
  'hsa_finalize.c',
  'hsa_isa.c',
  'hsa_queue.c',
  'hsa_region.c',
  'hsa_signal.c',
  'hsa_system.c',
]

libm2s_hsa_lib = both_libraries('m2s-hsa',
  install: true,
  sources: libm2s_hsa_src,
  dependencies: [have_m32])
