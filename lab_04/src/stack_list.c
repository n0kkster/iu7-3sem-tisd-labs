#include "stack_list.h"

error_codes_e push(list_node **stack, double el)
{
    list_node *node = malloc(sizeof(list_node));
    node->next = *stack;
    *stack = node;
    node->data = el;
    return ERROR_SUCCESS;
}

error_codes_e pop(list_node **stack, list_node **deleted, double *el)
{
    list_node *old;

    if (*stack == NULL)
        return ERROR_EMPTY_STACK;    

    old = *stack;
    *stack = old->next;
    *el = old->data;

    old->next = *deleted;
    *deleted = old;

    return ERROR_SUCCESS;
}

error_codes_e peek(list_node *stack, double *el)
{
    if (stack == NULL)
        return ERROR_EMPTY_STACK;

    *el = stack->data;
    return ERROR_SUCCESS;
}

void free_list(list_node *stack)
{
    if (stack == NULL)
        return;

    list_node *old;
    while (stack)
    {
        old = stack;
        stack = stack->next;
        free(old);
    }
}
