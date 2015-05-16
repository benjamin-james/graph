#include "graph.h"

#include "string.h" /* size_t, memcpy, memcmp */
#include "stdlib.h" /* malloc, realloc, free, NULL */


/* assumes v_a and v_b are not the actual pointers */
int graph_adjacent(struct graph *v_from, struct graph *v_to)
{
	struct graph *temp = v_from->edge;
	for (; temp; temp = temp->edge)
		if (temp->vertex == v_to)
			return 1;
	return 0;
}
struct graph *graph_add_vertex(struct graph **g, void *v, size_t size)
{
	struct graph *temp = malloc(sizeof(struct graph));
	temp->data = malloc(size);
	temp->size = size;
	memcpy(temp->data, v, size);
	temp->vertex = *g;
	*g = temp;
	return temp;
}
struct graph *graph_vertex(struct graph **g, void *v)
{
	struct graph *temp = *g;
	for (; temp; temp = temp->vertex)
		if (!memcmp(temp->data, v, temp->size))
			return temp;
	return NULL;
}
struct graph *graph_edge_data(struct graph **g, void *e)
{
	struct graph *temp = *g;
	for (; temp; temp = temp->edge)
		if (!memcmp(temp->data, e, temp->size))
			return temp;
	return NULL;
}
struct graph *graph_edge(struct graph *v_from, struct graph *v_to)
{
	struct graph *temp = v_from;
	for (; temp; temp = temp->edge) {
		if (temp->vertex == v_to)
			return temp;
	}
	return NULL;
}
/*
 * Because some edges might still point at the removed vertex,
 * the vertex is set to NULL if found so it can be properly cleaned
 * up later.
 */
void graph_remove_vertex(struct graph **g, void *v)
{
	struct graph *temp, *prev = NULL;
	for (temp = *g; temp; prev = temp, temp = temp->vertex) {
		if (!memcmp(temp->data, v, temp->size)) {
			free(temp->data);
			temp->data = NULL;
			if (temp != *g)
				prev->next = temp->next;
			else
				*g = temp->next;
			for (prev = temp->edge; prev; prev = prev->edge) {
				free(prev->data);
				free(prev);
			}
			free(temp);
			temp = NULL;
			return;
		}
	}
}
struct graph *graph_add_edge(struct graph *v_from, struct graph *v_to, void *data, size_t size)
{
	struct graph *temp = malloc(sizeof(struct graph));
	temp->data = malloc(size);
	temp->size = size;
	memcpy(temp->data, data, size);
	temp->vertex = v_to;
	temp->edge = v_from->edge;
	v_from->edge = temp;
	return temp;
}
void graph_remove_edge(struct graph *v_from, struct graph *v_to)
{
	struct graph *prev, *temp = v_from->edge;
	for (prev = temp; temp; prev = temp, temp = temp->edge) {
		if (temp->vertex == v_to) {
			free(temp->data);
			if (prev)
				prev->edge = temp->edge;
			else
				v_from->edge = temp->edge;
			free(temp);
			temp = prev;
			return;
		}
	}
}
int graph_clean(struct graph **g)
{
	int count = 0;
	struct graph *temp, *tedge, *prev;
	for (temp = *g; temp; temp = temp->vertex) {
		for (tedge = temp->edge; tedge; prev = tedge, tedge = tedge->edge) {
			if (tedge->vertex == NULL) {
				count++;
				prev->edge = tedge->edge;
				free(tedge);
				tedge = prev->edge;
			}
		}
	}
	return count;
}
