// Rodrigo Custodio

#include "pqueue/pqueue.h"
#include "a_star.h"
#include <stdbool.h>


static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr)
{
	return next < curr;
}

static pqueue_pri_t get_pri(void *e)
{
	return ((struct a_star_node *) e)->pri;
}

static void set_pri(void *e, pqueue_pri_t p)
{
	((struct a_star_node *) e)->pri = p;
}

static size_t get_pos(void *e)
{
	return ((struct a_star_node *) e)->pos;
}

static void set_pos(void *e, size_t pos)
{
	((struct a_star_node *) e)->pos = pos;
}



int a_star(void *start, bool (*goaltest)(void *),
	struct a_star_node *(*expand)(void *))
{
	pqueue_t *frontier;
	struct a_star_node *visited = NULL;
	struct a_star_node *costs = NULL;
	struct a_star_node *start_node = malloc(sizeof(struct a_star_node));
	if (start_node == NULL) {
		return -1;
	}
	start_node->elm = start;
	start_node->prev = NULL;
	start_node->cost = 0;
	start_node->pri = 0;

	frontier = pqueue_init(1, cmp_pri, get_pri, set_pri, get_pos, set_pos);
	if (!frontier) {
		free(start_node);
		return -1;
	}
	pqueue_insert(frontier, start_node);
	HASH_ADD_PTR(visited, elm, start_node);
	HASH_ADD_PTR(costs, elm, start_node);

	while (pqueue_size(frontier)) {
		struct a_star_node *cnode = pqueue_pop(frontier);
		if ((*goaltest)(cnode->elm)) {
			return 1;
		}
		struct a_star_node *neighbors = (*expand)(cnode->elm);	
		while ((neighbors++) != NULL) {

		}
	}
	return 0;
}
