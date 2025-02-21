#include <stdio.h>
#include "queue_arr.h"
#include "queue_list.h"
#include "simulation.h"
#include "calculation.h"
#include "errors.h"

int main(void)
{
    action_e action;
    error_codes_e rc;
    
    queue_arr_t queue_arr = { 0 };
    node_t *queue_list = NULL;

    simulation_params_t params = { 0 };

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
            return handle_error(rc);
        
        switch (action)
        {
            case SET_SIMULATION_PARAMS:
                if ((rc = handle_set_params(&params)) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
                break;
            case RUN_SIMULATION_ARR:
                if ((rc = run_simulation(&queue_arr, push_arr, pop_arr, is_empty_arr, get_queue_len_arr, &params)) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
                queue_arr.in = 0;
                queue_arr.out = 0;
                break;
            case RUN_SIMULATION_LIST:
                if ((rc = run_simulation(&queue_list, push_list, pop_list, is_empty_list, get_queue_len_list, &params)) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
                free_queue(queue_list);
                queue_list = NULL;
                break;
            case RUN_CALCULATION_ARR:
                if ((rc = run_calculation(&queue_arr, push_arr, pop_arr, is_empty_arr, get_queue_len_arr, &params)) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
                queue_arr.in = 0;
                queue_arr.out = 0;
                break;
            case RUN_CALCULATION_LIST:
                if ((rc = run_calculation(&queue_list, push_list, pop_list, is_empty_list, get_queue_len_list, &params)) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
                free_queue(queue_list);
                queue_list = NULL;
                break;
            case TOGGLE_VERBOSE_MODE:
                toggle_verbose();
                break;
            case TOGGLE_PRINT_MEMORY_INFO:
                toggle_mem_verbose();
                break;
            case EFFICIENCY_TEST:
                if ((rc = handle_efficiency_test()) != ERROR_SUCCESS)
                {
                    free_queue(queue_list);
                    return handle_error(rc);
                }
            default:
                break;
        }
    } while (action != EXIT);

    return ERROR_SUCCESS;
}
