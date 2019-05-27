m2s_inc = include_directories('.', '..')

deps = []

subdir('lib')
deps += [libcpp, libesim, libmhandle, libutil]
subdir('network')
deps += [libnetwork]
subdir('memory')
deps += [libmemory]
subdir('dram')
deps += [libdram]
subdir('arch')
deps += [libarch_common, libarch_arm, libarch_hsa, libarch_kepler,
         libarch_mips, libarch_southernislands, libarch_x86]
subdir('visual')
deps += [libvisual]

if host_machine.system() == 'linux'
    deps += [liblinux_ptrace]
endif

m2s = executable('m2s', sources: 'm2s.cc', install: true,
  dependencies: [threads, zlib, deps])
