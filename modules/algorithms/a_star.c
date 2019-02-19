// Rodrigo Custodio

#include "a_star.h"


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

void free_a_star_node(struct a_star_node *n, void (*free_elm)(void **))
{
	free_elm(&n->elm);
	free(n);
}

struct a_star_node *a_star_solve(void *start, bool (*goaltest)(void *),
				 GPtrArray *(*expand)(void *),
				 int (*path_cost)(void *, void *),
				 int (*heuristic)(void *),
				 void (*free_elm)(void **))
{
	pqueue_t *frontier;
	struct a_star_node *visited = NULL;
	struct a_star_node *start_node = malloc(sizeof(struct a_star_node));
	if (start_node == NULL) {
		return NULL;
	}
	start_node->elm = start;
	start_node->prev = NULL;
	start_node->cost = 0;
	start_node->pri = 0;

	frontier = pqueue_init(1, cmp_pri, get_pri, set_pri, get_pos, set_pos);
	if (!frontier) {
		free(start_node);
		return NULL;
	}
	pqueue_insert(frontier, start_node);
	HASH_ADD_PTR(visited, elm, start_node);
	while (pqueue_size(frontier)) {
		struct a_star_node *cnode = pqueue_pop(frontier);
		if ((*goaltest)(cnode->elm)) {
			// FREE ALL
			printf("Found solution\n");
			return cnode;
		}
		GPtrArray *neighbors = (*expand)(cnode->elm);
		for (int i = 0; i < neighbors->len; ++i) {
			struct a_star_node *next = 
				g_ptr_array_index(neighbors, i);
			int cost = (*path_cost)(cnode, next) +
				   cnode->cost;
			struct a_star_node *in;
			HASH_FIND_PTR(visited, next, in);
			if (!in || cost < next->cost) {
				next->cost = cost;
				next->pri = cost + (*heuristic)(next->elm);
				pqueue_insert(frontier, next);
			} else if (!in) {
				free_a_star_node(next, free_elm);
			}
		}
		g_ptr_array_free(neighbors, false);
		free_a_star_node(cnode, free_elm);
	}
	struct a_star_node *result = malloc(sizeof(struct a_star_node));
	result->elm = NULL;
	return result;
}
