#ifndef QUEUE_ARR_H
#define QUEUE_ARR_H

#include <stdbool.h>
#include <stdlib.h>
#include "task.h"
#include "errors.h"

#define QUEUE_LENGTH 1000

typedef struct
{
    task_t tasks[QUEUE_LENGTH + 1];
    long in, out;
} queue_arr_t;

error_codes_e push_arr(void *queue, task_t el);
error_codes_e pop_arr(void *queue, task_t *el);
bool is_empty_arr(void *queue);
size_t get_queue_len_arr(void *queue);

#endif /* QUEUE_ARR_H */
