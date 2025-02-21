#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ENSURE(x, ...) {error_codes_e rc; if ((rc = x) != ERROR_SUCCESS) {__VA_ARGS__ return rc;} }
#define MEASURES 1000

typedef enum
{
    EXIT,
    READ_TREE_FROM_FILE,
    PRINT_TREE,
    INSERT_NODE,
    REMOVE_NODE,
    FIND_NODE,
    GEN_RANDOM_FILE,
    SPEEDTEST,
    DRAW_GRAPH,
    ACTION_COUNT
} action_e;

#include "errors.h"
#include "files.h"
#include "tree.h"
#include "io.h"

error_codes_e handle_read_tree(node_t **tree);
error_codes_e handle_insert_node(node_t **tree);
error_codes_e handle_find_node(node_t *tree);
error_codes_e handle_print_tree_info(node_t *tree);
error_codes_e handle_create_graph(node_t *tree);
error_codes_e handle_remove_node(node_t **tree);
error_codes_e handle_gen_random_file(void);
error_codes_e handle_speedtest(void);

#endif /* ACTIONS_H */
