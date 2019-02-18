
#ifndef A_STAR_LIB
#define A_STAR_LIB

#include "pqueue/pqueue.h"
#include <gmodule.h>
#include <uthash.h>
#include <stdbool.h>

struct a_star_node {
	void *elm;
	void *prev;
	int cost;
	pqueue_pri_t pri;
	size_t pos;
	UT_hash_handle hh;
};

int a_star_solve(void *start, bool (*goaltest)(void *),
	GArray *(*expand)(void *),
	int (*path_cost)(void *c, void *n),
	int (*heuristic)(void *n));



#endif
