m2s_inc = include_directories('.', '..')

subdir('lib')
subdir('arch')
subdir('dram')
subdir('memory')
subdir('network')
subdir('visual')

m2s = executable('m2s', sources: 'm2s.cc', install: true,
  dependencies: [threads, zlib])
