#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node_t node_t;
struct node_t
{
    int v;
    node_t *next;
};

typedef struct
{
    int key;
    int in_degree;
    int out_degree;
    bool visited;
    node_t *value;
} vertex_t;

#include "assoc_array.h"

typedef struct assoc_node_t assoc_node_t;
struct assoc_node_t 
{
    vertex_t *vertex;
    assoc_node_t *next;
};

typedef struct assoc_array_type
{
    assoc_node_t *head;
} assoc_array_type;

assoc_array_error_t insert(assoc_node_t **head, assoc_node_t *elem);
assoc_array_error_t insert_list(node_t **head, node_t *elem);
assoc_array_error_t alloc_node(assoc_node_t **node, int key);
void free_vertex(vertex_t *vertex);
void free_assoc_list(assoc_node_t **head);
assoc_node_t *find(assoc_node_t *head, int key);
node_t *find_in_list(node_t *head, int el);
void free_list(node_t **head);
node_t *copy_list(node_t *head);
assoc_node_t *copy_assoc_list(assoc_node_t *head);

#endif /* LIST_H */
