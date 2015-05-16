#ifndef GRAPH_H
#define GRAPH_H

#include "string.h" /* size_t */
/*
 * Graph
 * a list of vertices (vertex) with data
 * and a list of edges (edge) on each
 * vertex that contains a pointer to the
 * vertex (vertex) and the next item (edge)
 *
 * The data will most likely implemented
 * as a struct (it is highly recommended)
 */

struct graph {
	void *data;
	size_t size;
	struct graph *edge;
	struct graph *vertex;
};

/* Gets the vertex defined by v */
struct graph *graph_vertex(struct graph **g, void *v);

/* Gets the edge defined by e */
struct graph *graph_edge_data(struct graph **g, void *e);

/* Gets the edge defined from v_from to v_to */
struct graph *graph_edge(struct graph *v_from, struct graph *v_to);

/* Cleans up graph, i.e. removes all NULL elements, returns 0 if nothing had been done */
int graph_clean(struct graph **g);

/* Sees if the vertex v_a is adjacent to vertex v_b */
int graph_adjacent(struct graph *v_a, struct graph *v_b);

/* Adds the vertex v to the graph, returns its pointer */
struct graph *graph_add_vertex(struct graph **g, void *v, size_t size);

/* Removes the vertex at v */
void graph_remove_vertex(struct graph **g, struct graph *v);

/* Adds an edge from v_from to v_to defined by data */
struct graph *graph_add_edge(struct graph *v_from, struct graph *v_to, void *data, size_t size);

/* Removes the edge defined by v_from to v_to */
void graph_remove_edge(struct graph *v_from, struct graph *v_to);

#endif
