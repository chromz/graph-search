sudoku_sources = [
  'sudoku/sudoku.c',
]

sudoku_lib = static_library('sudoku', sudoku_sources)

algs_sources = [
  'algorithms/a_star.c',
]

algs_lib = static_library('algorithms', algs_sources,
  include_directories: libs_include_dirs,
  dependencies: pqueue_dep)
