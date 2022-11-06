


project('zzzLib', 'cpp',
  version : '1.0',
  default_options : ['warning_level=3',
                     'cpp_std=c++20'])


incdir = include_directories('include', 'dependencies/MetaStuff/include')

src_files = ['source/parser.cpp', 'source/zzz_tree.cpp', 'source/tokenizer.cpp',
  'source/zzz_exception.cpp']

subdir('include')
subdir('dependencies')
subdir('source')

# library('zzzlib', src_files, include_directories : incdir)




exe = executable('zzzlib', src_files,
  install : true, include_directories : incdir)

serialTest = executable('serialTest', 'test_files/serialTest.cpp',
              include_directories : incdir, link_with : zzzlib)
test('serial', serialTest)


pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : zzzlib,
                 version : '1.0.0',
                 name : 'zzzlib',
                 filebase : 'zzzlib',
                 description : 'A library to read in zzz formatted files.')
