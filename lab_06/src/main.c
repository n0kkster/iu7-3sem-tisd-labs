#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "errors.h"
#include "actions.h"
#include "io.h"
#include "tree.h"

int main(void)
{
    error_codes_e rc;
    action_e action;
    node_t *tree = NULL;

    srand(time(NULL));

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
        {
            free_tree(tree);
            return handle_error(rc);
        }

        switch (action)
        {
            case READ_TREE_FROM_FILE:
                free_tree(tree);
                tree = NULL;
                ENSURE(handle_read_tree(&tree), handle_error(rc););
                break;
            case PRINT_TREE:
                puts("Инфиксный обход дерева: ");
                print_tree_infix(tree);
                printf("\n");

                puts("Префиксный обход дерева: ");
                print_tree_prefix(tree);
                printf("\n");

                puts("Постфиксный обход дерева: ");
                print_tree_postfix(tree);
                printf("\n");

                ENSURE(handle_print_tree_info(tree), free_tree(tree); handle_error(rc););
                break;
            case INSERT_NODE:
                ENSURE(handle_insert_node(&tree), free_tree(tree); handle_error(rc););
                break;
            case REMOVE_NODE:
                ENSURE(handle_remove_node(&tree),);
                break;
            case FIND_NODE:
                ENSURE(handle_find_node(tree), free_tree(tree); handle_error(rc););
                break;
            case GEN_RANDOM_FILE:
                ENSURE(handle_gen_random_file(), free_tree(tree); handle_error(rc););
                break;
            case SPEEDTEST:
                ENSURE(handle_speedtest(), handle_error(rc);)
                break;
            case DRAW_GRAPH:
                ENSURE(handle_create_graph(tree), free_tree(tree); handle_error(rc););
                break;
            default:
                break;
        }
    } while (action != EXIT);

    free_tree(tree);
    
    return ERROR_SUCCESS;
}