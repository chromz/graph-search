// Rodrigo Custodio

#include "a_star.h"
#include <gmodule.h>

static void (*free_element_fun)(void *);

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

void inline free_a_star_node(struct a_star_node *n, void (*free_elm)(void *))
{
	free_elm(&n->elm);
	free(n);
}

static inline void free_all(pqueue_t *frontier, GHashTable *costs)
{
	g_hash_table_destroy(costs);
	/* struct a_star_node *node; */
	/* while (( node = pqueue_pop(frontier))) { */
	/* 	free_a_star_node(node, free_elm); */
	/* } */
	pqueue_free(frontier);
}

static inline void free_a_star_node_void(void *pnode)
{
	struct a_star_node *node = pnode;
	free_a_star_node(node, free_element_fun);
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
	free_element_fun = free_elm;
	GHashTable *costs = g_hash_table_new_full(NULL, NULL,
						  free_a_star_node_void, NULL);
	GPtrArray *visited = g_ptr_array_new();
	struct a_star_node *start_node = malloc(sizeof(struct a_star_node));
	if (start_node == NULL) {
		return NULL;
	}
	start_node->elm = start;
	start_node->pri = 0;

	frontier = pqueue_init(1, cmp_pri, get_pri, set_pri, get_pos, set_pos);
	if (!frontier) {
		free(start_node);
		return NULL;
	}
	pqueue_insert(frontier, start_node);
	g_hash_table_insert(costs, start_node, GINT_TO_POINTER(0));
	while (pqueue_size(frontier)) {
		struct a_star_node *cnode = pqueue_pop(frontier);
		if ((*goaltest)(cnode->elm)) {
			// FREE ALL
			printf("Found solution\n");
			g_hash_table_steal(costs, cnode);
			g_hash_table_steal(costs, start_node);
			free(start_node);
			g_ptr_array_free(visited, true);
			free_all(frontier, costs);
			return cnode;
		}
		GPtrArray *neighbors = (*expand)(cnode->elm);
		for (int i = 0; i < neighbors->len; ++i) {
			struct a_star_node *next = 
				g_ptr_array_index(neighbors, i);
			int ccost = GPOINTER_TO_INT(
					g_hash_table_lookup(costs, cnode));
			int cost = (*path_cost)(cnode->elm, next->elm) + ccost;
			gboolean in = g_ptr_array_find_with_equal_func(
					visited, next, compare, NULL);
			if (!in || cost 
			    < GPOINTER_TO_INT(g_hash_table_lookup(
					      costs, cnode))) {
				g_hash_table_insert(costs, next,
						    GINT_TO_POINTER(cost));
				g_ptr_array_add(visited, next);
				next->pri = cost + (*heuristic)(next->elm);
				pqueue_insert(frontier, next);
			}
		}
		g_ptr_array_free(neighbors, true);
	}
	struct a_star_node *result = malloc(sizeof(struct a_star_node));
	result->elm = NULL;
	return result;
}
