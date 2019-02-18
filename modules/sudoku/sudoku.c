// Rodrigo Custodio

#include "sudoku.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 4

bool sudoku_goaltest(void *e)
{
	struct sudoku_board *board = e;
	int row[BOARD_SIZE] = {0};
	int column[BOARD_SIZE] = {0};
	int grid[BOARD_SIZE] = {0};
	bool check_row[BOARD_SIZE] = {false};
	bool check_column[BOARD_SIZE] = {false};
	bool check_grid[BOARD_SIZE] = {false};
	for (int i = 0; i < BOARD_SIZE; ++i) {
		memcpy(&row, board->grid[i], sizeof(row));
		for (int j = 0; j < BOARD_SIZE; ++j) {
			column[j] = board->grid[i][j];
			int index_row = (i * 2) % BOARD_SIZE + j % 2;
			int index_col = (i / 2) * 2 + j / 2;
			grid[j] = board->grid[index_row][index_col];
		}
		for (int k = 0; k < BOARD_SIZE; ++k) {
			if (check_row[row[k] - 1] ||
			    check_column[column[k] -1 ] ||
			    check_grid[grid[k] - 1]) {
				return false;
			}
			check_row[row[k] - 1] = true;
			check_column[column[k] - 1] = true;
			check_grid[grid[k] - 1] = true;
		}
		memset(row, 0, sizeof(row));
		memset(column, 0, sizeof(column));
		memset(grid, 0, sizeof(grid));
		memset(check_row, 0, sizeof(check_row));
		memset(check_column, 0, sizeof(check_row));
		memset(check_grid, 0, sizeof(check_row));
	}
	return true;
}

struct a_star_node *soduku_expand(void *e)
{

}

int sudoku_path_cost(void *c, void *n)
{

}
int sudoku_heuristic(void *n)
{

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
			char e = *in++;
			if (e == '.') {
				board->grid[i][j] = 0;
			} else if (!isdigit(e) && e > 0 && e <= board->size) {
				sudoku_delete_board(&board);
				return NULL;
			} else {

				board->grid[i][j] = e - '0';
			}
		}
	}
	return board;
}
