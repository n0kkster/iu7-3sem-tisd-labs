#include <stdio.h>
#include "io.h"
#include "errors.h"
#include "actions.h"
#include "stack_array.h"
#include "stack_list.h"

int main(void)
{
    error_codes_e rc;
    action_e action;

    stack_arr_t stack_arr;
    list_node *stack_list = NULL, *deleted = NULL;

    double el;

    stack_arr.sp1 = -1;
    stack_arr.sp2 = STACK_DEPTH;

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
            return handle_error(rc);
        
        switch (action)
        {
            case PUSH_ARR_BEG:
                if ((rc = handle_push_beg(&stack_arr)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                break;
            case POP_ARR_BEG:
                if ((rc = handle_pop_beg(&stack_arr, &el)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PEEK_ARR_BEG:
                if ((rc = handle_peek_beg(&stack_arr, &el)) != ERROR_SUCCESS)
                {
                    printf(RED"Стек пуст!\n"RESET);
                    break;
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PUSH_ARR_END:
                if ((rc = handle_push_end(&stack_arr)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                break;
            case POP_ARR_END:
                if ((rc = handle_pop_end(&stack_arr, &el)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PEEK_ARR_END:
                if ((rc = handle_peek_end(&stack_arr, &el)) != ERROR_SUCCESS)
                 {
                    printf(RED"Стек пуст!\n"RESET);
                    break;
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PUSH_LIST:
                if ((rc = handle_push_list(&stack_list)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                break;
            case POP_LIST:
                if ((rc = handle_pop_list(&stack_list, &deleted, &el)) != ERROR_SUCCESS)
                {
                    free_list(stack_list);
                    free_list(deleted);
                    return handle_error(rc);
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PEEK_LIST:
                if ((rc = handle_peek_list(stack_list, &el)) != ERROR_SUCCESS)
                {
                    printf(RED"Стек пуст!\n"RESET);
                    break;
                }
                printf(GREEN);
                printf("%lf\n", el);
                printf(RESET);
                break;
            case PRINT_ARR_BEG:
                handle_print_arr_beg(&stack_arr);
                break;
            case PRINT_ARR_END:
                handle_print_arr_end(&stack_arr);
                break;
            case PRINT_LIST:
                handle_print_list_stack(stack_list);
                break;
            case PRINT_LIST_DELETED:
                handle_print_deleted(deleted);
                break;
            case SPEEDTEST:
                handle_speedtest();
                break;
            default:
                break;
        }
    } while (action != EXIT);
    
    free_list(stack_list);
    free_list(deleted);
    
    return ERROR_SUCCESS;
}