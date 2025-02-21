#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

#define BLOCK_SIZE 16
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct node_t
{
    long key;
    struct node_t *left;
    struct node_t *right;
} node_t;

typedef struct
{
    int tree_depth;
    int curr_depth;
    int *nodes_on_depth;
    int realloc_factor;
} tree_info_t;


error_codes_e insert(node_t **tree, long el);
void print_tree_infix(node_t *tree);
void print_tree_prefix(node_t *tree);
void print_tree_postfix(node_t *tree);
void free_tree(node_t *tree);
node_t* search(node_t *tree, long el);
void get_tree_info(node_t *tree, tree_info_t *tree_info);
void tree_to_graph(node_t *tree, FILE *f);
error_codes_e remove_node(node_t **tree, node_t *node);

#endif /* TREE_H */
