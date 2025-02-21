#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "errors.h"
#include "actions.h"
#include "io.h"
#include "graph.h"

int main(void)
{
    error_codes_e rc;
    action_e action;
    graph_t *graph = NULL;

    srand(time(NULL));
    if ((rc = create_graph(&graph)) != ERROR_SUCCESS)
        return rc;

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
        {
            free_graph(&graph);
            return handle_error(rc);
        }

        switch (action)
        {
            case READ_DATA_FROM_FILE:
                free_graph(&graph);
                create_graph(&graph);
                ENSURE(handle_read_data_from_file(graph), free_graph(&graph); handle_error(rc););
                break;
            
            case PRINT_GRAPH:
                print_graph(graph);
                break;

            case FIND_EULERIAN_PATH:
                handle_find_eulerian_path(graph);
                break;

            case CHECK_CONNECTED:
                if (is_connected(graph))
                    printf("Граф связный.\n");
                else
                    printf("Граф не связный.\n");
                break;

            case ADD_EDGE:
                handle_add_edge(graph);
                break;
            
            case REMOVE_EDGE:
                handle_remove_edge(graph);
                break;

            case SPEEDTEST:
                ENSURE(handle_speedtest(), );
                break;

            case DRAW_GRAPH:
                ENSURE(handle_draw_graph(graph, "graph"), free_graph(&graph); handle_error(rc););
                break;
            default:
                break;
        }

    } while (action != EXIT);
    
    free_graph(&graph);

    return ERROR_SUCCESS;
}
