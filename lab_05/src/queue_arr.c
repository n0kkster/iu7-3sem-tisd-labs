#include "queue_arr.h"

error_codes_e push_arr(void *queue, task_t el)
{
    queue_arr_t *queue_internal = queue;

    queue_internal->tasks[queue_internal->in] = el;
    queue_internal->in = (queue_internal->in + 1) % (QUEUE_LENGTH + 1);

    if (queue_internal->in == queue_internal->out)
        return ERROR_QUEUE_FULL;

    return ERROR_SUCCESS;
}

error_codes_e pop_arr(void *queue, task_t *el)
{
    queue_arr_t *queue_internal = queue;

    if (queue_internal->out == queue_internal->in)
        return ERROR_QUEUE_EMPTY;
    
    *el = queue_internal->tasks[queue_internal->out];
    queue_internal->out = (queue_internal->out + 1) % (QUEUE_LENGTH + 1);
    return ERROR_SUCCESS;
}

bool is_empty_arr(void *queue)
{
    queue_arr_t *queue_internal = queue;
    return queue_internal->in == queue_internal->out;
}

size_t get_queue_len_arr(void *queue)
{
    queue_arr_t *queue_internal = queue;
    if (queue_internal->in >= queue_internal->out)
        return (size_t)(queue_internal->in - queue_internal->out);

    return QUEUE_LENGTH - queue_internal->out + queue_internal->in;
}
