#ifdef _HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int graph_init(struct graph *g, int (*data_cmp)(const DATA_T left, const DATA_T right), int (*weight_cmp)(const WEIGHT_T left, const WEIGHT_T right))
{
	if (g == NULL) {
		return -1;
	}
	g->head = NULL;
	g->data_cmp = data_cmp;
	g->weight_cmp = weight_cmp;
	return 0;
}

int graph_delete(struct graph *g)
{
	struct vertex *v = g->head;
	for (v = g->head; v != NULL; v = v->next) {
		graph_remove_vertex(g, v);
	}
	return 0;
}

struct vertex * graph_get_vertex(struct graph *g, const DATA_T data)
{
	struct vertex *v = g->head;
	for (v = g->head; v != NULL; v = v->next) {
		if (!g->data_cmp(data, v->data)) {
			return v;
		}
	}
	return NULL;
}

struct edge * graph_get_edge(struct vertex *from, struct vertex *to)
{
	struct edge *e = from->edges;
	for (e = from->edges; e != NULL; e = e->next) {
		if (e->to == to) {
			return e;
		}
	}
	return NULL;
}

struct vertex * graph_add_vertex(struct graph *g, const DATA_T data)
{
	struct vertex *v, *temp, *n;
	for (temp = NULL, v = g->head; v != NULL; temp = v, v = v->next) {
		if (!g->data_cmp(data, v->data)) {
			return v;
		}
	}
	n = malloc(sizeof *n);
	if (n == NULL) {
		perror("malloc");
		return NULL;
	} else if (temp == NULL) {
		g->head = n;
	} else {
		temp->next = n;
	}
	n->data = data;
	n->next = NULL;
	return n;
}

static void _add_edge(struct edge **list, struct edge *n, int (*weight_cmp)(const WEIGHT_T left, const WEIGHT_T right))
{
	struct edge *tmp = *list, *prev = NULL;
	while (tmp != NULL && weight_cmp(tmp->weight, n->weight) <= 0) {		prev = tmp;
		tmp = tmp->next;
	}
	n->next = tmp;
	if (prev == NULL) {
		*list = n;
	} else {
		prev->next = n;
	}
}

struct edge * graph_add_edge(struct graph *g, struct vertex *from, struct vertex *to, const WEIGHT_T weight)
{
	struct edge *e = malloc(sizeof *e);
	if (!e) {
		perror("malloc");
		return NULL;
	}
	e->weight = weight;
	e->to = to;
	e->next = NULL;
	_add_edge(&from->edges, e, g->weight_cmp);
	return e;
}

int graph_remove_vertex_deep(struct graph *g, struct vertex *v)
{
	struct vertex *t;
	for (t = g->head; t != NULL; t = t->next) {
		graph_remove_edge(t, v);
	}
	return graph_remove_vertex(g, v);
}

int graph_remove_vertex(struct graph *g, struct vertex *v)
{
	struct edge *e;
	if (g->head == NULL) {
		return 0;
	}
	while (v->edges != NULL) {
		e = v->edges;
		v->edges = v->edges->next;
		free(e);
	}
	if (g->head == v) {
		struct vertex *temp = g->head;
		g->head = g->head->next;
		free(temp);
	} else {
		struct vertex *cur = g->head->next;
		struct vertex *prev = g->head;
		while (cur != NULL && prev != NULL) {
			if (v == cur) {
				struct vertex *tmp = cur;
				prev->next = cur->next;
				free(tmp);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
	}
	return 0;
}

int graph_remove_edge(struct vertex *from, struct vertex *to)
{
	if (from == NULL || from->edges == NULL) {
		return 0;
	} else if (from->edges->to == to) {
		struct edge *tmp = from->edges;
		from->edges = from->edges->next;
		free(tmp);
	} else {
		struct edge *cur = from->edges->next;
		struct edge *prev = from->edges;
		while (cur != NULL && prev != NULL) {
			if (to == cur->to) {
				struct edge *tmp = cur;
				prev->next = cur->next;
				free(tmp);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
	}
	return 0;
}
