
project('graph-search', 'c')

c = meson.get_compiler('c')
add_project_arguments('-Wpedantic', language : 'c')

graph_search_include_dirs = include_directories('modules')
libs_include_dirs = include_directories('libs')
glib_dep = dependency('glib-2.0',
  method : 'pkg-config',
  version : '>=2.58',
  required : true)

subdir('libs')
subdir('modules')

cli_sources = [
    'cli/cli.c'
]


executable('cli', 
           sources: cli_sources,
           include_directories : [graph_search_include_dirs, libs_include_dirs],
           dependencies : [glib_dep],
           link_with : [sudoku_lib, algs_lib])

