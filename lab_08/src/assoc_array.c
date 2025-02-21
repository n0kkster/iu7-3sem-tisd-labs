#include <stdlib.h>
#include <string.h>
#include "assoc_array.h"
#include "list.h"

assoc_array_t assoc_array_create(void)
{
    assoc_array_t assoc_array = NULL;

    assoc_array = malloc(sizeof(assoc_array_type));
    if (!assoc_array)
        return NULL;
    assoc_array->head = NULL;

    return assoc_array;
}

void assoc_array_destroy(assoc_array_t *arr)
{
    if (!arr || !(*arr))
        return;

    free_assoc_list(&(*arr)->head);
    free(*arr);
    *arr = NULL;
}

int assoc_array_insert(assoc_array_t arr, int key, int val)
{
    assoc_node_t *node;
    node_t *list = NULL, *new;
    assoc_array_error_t rc;

    new = malloc(sizeof(node_t));
    new->v = val;
    new->next = NULL;

    if (assoc_array_find(arr, key, &list) == ASSOC_ARRAY_OK)
    {
        if (insert_list(&list, new) == ASSOC_ARRAY_KEY_EXISTS)
            free(new);
        return 0;
    }

    if ((rc = alloc_node(&node, key)) != ASSOC_ARRAY_OK)
        return rc;

    node->vertex = malloc(sizeof(vertex_t));
    node->vertex->key = key;
    node->vertex->in_degree = 0;
    node->vertex->out_degree = 0;
    node->vertex->value = new;
    node->vertex->visited = false;
    node->next = NULL;

    if ((rc = insert(&arr->head, node)) != ASSOC_ARRAY_OK)
    {
        free_vertex(node->vertex);
        free(node);
        return 0;
    }

    return 1;
}

assoc_array_error_t assoc_array_find(assoc_array_t arr, int key, node_t **found)
{
    assoc_node_t *curr = arr->head;

    while (curr)
    {
        if (curr->vertex->key == key)
        {
            *found = curr->vertex->value;
            return ASSOC_ARRAY_OK;
        }
        curr = curr->next;
    }

    return ASSOC_ARRAY_NOT_FOUND;
}

// assoc_array_error_t assoc_array_remove(assoc_array_t arr, const char *key)
// {
//     node_t *found;

//     if (!key || !strlen(key) || !arr)
//         return ASSOC_ARRAY_INVALID_PARAM;

//     found = find(arr->head, key);
//     if (!found)
//         return ASSOC_ARRAY_NOT_FOUND;

//     remove_list_el(&arr->head, found);

//     return ASSOC_ARRAY_OK;
// }
