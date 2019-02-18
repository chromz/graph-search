// Rodrigo Custodio

#ifndef SUDOKU_LIB_HEADER
#define SUDOKU_LIB_HEADER

#include "algorithms/a_star.h"
#include <gmodule.h>
#include <stdbool.h>

struct sudoku_board {
	int **grid;
	unsigned size;
	unsigned freespcs;
};

struct sudoku_board *sudoku_read(char *in);

bool sudoku_goaltest(void *e);
GArray *sudoku_expand(void *e);
int sudoku_path_cost(void *c, void *n);
int sudoku_heuristic(void *n);

void sudoku_delete_board(struct sudoku_board **board);
void sudoku_print_board(struct sudoku_board *board);

#endif
