
#include "fifteen.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define FIFTEEN_SIZE 4

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
	board->diff = 0;
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
	if ((((board->blanc + 1) % 4) - 4) % 2 == 0) {
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
	return true;
}

GPtrArray *fifteen_expand(void *e)
{
}

int fifteen_path_cost(void *c, void *n)
{
}

int fifteen_heuristic(void *n)
{
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
	for (int i = 0; i < ba->size; ++i) {
		if (ba->grid[i] != bb->grid[i]) {
			return false;
		}
	}
	return true;
}

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

