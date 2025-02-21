#include "queue_list.h"

void print_queue_addresses(node_t *queue)
{
    while (queue)
    {
        mem_print_mem(queue);
        queue = queue->next;
    }
    mem_printf_info("\n");
}

error_codes_e push_list(void *queue, task_t el)
{
    node_t **queue_internal = queue;

    node_t *node = malloc(sizeof(node_t));
    if (!node)
        return ERROR_ALLOCATING_MEM;

    mem_printf_info("Адреса очереди до добавления элемента:\n");
    print_queue_addresses(*queue_internal);        

    node->next = *queue_internal;
    *queue_internal = node;
    node->task = el;

    mem_printf_info("Адреса очереди после добавления элемента:\n");
    print_queue_addresses(*queue_internal);   

    return ERROR_SUCCESS;
}

error_codes_e pop_list(void *queue, task_t *el)
{
    node_t **queue_internal = queue;
    node_t *cur = *queue_internal, *prev = *queue_internal;

    if (queue == NULL)
        return ERROR_QUEUE_EMPTY;

    mem_printf_info("Адреса очереди до удаления элемента:\n");
    print_queue_addresses(*queue_internal);  

    if (cur->next == NULL)
    {
        *el = cur->task;
        free(cur);
        *queue_internal = NULL;
        return ERROR_SUCCESS;
    }

    while (cur->next)
    {
        prev = cur;
        cur = cur->next;
    }

    *el = cur->task;
    prev->next = NULL;

    mem_printf_info("Адреса очереди после удаления элемента:\n");
    print_queue_addresses(*queue_internal);  

    free(cur);
    return ERROR_SUCCESS;
}

bool is_empty_list(void *queue)
{
    node_t **queue_internal = queue;
    return *queue_internal == NULL;
}

size_t get_queue_len_list(void *queue)
{
    node_t **queue_internal = queue;
    node_t *temp_head = *queue_internal;
    size_t len = 0;

    while (temp_head)
    {
        len++;
        temp_head = temp_head->next;
    }

    return len;
}

void free_queue(node_t *queue)
{
    if (queue == NULL)
        return;

    node_t *old;
    while (queue)
    {
        old = queue;
        queue = queue->next;
        free(old);
    }
}
