#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "errors.h"
#include "actions.h"
#include "io.h"
#include "tree.h"
#include "avl_tree.h"
#include "hashtable.h"

#define FREE_ALL free_tree(tree); free_avl_tree(avl_tree); free_hashtable(ht_open); free_hashtable(ht_closed);
static hashing_e hashing = OPEN;

int main(void)
{
    error_codes_e rc;
    action_e action;
    node_t *tree = NULL;
    avl_node_t *avl_tree = NULL;
    hashtable_t *ht_open, *ht_closed;
    data_type_e dt;

    srand(time(NULL));

    init_hashtable(&ht_open, CLOSED, 100);
    init_hashtable(&ht_closed, CLOSED, 100);

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
        {
            FREE_ALL
            return handle_error(rc);
        }

        switch (action)
        {
            case READ_TREE_FROM_FILE:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        free_tree(tree);
                        tree = NULL;
                        ENSURE(handle_read_data(&tree, insert_tree), FREE_ALL handle_error(rc););
                        break;
                    case AVL:
                        free_avl_tree(avl_tree);
                        avl_tree = NULL;
                        ENSURE(handle_read_data(&avl_tree, insert_avl_tree), FREE_ALL handle_error(rc););
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                        {
                            ENSURE(handle_read_hashtable(&ht_open, insert_hashtable, OPEN), FREE_ALL handle_error(rc););
                        }
                        else
                        {
                            ENSURE(handle_read_hashtable(&ht_closed, insert_hashtable, CLOSED), FREE_ALL handle_error(rc););
                        }
                        break;
                }
                break;
            case BST_TO_AVL:
                if (tree)
                {
                    free_avl_tree(avl_tree);
                    convert_bst_to_avl(tree, &avl_tree);
                }
                else
                    puts("Дерево пустое!");
                break;
            case PRINT_TREE:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        puts("==========================");
                        puts("BST:");
                        puts("Инфиксный обход дерева: ");
                        print_tree_infix(tree);
                        printf("\n");

                        puts("Префиксный обход дерева: ");
                        print_tree_prefix(tree);
                        printf("\n");

                        puts("Постфиксный обход дерева: ");
                        print_tree_postfix(tree);
                        printf("\n");

                        ENSURE(handle_print_tree_info(tree, get_tree_info), FREE_ALL handle_error(rc););
                        puts("==========================");
                        break;
                    case AVL:
                        puts("==========================");
                        puts("AVL:");
                        puts("Инфиксный обход дерева: ");
                        print_avl_tree_infix(avl_tree);
                        printf("\n");

                        puts("Префиксный обход дерева: ");
                        print_avl_tree_prefix(avl_tree);
                        printf("\n");

                        puts("Постфиксный обход дерева: ");
                        print_avl_tree_postfix(avl_tree);
                        printf("\n");

                        ENSURE(handle_print_tree_info(avl_tree, get_avl_tree_info), FREE_ALL handle_error(rc););
                        puts("==========================");
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                            print_hashtable(ht_open);
                        else
                            print_hashtable(ht_closed);
                        break;
                }
                break;
            case INSERT_NODE:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        ENSURE(handle_insert_node(&tree, insert_tree), FREE_ALL handle_error(rc););
                        break;
                    case AVL:
                        ENSURE(handle_insert_node(&avl_tree, insert_avl_tree), FREE_ALL handle_error(rc););
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                        {
                            ENSURE(handle_insert_node(&ht_open, insert_hashtable), FREE_ALL handle_error(rc););
                        }
                        else
                        {
                            ENSURE(handle_insert_node(&ht_closed, insert_hashtable), FREE_ALL handle_error(rc););
                        }
                        break;
                }
                break;
            case REMOVE_NODE:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        ENSURE(handle_remove_node(&tree, search_tree, remove_node, dt),);
                        break;
                    case AVL:
                        ENSURE(handle_remove_node(&avl_tree, search_avl_tree, remove_node_avl_tree, dt),);
                        avl_tree = balance_all(avl_tree);
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                        {
                            ENSURE(handle_remove_node(ht_open, search_hashtable, remove_from_hashtable, dt),);
                        }
                        else
                        {
                            ENSURE(handle_remove_node(ht_closed, search_hashtable, remove_from_hashtable, dt),);
                        }
                        break;
                }
                break;
            case FIND_NODE:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        ENSURE(handle_find_node(tree, search_tree, dt), FREE_ALL handle_error(rc););
                        break;
                    case AVL:
                        ENSURE(handle_find_node(avl_tree, search_avl_tree, dt), FREE_ALL handle_error(rc););
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                        {
                            ENSURE(handle_find_node(ht_open, find_hashtable, dt), FREE_ALL handle_error(rc););
                        }
                        else
                        {
                            ENSURE(handle_find_node(ht_closed, find_hashtable, dt), FREE_ALL handle_error(rc););
                        }
                        break;
                }
                break;
            case GEN_RANDOM_FILE:
                ENSURE(handle_gen_random_file(), FREE_ALL handle_error(rc););
                break;
            case SPEEDTEST:
                ENSURE(handle_speedtest(), handle_error(rc);)
                break;
            case DRAW_GRAPH:
                ENSURE(read_data_choice(&dt), FREE_ALL handle_error(rc););
                switch (dt)
                {
                    case BST:
                        ENSURE(handle_create_graph(tree, "BST", tree_to_graph), FREE_ALL handle_error(rc););
                        break;
                    case AVL:
                        ENSURE(handle_create_graph(avl_tree, "AVL", avl_tree_to_graph), FREE_ALL handle_error(rc););
                        break;
                    case HASHTABLE:
                        if (hashing == OPEN)
                        {
                            ENSURE(handle_create_graph(ht_open, "HT_OPEN", hashtable_to_graph), FREE_ALL handle_error(rc););
                        }
                        else
                        {
                            ENSURE(handle_create_graph(ht_closed, "HT_CLOSED", hashtable_to_graph), FREE_ALL handle_error(rc););
                        }
                        break;
                }
                break;
            case TOGGLE_HASHING:
                hashing = 1 - hashing;
            default:
                break;
        }
    } while (action != EXIT);

    free_tree(tree);
    free_avl_tree(avl_tree);
    free_hashtable(ht_open);
    free_hashtable(ht_closed);
    
    return ERROR_SUCCESS;
}