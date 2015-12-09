#ifdef _HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int graph_init(struct graph *g, size_t alloc, int (*data_cmp)(const __DATA_T left, const __DATA_T right), int (*weight_cmp)(const __WEIGHT_T left, const __WEIGHT_T right), uintptr_t (*hash)(const __DATA_T data))
{
	g->vertices = calloc(alloc, sizeof *g->vertices);
	if (!g->vertices) {
		perror("calloc");
		return -1;
	}
	g->size = alloc;
	g->data_cmp = data_cmp;
	g->weight_cmp = weight_cmp;
	g->hash = hash;
	return 0;
}

int graph_delete(struct graph *g)
{
	uintptr_t i;
	for (i = 0; i < g->size; i++) {
		if (!g->vertices[i].is_set) {
			continue;
		}
		graph_remove_vertex(g->vertices + i);
	}
	free(g->vertices);
	return 0;
}


int graph_get_vertex(struct graph *g, const __DATA_T data, struct vertex **v_ret)
{
	uintptr_t i;
	for (i = g->hash(data) % g->size; i < g->size; i++) {
		if (!g->vertices[i].is_set) {
			break;
		}
		if (!g->data_cmp(g->vertices[i].data, data)) {
			*v_ret = g->vertices + i;
			return 0;
		}
	}
	return -1;
}
int graph_get_edge(struct vertex *from, struct vertex *to, struct edge **e_ret)
{
	struct edge *e;
	for (e = from->list; e != NULL; e = e->next) {
		if (e->to == to) {
			*e_ret = e;
			return 0;
		}
	}
	return -1;
}

int graph_add_vertex(struct graph *g, const __DATA_T data, struct vertex **v_ret)
{
	uintptr_t i;
	for (i = g->hash(data) % g->size; i < g->size; i++) {
		if (g->vertices[i].is_set) {
			if (!g->data_cmp(g->vertices[i].data, data)) {
				return -1;
			}
			continue;
		}
		g->vertices[i].is_set = 1;
		g->vertices[i].list = NULL;
		memcpy(&g->vertices[i].data, &data, sizeof data);
		if (v_ret) {
			*v_ret = g->vertices + i;
		}
		return 0;
	}
	return -1;
}

static int edge_new(const __WEIGHT_T weight, struct vertex *to, struct edge *next, struct edge **ret)
{
	struct edge *e = malloc(sizeof *e);
	if (!e) {
		perror("malloc");
		return -1;
	}
	memcpy(&e->weight, &weight, sizeof weight);
	e->to = to;
	e->next = next;
	*ret = e;
	return 0;
}
int graph_add_edge(struct graph *g, struct vertex *from, struct vertex *to, const __WEIGHT_T weight, struct edge **e_ret)
{
	struct edge *e, *prev;
	if (to == from->list->to) {
		return -1;
	} else if (g->weight_cmp(weight, from->list->weight) >= 0) {
		int ret = edge_new(weight, to, from->list, &from->list);
		if (e_ret) {
			*e_ret = from->list;
		}
		return ret;
	}
	for (prev = from->list, e = prev->next; prev != NULL; prev = e, e = e->next) {
		if (to == e->to) {
			return -1;
		}
		if (g->weight_cmp(weight, e->weight) < 0) {
			continue;
		}
		int ret = edge_new(weight, to, e, &prev->next);
		if (e_ret) {
			*e_ret = prev->next;
		}
		return ret;
	}
	return -1;
}

int graph_remove_vertex(struct vertex *v)
{
	struct edge *temp;
	while ((temp = v->list) != NULL) {
		v->list = v->list->next;
		graph_remove_edge(temp);
	}
	v->is_set = 0;
	return 0;
}

int graph_remove_edge(struct edge *e)
{
	free(e);
	return 0;
}
