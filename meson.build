project('multi2sim', ['c', 'cpp'], version: '5.0',
  default_options: ['c_std=c99', 'cpp_std=c++11'])

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

threads = dependency('threads')
zlib = dependency('zlib')
libdl = cc.find_library('dl', required: false)
gtk3 = dependency('gtk+-3.0', required: false)

add_global_arguments('-DVERSION="5.0"', language: ['c', 'cpp'])
add_global_arguments('-D_GNU_SOURCE', language: ['c', 'cpp'])

if get_option('32bit')
  add_global_arguments('-m32', language: ['c', 'cpp'])
  add_global_link_arguments('-m32', language: ['c', 'cpp'])
endif

if cc.has_argument('-m32') and cpp.has_argument('-m32')
  have_m32 = declare_dependency(link_args: '-m32',
                                compile_args: '-m32')
else
  have_m32 = disabler()
endif

subdir('runtime')
subdir('src')
subdir('tests')
