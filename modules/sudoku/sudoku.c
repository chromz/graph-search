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

gboolean sudoku_compare(gconstpointer a, gconstpointer b)
{
	if (a == b) {
		return true;
	}
	const struct sudoku_board *ba = *((void **) a);
	const struct sudoku_board *bb = *((void **) b);
	if (ba->size != bb->size) {
		return false;
	}
	for (int i = 0; i < ba->size; ++i) {
		for (int j = 0; j < ba->size; ++j) {
			if (ba->grid[i][j] != bb->grid[i][j]) {
				return false;
			}
		}
	}
	return true;
}


static struct sudoku_board *sudoku_new_board(int size)
{
	struct sudoku_board *board = malloc(sizeof(struct sudoku_board));
	board->size = size;
	board->grid = malloc(board->size * sizeof(int *));
	board->freespcs = 0;
	board->diffnum = NULL;
	for (int i = 0; i < board->size; ++i) {
		board->grid[i] = malloc(board->size * sizeof(int));
	}
	return board;
}

static struct sudoku_board *sudoku_board_clone(struct sudoku_board *board)
{
	struct sudoku_board *new_board = malloc(sizeof(struct sudoku_board));
	new_board->size = board->size;
	new_board->grid = malloc(new_board->size * sizeof(int *));
	new_board->freespcs = board->freespcs;
	new_board->diffnum = NULL;
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
	printf("Checking sudoku: \n");
	sudoku_print_board(board);
	return board->freespcs == 0;
}

static bool check_grid_section(struct sudoku_board *board,
			       int num, int x, int y, unsigned *diffnum)
{
	if (diffnum != NULL) {
		for (int i = x; i < x + BOARD_MUL; ++i) {
			for (int j = y; j < y + BOARD_MUL; ++j) {
				int grid_val = board->grid[i][j];
				if (num == grid_val) {
					return true;
				}
				if (grid_val != 0) {
					diffnum[grid_val - 1] = 1;
				}
			}
		}
	} else {
		for (int i = x; i < x + BOARD_MUL; ++i) {
			for (int j = y; j < y + BOARD_MUL; ++j) {
				if (num == board->grid[i][j]) {
					return true;
				}
			}
		}
	}
	return false;
}


static bool is_in_grid_with_diff(struct sudoku_board *board, int num,
				  int row, int col, unsigned *diffnum)
{
	if (row < BOARD_MUL && col < BOARD_MUL) {
		return check_grid_section(board, num, 0, 0, diffnum);
	}

	if (row >= BOARD_MUL && col < BOARD_MUL) {
		return check_grid_section(board, num, BOARD_MUL, 0, diffnum);
	}
	if (row < BOARD_MUL && col >= BOARD_MUL) {
		return check_grid_section(board, num, 0, BOARD_MUL, diffnum);
	}

	if (row >= BOARD_MUL && col >= BOARD_MUL) {
		return check_grid_section(board, num, BOARD_MUL,
					  BOARD_MUL, diffnum);
	}

	return false;
}

static inline void sudoku_free_a_star_void(void *pboard)
{
	free_a_star_node((struct a_star_node *) pboard, sudoku_free_board_void);
}

inline void sudoku_free_board_void(void *pboard)
{
	sudoku_free_board((struct sudoku_board **)pboard);
}


void sudoku_free_board(struct sudoku_board **pboard)
{
	struct sudoku_board *board = *pboard;
	if (board != NULL) {
		for(unsigned i = 0; i < board->size; ++i) {
			free(board->grid[i]);
		}
		if (board->diffnum != NULL) {
			free(board->diffnum);
		}
		free(board->grid);
		free(board);
		*pboard = NULL;
	}
}

static void add_valid_states(GPtrArray *neighbors, struct sudoku_board *board,
			     int row, int col)
{
	for (int i = 1; i <= board->size; ++i) {
		bool is_good = true;
		unsigned *diffnum = calloc(board->size, sizeof(unsigned));
		for (int j = 0; j < board->size; ++j) {
			int rowval = board->grid[row][j];
			int colval = board->grid[j][col];
			if (rowval == i 
			    || colval == i) {
				is_good = false;
				break;
			}
			if (rowval != 0) {
				diffnum[rowval - 1] += 1;
			}
			if (colval != 0) {
				diffnum[colval - 1] += 1;
			}
		}
		if (is_good && !is_in_grid_with_diff(board, i, row,
						     col, diffnum)) {
			struct sudoku_board *new_board =
				sudoku_board_clone(board);
			new_board->freespcs--;
			new_board->grid[row][col] = i;
			new_board->diffnum = diffnum;
			struct a_star_node *new_node =
				malloc(sizeof(struct a_star_node));
			new_node->elm = new_board;
			g_ptr_array_add(neighbors, (gpointer) new_node);
		} else {
			free(diffnum);
		}

	}
}

GPtrArray *sudoku_expand(void *e)
{
	struct sudoku_board *board = e;
	GPtrArray* neighbors = g_ptr_array_new();
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
	struct sudoku_board *board1 = c;
	struct sudoku_board *board2 = n;
	return (board2->freespcs > board1->freespcs)
		? board2->freespcs - board1->freespcs
		: board1->freespcs - board2->freespcs;
}
int sudoku_heuristic(void *n)
{
	struct sudoku_board *board = n;
	if (board->diffnum == NULL) {
		return 0;
	}
	int score = 0;
	for (int i = 0; i < board->size; ++i) {
		if (board->diffnum[i] != 0) {
			score += board->diffnum[i];
		}
	}
	return score;
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
				sudoku_free_board(&board);
				return NULL;
			} else {

				board->grid[i][j] = e - '0';
			}
		}
	}
	return board;
}
