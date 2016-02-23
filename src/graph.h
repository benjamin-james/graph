#ifndef GRAPH_H
#define GRAPH_H

#ifndef WEIGHT_T
#define WEIGHT_T int
#endif

#ifndef DATA_T
#define DATA_T char
#endif

struct edge {
	WEIGHT_T weight;
	struct vertex *to;
	struct edge *next;
};

struct vertex {
	DATA_T data;
	struct edge *edges;
	struct vertex *next;
};

struct graph {
	struct vertex *head;
	int (*data_cmp)(const DATA_T left, const DATA_T right);
	int (*weight_cmp)(const WEIGHT_T left, const WEIGHT_T right);
};

int graph_init(struct graph *g, int (*data_cmp)(const DATA_T left, const DATA_T right), int (*weight_cmp)(const WEIGHT_T left, const WEIGHT_T right));
int graph_delete(struct graph *g);

struct vertex * graph_get_vertex(struct graph *g, const DATA_T data);
struct edge * graph_get_edge(struct vertex *from, struct vertex *to);

struct vertex * graph_add_vertex(struct graph *g, const DATA_T data);
struct edge * graph_add_edge(struct graph *g, struct vertex *from, struct vertex *to, const WEIGHT_T weight);

int graph_remove_vertex(struct graph *g, struct vertex *v);
int graph_remove_vertex_deep(struct graph *g, struct vertex *);
int graph_remove_edge(struct vertex *from, struct vertex *to);

#endif
