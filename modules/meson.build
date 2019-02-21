
ncursesdep = dependency('ncurses', required: true)

algs_sources = [
  'algorithms/a_star.c',
]

algs_lib = static_library('algorithms',
  sources: algs_sources,
  include_directories : libs_include_dirs,
  dependencies: [pqueue_dep, glib_dep, ncursesdep])

sudoku_sources = [
  'sudoku/sudoku.c',
]

sudoku_lib = static_library('sudoku', sudoku_sources,
  include_directories : libs_include_dirs,
  link_with : [algs_lib],
  dependencies : [glib_dep, ncursesdep])

fifteen_sources = [
  'fifteen/fifteen.c',
]

fifteen_lib = static_library('fifteen', fifteen_sources,
  include_directories : libs_include_dirs,
  link_with : [algs_lib],
  dependencies : [glib_dep])
