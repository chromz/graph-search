
#include "fifteen.h"
#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define FIFTEEN_SIZE 4

void fifteen_print_board(struct fifteen_board *board)
{
	for (int i = 0; i < board->size; ++i) {
		if (i % FIFTEEN_SIZE == 0) {
			printf("|");
		}
		if (board->grid[i] == 0) {
			printf(" x ");
		} else if (board->grid[i] < 10){
			printf(" %d ", board->grid[i]);
		} else {
			printf(" %d", board->grid[i]);
		}
		if ((i + 1) % FIFTEEN_SIZE == 0) {
			printf(" |\n");
		}
	}

}

static void fifteen_print_board_ncurses(struct fifteen_board *board)
{
	clear();
	printw("Board:\n");
	for (int i = 0; i < board->size; ++i) {
		int y = i / 4 + 1;
		int x = i % 4 * 5 + 3;
		if (i % FIFTEEN_SIZE == 0) {
			mvprintw(y, 0, "|");
		}
		if (board->grid[i] == 0) {
			mvprintw(y, x - 1, "x");
		} else {
			mvprintw(y, x - 1, "%d", board->grid[i]);
		}
		if ((i + 1) % FIFTEEN_SIZE == 0) {
			mvprintw(y, x + 1, " |");
		}
	}
	refresh();
}

inline void fifteen_free_board_void(void *pboard)
{
	fifteen_free_board((struct fifteen_board **)pboard);
}

void fifteen_free_board(struct fifteen_board **pboard)
{
	struct fifteen_board *board = *pboard;
	if (board != NULL) {
		free(board->grid);
		free(board);
		*pboard = NULL;
	}
}

static struct fifteen_board *fifteen_new_board(size_t size)
{
	struct fifteen_board * board = malloc(sizeof(struct fifteen_board));
	board->size = size * size;
	board->blanc = -1;
	board->grid = malloc(board->size * sizeof(unsigned));
	return board;
}

static int hex_to_int(char c)
{
	if (isdigit(c)) {
		return c - '0';
	} 
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'A' + 10;
	}
	return -1;
}

bool fifteen_is_valid(struct fifteen_board *board)
{
	if (board == NULL) {
		return false;
	}
	unsigned inversions = 0;
	for (int i = 0; i < board->size; ++i) {
		for (int j = i; j < board->size; ++j) {
			if (board->grid[i] != 0 && board->grid[j] != 0
			    && board->grid[i] > board->grid[j]) {
				inversions++;
			}
		}
	}
	if (inversions == 0) {
		return true;
	}
	int pos = 4 - (board->blanc / 4);
	if (pos % 2 == 0) {
		return inversions % 2 == 1;
	}
	return inversions % 2 == 0;
}

struct fifteen_board *fifteen_read(char *in)
{
	size_t len = strlen(in);
	if (len != FIFTEEN_SIZE * FIFTEEN_SIZE) {
		return NULL;
	}
	struct fifteen_board *board = fifteen_new_board(FIFTEEN_SIZE);
	for (int i = 0; i < board->size; ++i) {
		char e = *in++;
		if (e == '.') {
			if (board->blanc != -1) {
				fifteen_free_board(&board);
				return NULL;
			}
			board->grid[i] = 0;
			board->blanc = i;
		} else {
			int num = hex_to_int(e);
			if (num == -1) {
				fifteen_free_board(&board);
				return NULL;
			}
			board->grid[i] = (unsigned) num;
		}
		
	}
	return board;
}


bool fifteen_goaltest(void *e)
{
	struct fifteen_board *board = e;
	fifteen_print_board_ncurses(board);
	for (int i = 1; i <= board->size; ++i) {
		if (board->grid[i - 1] != i % board->size) {
			return false;
		}
	}
	return true;
}

static struct fifteen_board *clone_and_change(struct fifteen_board *board,
					      int new_blanc_pos)
{
	struct fifteen_board *new_board = malloc(sizeof(struct fifteen_board));
	new_board->size = board->size;
	new_board->blanc = new_blanc_pos;
	new_board->grid = malloc(new_board->size * sizeof(unsigned));
	memcpy(new_board->grid, board->grid,
	       new_board->size * sizeof(unsigned));
	new_board->grid[new_blanc_pos] = 0;
	new_board->grid[board->blanc] = board->grid[new_blanc_pos];
	return new_board;
}

GPtrArray *fifteen_expand(void *e)
{
	struct fifteen_board *board = e;
	GPtrArray *neighbors = g_ptr_array_new();
	int top = board->blanc - 4;
	int bottom = board->blanc + 4;
	int left = board->blanc - 1;
	int right = board->blanc + 1;
	if (top >= 0) {
		// GEN TOP State
		struct fifteen_board *tboard = clone_and_change(board, top);
		struct a_star_node *new_node = 
			malloc(sizeof(struct a_star_node));
		new_node->elm = tboard;
		g_ptr_array_add(neighbors, new_node);
	}
	if (bottom < board->size) {
		// GEN BOTTOM State
		struct fifteen_board *bboard = clone_and_change(board, bottom);
		struct a_star_node *new_node = 
			malloc(sizeof(struct a_star_node));
		new_node->elm = bboard;
		g_ptr_array_add(neighbors, new_node);
	}
	if (left >= 0 && left / 4 == board->blanc / 4) {
		// GEN LEFT State
		struct fifteen_board *lboard = clone_and_change(board, left);
		struct a_star_node *new_node = 
			malloc(sizeof(struct a_star_node));
		new_node->elm = lboard;
		g_ptr_array_add(neighbors, new_node);
	}
	if (right < board->size && right / 4 == board->blanc / 4) {
		// GEN Right state
		struct fifteen_board *rboard = clone_and_change(board, right);
		struct a_star_node *new_node = 
			malloc(sizeof(struct a_star_node));
		new_node->elm = rboard;
		g_ptr_array_add(neighbors, new_node);
	}
	return neighbors;
}

int fifteen_path_cost(void *c, void *n)
{
	return 1;
}

static inline int manhattan_distance(int pos, int dest)
{
	int xpos = pos / 4;
	int ypos = pos % 4;
	int xdest = dest / 4;
	int ydest = dest % 4;
	return abs(xdest - xpos) + abs(ydest - ypos);
}

int fifteen_heuristic(void *n)
{
	struct fifteen_board *board = n;
	int smhd = 0;
	for (int i = 0; i < board->size; ++i) {
		int num = board->grid[i];
		if (num == 0) {
			num = 16;
		}

		smhd += manhattan_distance(i, num - 1);
		// Linear conflicts
		int start = i / 4;
		for (int j = start * 4; j < i; ++j) {
			int num2 = board->grid[j];
			int num_goal_line = (num - 1) / 4;
			int num2_goal_line = (num2 - 1) / 4;
			if (start == num_goal_line 
			    && num_goal_line == num2_goal_line
			    && num  < num2) {
				smhd += 1;
			}
		}
	}
	/* fifteen_print_board_ncurses(board); */
	return smhd;
}

gboolean fifteen_compare(gconstpointer a, gconstpointer b)
{
	if (a == b) {
		return true;
	}
	const struct fifteen_board *ba = *((void **) a);
	const struct fifteen_board *bb = *((void **) b);
	if (ba->size != bb ->size) {
		return false;
	}
	if (ba->blanc != bb->blanc) {
		return false;
	}
	for (int i = 0; i < ba->size; ++i) {
		if (ba->grid[i] != bb->grid[i]) {
			return false;
		}
	}
	return true;
}




