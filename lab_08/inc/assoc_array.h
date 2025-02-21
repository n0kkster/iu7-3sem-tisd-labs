#ifndef ASSOC_ARRAY_H
#define ASSOC_ARRAY_H

typedef struct assoc_array_type *assoc_array_t;

typedef enum error
{
    ASSOC_ARRAY_OK = 0,
    ASSOC_ARRAY_INVALID_PARAM, 
    ASSOC_ARRAY_MEM,
    ASSOC_ARRAY_NOT_FOUND,
    ASSOC_ARRAY_KEY_EXISTS
} assoc_array_error_t;

#include "list.h"

assoc_array_t assoc_array_create(void);
void assoc_array_destroy(assoc_array_t *arr);
int assoc_array_insert(assoc_array_t arr, int key, int val);
assoc_array_error_t assoc_array_find(assoc_array_t arr, int key, node_t **found);

#endif /* ASSOC_ARRAY_H */

