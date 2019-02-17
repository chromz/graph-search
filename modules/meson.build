sudoku_sources = [
  'sudoku/sudoku.c',
]

sudoku_lib = static_library('sudoku', sudoku_sources)

algs_sources = [
  'algorithms/a_star.c',
]

has_uthash = c.has_header('uthash.h')
assert(has_uthash, 'uthash is required, please see' +
  'https://troydhanson.github.io/uthash/')

algs_lib = static_library('algorithms', algs_sources,
  include_directories: libs_include_dirs,
  dependencies: pqueue_dep)
