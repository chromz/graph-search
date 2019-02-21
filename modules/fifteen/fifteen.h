// Rodrigo Custodio

#ifndef FIFTEEN_LIB_HEADER
#define FIFTEEN_LIB_HEADER

#include "algorithms/a_star.h"
#include <glib.h>
#include <stdbool.h>

struct fifteen_board {
	unsigned *grid;
	unsigned size;
	int blanc;
};

struct fifteen_board *fifteen_read(char *in);

bool fifteen_is_valid(struct fifteen_board *board);
bool fifteen_goaltest(void *e);
GPtrArray *fifteen_expand(void *e);
int fifteen_path_cost(void *c, void *n);
int fifteen_heuristic(void *n);

gboolean fifteen_compare(gconstpointer a, gconstpointer b);

void fifteen_free_board_void(void *pboard);
void fifteen_free_board(struct fifteen_board **pboard);
void fifteen_print_board(struct fifteen_board *board);

#endif
