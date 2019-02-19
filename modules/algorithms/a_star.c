// Rodrigo Custodio

#include "a_star.h"
#include <gmodule.h>


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

void free_a_star_node(struct a_star_node *n, void (*free_elm)(void *))
{
	free_elm(&n->elm);
	free(n);
}

static inline void free_all(pqueue_t *frontier, GHashTable *visited,
			    void (*free_elm)(void *))
{
	g_hash_table_destroy(visited);
	struct a_star_node *node;
	while (( node = pqueue_pop(frontier))) {
		free_a_star_node(node, free_elm);
	}
	pqueue_free(frontier);
}

struct a_star_node *a_star_solve(void *start, bool (*goaltest)(void *),
				 GPtrArray *(*expand)(void *),
				 gboolean (*compare)(gconstpointer,
						     gconstpointer),
				 int (*path_cost)(void *, void *),
				 int (*heuristic)(void *),
				 void (*free_elm)(void *))
{
	pqueue_t *frontier;
	GHashTable *visited = g_hash_table_new_full(NULL, compare,
						    free_elm, NULL);
	struct a_star_node *start_node = malloc(sizeof(struct a_star_node));
	if (start_node == NULL) {
		return NULL;
	}
	start_node->elm = start;
	start_node->prev = NULL;
	start_node->pri = 0;

	frontier = pqueue_init(1, cmp_pri, get_pri, set_pri, get_pos, set_pos);
	if (!frontier) {
		free(start_node);
		return NULL;
	}
	pqueue_insert(frontier, start_node);
	g_hash_table_insert(visited, start_node, GINT_TO_POINTER(0));
	while (pqueue_size(frontier)) {
		struct a_star_node *cnode = pqueue_pop(frontier);
		if ((*goaltest)(cnode->elm)) {
			// FREE ALL
			printf("Found solution\n");
			free_all(frontier, visited, free_elm);
			return cnode;
		}
		GPtrArray *neighbors = (*expand)(cnode->elm);
		for (int i = 0; i < neighbors->len; ++i) {
			struct a_star_node *next = 
				g_ptr_array_index(neighbors, i);
			int ccost = GPOINTER_TO_INT(
					g_hash_table_lookup(visited, cnode));
			int cost = (*path_cost)(cnode, next) + ccost;
			gboolean in = g_hash_table_lookup_extended(
					visited, next, NULL, NULL);
			if (!in || cost 
			    < GPOINTER_TO_INT(g_hash_table_lookup(
					      visited, cnode))) {
				g_hash_table_insert(visited, next,
						    GINT_TO_POINTER(cost));
				next->pri = cost + (*heuristic)(next->elm);
				pqueue_insert(frontier, next);
			}
		}
		/* g_ptr_array_free(neighbors, false); */
		/* free_a_star_node(cnode, free_elm); */
	}
	struct a_star_node *result = malloc(sizeof(struct a_star_node));
	result->elm = NULL;
	return result;
}
