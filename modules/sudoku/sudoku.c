// Rodrigo Custodio

#include "sudoku.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 4


static int get_number(char e)
{
	if (e == '.') {
		return 0;
	}
	if (!isdigit(e)) {
		return -1;
	}
	return e - '0';
}


void sudoku_delete_board(struct sudoku_board **pboard)
{
	struct sudoku_board *board = *pboard;
	if (board != NULL) {
		for(unsigned i = 0; i < board->size; ++i) {
			free(board->grid[i]);
		}
		free(board->grid);
		free(board);
		*pboard = NULL;
	}
}

void sudoku_print_board(struct sudoku_board *board)
{
	for (int i = 0; i < board->size; ++i) {
		printf("|");
		for (int j = 0; j < board->size; ++j) {
			printf(" %c", board->grid[i][j] + '0');
		}
		printf(" |\n");
	}
}

struct sudoku_board *sudoku_read(char *in)
{
	size_t len = strlen(in);
	if (len != BOARD_SIZE * BOARD_SIZE) {
		return NULL;
	}
	struct sudoku_board *board = malloc(sizeof(struct sudoku_board));
	board->size = BOARD_SIZE;
	board->grid = malloc(board->size * sizeof(int *));
	for (int i = 0; i < board->size; ++i) {
		board->grid[i] = malloc(board->size * sizeof(int));
	}
	for (int i = 0; i < board->size; ++i) {
		for (int j = 0; j <  board->size; ++j) {
			int num = get_number(*in++);
			if (num == -1) {
				sudoku_delete_board(&board);
				return NULL;
			}
			board->grid[i][j] = num;
		}
	}
	return board;
}
