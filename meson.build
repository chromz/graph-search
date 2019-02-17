
project('graph-search', 'c')

c = meson.get_compiler('c')
add_project_arguments('-Wpedantic', language : 'c')

graph_search_include_dirs = include_directories('modules')

subdir('modules')

cli_sources = [
    'cli/cli.c'
]


executable('cli', 
           sources: cli_sources,
           include_directories : graph_search_include_dirs,
           link_with: [sudoku_lib, algs_lib])

