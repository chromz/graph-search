// Rodrigo Custodio

#ifndef SUDOKU_LIB_HEADER
#define SUDOKU_LIB_HEADER

#include "algorithms/a_star.h"
#include <glib.h>
#include <stdbool.h>

struct sudoku_board {
	int **grid;
	unsigned size;
	unsigned freespcs;
	unsigned *diffnum;
};

struct sudoku_board *sudoku_read(char *in);

bool sudoku_goaltest(void *e);
GPtrArray *sudoku_expand(void *e);
int sudoku_path_cost(void *c, void *n);
int sudoku_heuristic(void *n);

void sudoku_free_board_void(void **pboard);
void sudoku_free_board(struct sudoku_board **pboard);
void sudoku_print_board(struct sudoku_board *board);

#endif
