#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h> /* size_t */


#ifndef __WEIGHT_T
#define __WEIGHT_T double
#endif

#ifndef __DATA_T
#define __DATA_T long
#endif

struct edge {
	__WEIGHT_T weight;
	struct vertex *to;
	struct edge *next;
};

struct vertex {
	__DATA_T data;
	bool is_set;
	struct edge *list;
};

struct graph {
	struct vertex *vertices;
	size_t size;
	int (*data_cmp)(const __DATA_T left, const __DATA_T right);
	int (*weight_cmp)(const __WEIGHT_T left, const __WEIGHT_T right);
	uintptr_t (*hash)(const __DATA_T data);
};

int graph_init(struct graph *g, size_t alloc, int (*data_cmp)(const __DATA_T left, const __DATA_T right), int (*weight_cmp)(const __WEIGHT_T left, const __WEIGHT_T right), uintptr_t (*hash)(const __DATA_T data));
int graph_delete(struct graph *g);

int graph_get_vertex(struct graph *g, const __DATA_T data, struct vertex **v_ret);
int graph_get_edge(struct vertex *from, struct vertex *to, struct edge **e_ret);

struct vertex *graph_add_vertex(struct graph *g, const __DATA_T data);
struct edge *graph_add_edge(struct graph *g, struct vertex *from, struct vertex *to, const __WEIGHT_T weight);

int graph_remove_vertex(struct vertex *v);
int graph_remove_edge(struct edge *e);

#endif
