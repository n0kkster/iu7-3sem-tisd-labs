#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ENSURE(x, ...) do { error_codes_e rc; if ((rc = x) != ERROR_SUCCESS) {__VA_ARGS__ return rc;} } while(0);
#define MEASURES 1000

typedef enum
{
    EXIT,
    READ_TREE_FROM_FILE,
    BST_TO_AVL,
    PRINT_TREE,
    INSERT_NODE,
    REMOVE_NODE,
    FIND_NODE,
    GEN_RANDOM_FILE,
    SPEEDTEST,
    DRAW_GRAPH,
    TOGGLE_HASHING,
    ACTION_COUNT
} action_e;

typedef enum
{
    BST = 1,
    AVL,
    HASHTABLE
} data_type_e;

#include "errors.h"
#include "files.h"
#include "tree.h"
#include "avl_tree.h"
#include "hashtable.h"
#include "io.h"

error_codes_e handle_read_data(void *data, error_codes_e (*insert)(void *, long));
error_codes_e handle_read_hashtable(hashtable_t **ht, error_codes_e (*insert)(void *, long), hashing_e hashing);
void convert_bst_to_avl(node_t *tree, avl_node_t **avl_tree);
error_codes_e handle_insert_node(void *data, error_codes_e (*insert)(void *, long));
error_codes_e handle_find_node(void *data, void *(*search)(void *, long), data_type_e dt);
error_codes_e handle_print_tree_info(void *tree, void (*tree_info_fn)(void *, tree_info_t *));
error_codes_e handle_create_graph(void *data, const char *filename, void (*data_to_graph)(void *, FILE *));
error_codes_e handle_remove_node(void *data, void *(*search)(void *, long), error_codes_e (*remove_node)(void *, void *), data_type_e dt);
error_codes_e handle_gen_random_file(void);
error_codes_e handle_speedtest(void);

#endif /* ACTIONS_H */
