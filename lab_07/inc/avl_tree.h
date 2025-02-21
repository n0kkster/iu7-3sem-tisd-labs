#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "tree.h"

typedef struct avl_node_t
{
    long key;
    struct avl_node_t *left;
    struct avl_node_t *right;
    int height;
} avl_node_t;

error_codes_e insert_avl_tree(void *vavl_tree, long el);
void print_avl_tree_infix(avl_node_t *avl_tree);
void print_avl_tree_prefix(avl_node_t *avl_tree);
void print_avl_tree_postfix(avl_node_t *avl_tree);
void free_avl_tree(avl_node_t *avl_tree);
void *search_avl_tree(void *vavl_tree, long el);
void get_avl_tree_info(void *vavl_tree, tree_info_t *tree_info);
void avl_tree_to_graph(void *vavl_tree, FILE *f);
error_codes_e remove_node_avl_tree(void *vavl_tree, void *vnode);
avl_node_t *balance(avl_node_t *avl_tree);
int get_height(avl_node_t *avl_tree);
void set_height(avl_node_t *avl_tree);
avl_node_t *balance_all(avl_node_t *avl_tree);
int calc_compares_avl_tree(avl_node_t *tree, long el, int compares);

#endif /* AVL_TREE_H */
