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

error_codes_e insert_tree(void *vtree, long el);
void print_tree_infix(node_t *tree);
void print_tree_prefix(node_t *tree);
void print_tree_postfix(node_t *tree);
void free_tree(node_t *tree);
void *search_tree(void *vtree, long el);
void get_tree_info(void *vtree, tree_info_t *tree_info);
void tree_to_graph(void *tree, FILE *f);
error_codes_e remove_node(void *vtree, void *vnode);
int calc_compares_tree(node_t *tree, long el, int compares);

#endif /* TREE_H */
