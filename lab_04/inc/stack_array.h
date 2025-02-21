#ifndef STACK_ARRAY_H
#define STACK_ARRAY_H

#include "errors.h"

#define STACK_DEPTH 1000

typedef struct
{
    double arr[STACK_DEPTH];
    long sp1, sp2;
} stack_arr_t;

error_codes_e pop_beg(stack_arr_t *stack, double *el);
error_codes_e peek_beg(stack_arr_t *stack, double *el);
error_codes_e push_beg(stack_arr_t *stack, double el);

error_codes_e pop_end(stack_arr_t *stack, double *el);
error_codes_e peek_end(stack_arr_t *stack, double *el);
error_codes_e push_end(stack_arr_t *stack, double el);

#endif /* STACK_ARRAY_H */
