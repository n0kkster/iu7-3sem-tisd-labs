#include "list.h"

assoc_array_error_t insert(assoc_node_t **head, assoc_node_t *elem)
{
    assoc_node_t *temp_head;
    if (*head == NULL)
    {
        *head = elem;
        (*head)->next = NULL;
        return ASSOC_ARRAY_OK;
    }

    temp_head = *head;

    while (temp_head->next)
    {
        if (temp_head->vertex->key == elem->vertex->key)
            return ASSOC_ARRAY_KEY_EXISTS;
        temp_head = temp_head->next;
    }

    if (temp_head->vertex->key == elem->vertex->key)
        return ASSOC_ARRAY_KEY_EXISTS;

    elem->next = NULL;
    temp_head->next = elem;

    return ASSOC_ARRAY_OK;
}

assoc_array_error_t insert_list(node_t **head, node_t *elem)
{
    node_t *temp_head;
    if (*head == NULL)
    {
        *head = elem;
        (*head)->next = NULL;
        return ASSOC_ARRAY_OK;
    }

    temp_head = *head;

    while (temp_head->next)
    {
        if (temp_head->v == elem->v)
            return ASSOC_ARRAY_KEY_EXISTS;
        temp_head = temp_head->next;
    }

    if (temp_head->v == elem->v)
        return ASSOC_ARRAY_KEY_EXISTS;

    elem->next = NULL;
    temp_head->next = elem;
    return ASSOC_ARRAY_OK;
}

assoc_array_error_t alloc_node(assoc_node_t **node, int key)
{
    vertex_t *vertex;

    *node = malloc(sizeof(node_t));
    if (!*node)
        return ASSOC_ARRAY_MEM;

    vertex = malloc(sizeof(vertex_t));
    if (!vertex)
    {
        free(*node);
        return ASSOC_ARRAY_MEM;
    }

    vertex->in_degree = 0;
    vertex->out_degree = 0;
    vertex->visited = false;
    vertex->key = key;
    vertex->value = NULL;
   
    (*node)->vertex = vertex;
    (*node)->next = NULL;

    return ASSOC_ARRAY_OK;
}

void free_vertex(vertex_t *vertex)
{
    if (!vertex)
        return;
    
    free_list(&vertex->value);
    free(vertex);
}

void free_assoc_list(assoc_node_t **head)
{
    assoc_node_t *old;

    if (*head == NULL)
        return;

    while (*head)
    {
        old = *head;
        (*head) = (*head)->next;
        free_vertex(old->vertex);
        free(old);
    }
}

void free_list(node_t **head)
{
    node_t *old;

    while (*head)
    {
        old = *head;
        (*head) = (*head)->next;
        free(old);
    }
    *head = NULL;
}

assoc_node_t *find(assoc_node_t *head, int key)
{
    while (head)
    {
        if (head->vertex->key == key)
            return head;
        head = head->next;
    }

    return NULL;
}

node_t *find_in_list(node_t *head, int el)
{
    while (head)
    {
        if (head->v == el)
            return head;
    }
    
    return NULL;
}

node_t *copy_list(node_t *head)
{
    node_t *head_new = NULL, *new;
    while (head)
    {
        new = malloc(sizeof(node_t));
        new->v = head->v;
        new->next = NULL;

        insert_list(&head_new, new);
        head = head->next;
    }

    return head_new;
}

vertex_t *copy_vertex(vertex_t *vertex)
{
    vertex_t *new = malloc(sizeof(vertex_t));

    new->in_degree = vertex->in_degree;
    new->out_degree = vertex->out_degree;
    new->key = vertex->key;
    new->visited = vertex->visited;
    new->value = copy_list(vertex->value);

    return new;
}

assoc_node_t *copy_assoc_list(assoc_node_t *head)
{
    assoc_node_t *head_new = NULL, *curr;
    vertex_t *vertex;

    while (head)
    {
        vertex = copy_vertex(head->vertex);
        curr = malloc(sizeof(assoc_node_t));
        curr->vertex = vertex;
        curr->next = NULL;

        insert(&head_new, curr);

        head = head->next;
    }

    return head_new;
}
