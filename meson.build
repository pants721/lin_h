project('lin_h', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

# incdir = include_directories('./lin.h')

exe = executable('lin_h', 'src/main.c',
  link_args : '-lm',
  install : true)

test('basic', exe)
