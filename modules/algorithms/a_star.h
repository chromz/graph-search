
#ifndef A_STAR_LIB
#define A_STAR_LIB

#include <uthash.h>

struct a_star_node {
	void *elm;
	void *prev;
	int cost;
	pqueue_pri_t pri;
	size_t pos;
	UT_hash_handle hh;
};



#endif
