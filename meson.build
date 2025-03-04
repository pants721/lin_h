project('lin_h', 'c',
  version : '0.1',
  default_options : ['warning_level=everything'])

unity_subproject = subproject('unity')
unity_dep = unity_subproject.get_variable('unity_dep')

inc = include_directories('./')

test_mat = executable('test_mat',
  sources : ['test/mat.c'],
  include_directories : [inc],
  dependencies : [unity_dep],
  link_args : '-lm',
  install : false)

test_vec = executable('test_vec',
  sources : ['test/vec.c'],
  include_directories : [inc],
  dependencies : [unity_dep],
  link_args : '-lm',
  install : false)

test('test_mat', test_mat)
test('test_vec', test_vec)

exe = executable('lin_h', 'src/main.c',
  link_args : '-lm',
  install : true)
