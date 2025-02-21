#ifndef ACTIONS_H
#define ACTIONS_H

typedef enum
{
    EXIT,
    PUSH_ARR_BEG,
    POP_ARR_BEG,
    PEEK_ARR_BEG,
    PRINT_ARR_BEG,
    PUSH_ARR_END,
    POP_ARR_END,
    PEEK_ARR_END,
    PRINT_ARR_END,
    PUSH_LIST,
    POP_LIST,
    PEEK_LIST,
    PRINT_LIST,
    PRINT_LIST_DELETED,
    SPEEDTEST,
    ACTION_COUNT
} action_e;

#include <stdio.h>
#include <sys/time.h>
#include "stack_array.h"
#include "stack_list.h"
#include "io.h"
#include "errors.h"

#define MEASURES 1000

error_codes_e handle_push_beg(stack_arr_t *stack);
error_codes_e handle_pop_beg(stack_arr_t *stack, double *el);
error_codes_e handle_peek_beg(stack_arr_t *stack, double *el);
error_codes_e handle_push_end(stack_arr_t *stack);
error_codes_e handle_pop_end(stack_arr_t *stack, double *el);
error_codes_e handle_peek_end(stack_arr_t *stack, double *el);
error_codes_e handle_push_list(list_node **stack);
error_codes_e handle_pop_list(list_node **stack, list_node **deleted, double *el);
error_codes_e handle_peek_list(list_node *stack, double *el);
void handle_print_list_stack(list_node *stack);
void handle_print_arr_beg(stack_arr_t *stack);
void handle_print_arr_end(stack_arr_t *stack);
void handle_print_deleted(list_node *deleted);
void handle_speedtest();

error_codes_e validate_double(char *buffer, double *el);

#endif /* ACTIONS_H */
