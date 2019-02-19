// Rodrigo Custodio

#include "sudoku.h"
#include <ctype.h>
#include <gmodule.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 4
#define BOARD_MUL 2

static struct sudoku_board *sudoku_new_board(int size)
{
	struct sudoku_board *board = malloc(sizeof(struct sudoku_board));
	board->size = size;
	board->grid = malloc(board->size * sizeof(int *));
	board->freespcs = 0;
	for (int i = 0; i < board->size; ++i) {
		board->grid[i] = malloc(board->size * sizeof(int));
	}
	return board;
}

static struct sudoku_board *sudoku_board_clone(struct sudoku_board *board)
{
	struct sudoku_board *new_board= malloc(sizeof(struct sudoku_board));
	new_board->size = board->size;
	new_board->grid = malloc(new_board->size * sizeof(int *));
	new_board->freespcs = board->freespcs;
	for (int i = 0; i < new_board->size; ++i) {
		new_board->grid[i] = malloc(new_board->size * sizeof(int));
		memcpy(new_board->grid[i], board->grid[i],
		       new_board->size * sizeof(int));
	}
	return new_board;
}

bool sudoku_goaltest(void *e)
{
	struct sudoku_board *board = e;
	int row[BOARD_SIZE] = {0};
	int column[BOARD_SIZE] = {0};
	int grid[BOARD_SIZE] = {0};
	bool check_row[BOARD_SIZE + 1] = {false};
	bool check_column[BOARD_SIZE + 1] = {false};
	bool check_grid[BOARD_SIZE + 1] = {false};
	for (int i = 0; i < BOARD_SIZE; ++i) {
		memcpy(&row, board->grid[i], sizeof(row));
		for (int j = 0; j < BOARD_SIZE; ++j) {
			column[j] = board->grid[i][j];
			int index_row = (i * BOARD_MUL) 
				        % BOARD_SIZE + j % BOARD_MUL;
			int index_col = (i / BOARD_MUL) * BOARD_MUL 
				        + j / BOARD_MUL;
			grid[j] = board->grid[index_row][index_col];
		}
		for (int k = 0; k < BOARD_SIZE; ++k) {
			if (check_row[row[k]]
			    || check_column[column[k]]
			    || check_grid[grid[k]]) {
				return false;
			}
			check_row[row[k]] = true;
			check_column[column[k]] = true;
			check_grid[grid[k]] = true;
		}
		if (check_row[0] || check_column[0]
		    || check_grid[0]) {
			return false;
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

static bool check_grid_section(struct sudoku_board *board,
			       int num, int x, int y)
{
	for (int i = x; i < x + BOARD_MUL; ++i) {
		for (int j = y; j < y + BOARD_MUL; ++j) {
			if (num == board->grid[i][j]) {
				return true;
			}
		}
	}
	return false;
}


static bool is_in_grid(struct sudoku_board *board, int num, int row, int col)
{
	if (row < BOARD_MUL && col < BOARD_MUL) {
		return check_grid_section(board, num, 0, 0);
	}

	if (row >= BOARD_MUL && col < BOARD_MUL) {
		return check_grid_section(board, num, BOARD_MUL, 0);
	}
	if (row < BOARD_MUL && col >= BOARD_MUL) {
		return check_grid_section(board, num, 0, BOARD_MUL);
	}

	if (row >= BOARD_MUL && col >= BOARD_MUL) {
		return check_grid_section(board, num, BOARD_MUL, BOARD_MUL);
	}

	return false;
}

static void add_valid_states(GArray *neighbors, struct sudoku_board *board,
			     int row, int col)
{
	for (int i = 1; i <= board->size; ++i) {
		bool is_good = true;
		for (int j = 0; j < board->size; ++j) {
			if (board->grid[row][j] == i || board->grid[j][col] == i
			    || is_in_grid(board, i, row, col)) {
				is_good = false;
				break;
			}
		}
		if (is_good) {
			struct sudoku_board *new_board =
				sudoku_board_clone(board);
			new_board->grid[row][col] = i;
			new_board->freespcs--;
			struct a_star_node *new_node =
				malloc(sizeof(struct a_star_node));
			new_node->elm = new_board;
			g_array_append_val(neighbors, new_node);
		}
	}
}

GArray *sudoku_expand(void *e)
{
	struct sudoku_board *board = e;
	GArray *neighbors = g_array_new(false, false,
		                        sizeof(struct a_star_node *));
	for (int i = 0; i < board->size; ++i) {
		for (int j = 0; j < board->size; ++j) {
			if (board->grid[i][j] == 0) {
				add_valid_states(neighbors, board, i, j);
			}
		}
	}
	return neighbors;

}


int sudoku_path_cost(void *c, void *n)
{
	struct sudoku_board *board1 = (struct sudoku_board *) c;
	struct sudoku_board *board2 = (struct sudoku_board *) n;
	return (board2->freespcs > board1->freespcs)
		? board2->freespcs - board1->freespcs
		: board1->freespcs - board2->freespcs;
}
int sudoku_heuristic(void *n)
{
	return 0;
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
	struct sudoku_board *board = sudoku_new_board(BOARD_SIZE);
	for (int i = 0; i < board->size; ++i) {
		for (int j = 0; j <  board->size; ++j) {
			char e = *in++;
			if (e == '.') {
				board->grid[i][j] = 0;
				board->freespcs++;
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
