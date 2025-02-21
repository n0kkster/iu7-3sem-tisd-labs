#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include "errors.h"

typedef struct list_node list_node;
struct list_node
{
    double data;
    list_node *next;
};

error_codes_e pop(list_node **stack, list_node **deleted, double *el);
error_codes_e push(list_node **stack, double el);
error_codes_e peek(list_node *stack, double *el);
void free_list(list_node *stack);

#endif /* STACK_LIST_H */
