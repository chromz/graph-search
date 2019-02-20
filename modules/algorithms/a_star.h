
#ifndef A_STAR_LIB
#define A_STAR_LIB

#include "pqueue/pqueue.h"
#include <gmodule.h>
#include <stdbool.h>

struct a_star_node {
	void *elm;
	pqueue_pri_t pri;
	size_t pos;
};

void free_a_star_node(struct a_star_node *n, void (*free_elm)(void *));

struct a_star_node *a_star_solve(void *start, bool (*goaltest)(void *),
				 GPtrArray *(*expand)(void *),
				 gboolean (*compare)(gconstpointer,
						     gconstpointer),
				 int (*path_cost)(void *, void *),
				 int (*heuristic)(void *),
				 void (*free_elm)(void *));



#endif
