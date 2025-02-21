#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "errors.h"
#include "task.h"
#include "io.h"


typedef struct node_t
{
    task_t task;
    struct node_t *next;
} node_t;


error_codes_e push_list(void *queue, task_t el);
error_codes_e pop_list(void *queue, task_t *el);
bool is_empty_list(void *queue);
size_t get_queue_len_list(void *queue);
void free_queue(node_t *queue);

#endif /* QUEUE_LIST_H */
