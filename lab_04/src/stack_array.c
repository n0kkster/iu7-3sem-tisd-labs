#include "stack_array.h"

error_codes_e push_beg(stack_arr_t *stack, double el)
{
    if (stack->sp1 + 1 >= stack->sp2)
        return ERROR_STACK_OVERFLOW;
    stack->arr[++stack->sp1] = el;
    return ERROR_SUCCESS;
}

error_codes_e push_end(stack_arr_t *stack, double el)
{
    if (stack->sp2 - 1 <= stack->sp1)
        return ERROR_STACK_OVERFLOW;
    stack->arr[--stack->sp2] = el;
    return ERROR_SUCCESS;
}

error_codes_e pop_beg(stack_arr_t *stack, double *el)
{
    if (stack->sp1 < 0)
        return ERROR_EMPTY_STACK;
    *el = stack->arr[stack->sp1--];
    return ERROR_SUCCESS;
}

error_codes_e pop_end(stack_arr_t *stack, double *el)
{
    if (stack->sp2 >= STACK_DEPTH)
        return ERROR_EMPTY_STACK;
    *el = stack->arr[stack->sp2++];
    return ERROR_SUCCESS;
}

error_codes_e peek_beg(stack_arr_t *stack, double *el)
{
    if (stack->sp1 < 0)
        return ERROR_EMPTY_STACK;
    *el = stack->arr[stack->sp1];
    return ERROR_SUCCESS;
}

error_codes_e peek_end(stack_arr_t *stack, double *el)
{
    if (stack->sp2 >= STACK_DEPTH)
        return ERROR_EMPTY_STACK;
    *el = stack->arr[stack->sp2];
    return ERROR_SUCCESS;
}