// Rodrigo Custodio

#ifndef SUDOKU_LIB_HEADER
#define SUDOKU_LIB_HEADER

struct sudoku_board {
	int **grid;
	unsigned size;
};

struct sudoku_board *sudoku_read(char *in);
void sudoku_delete_board(struct sudoku_board **board);
void sudoku_print_board(struct sudoku_board *board);

#endif
