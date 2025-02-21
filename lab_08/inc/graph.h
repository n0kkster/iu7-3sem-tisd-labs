#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
#include "assoc_array.h"
#include "list.h"

typedef struct 
{
    int vertices;
    int edges;
    assoc_array_t adjList;
} graph_t;

error_codes_e create_graph(graph_t **graph);
void insert_edge(graph_t *graph, int src, int dst);
bool is_eulerian(graph_t *graph);
bool is_connected(graph_t *graph);
void remove_edge(graph_t *graph, int src, int dst);
void dfs(graph_t *graph, int v);
int *find_eulerian_path(graph_t *graph, int *path_size);
void print_graph(graph_t *graph);
void free_graph(graph_t **graph);
void draw_graph(graph_t *graph, FILE *f);
void mark_visited(graph_t *graph, int v);
void reset_visited_marks(graph_t *graph);
bool is_visited(graph_t *graph, int v);
graph_t *copy_graph(graph_t *graph);
void print_list(node_t *head);

#endif /* GRAPH_H */
