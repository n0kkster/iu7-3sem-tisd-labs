#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

typedef struct
{
    double max_task_generation_interval;
    double max_task_processing_time;
    size_t total_tasks_count;
    size_t task_repeat;
    size_t print_queue_info_frequency;
} simulation_params_t;

#include <stdio.h>
#include "errors.h"
#include "queue_list.h"
#include "queue_arr.h"
#include "random.h"
#include "task.h"
#include "io.h"

#define MAX_TASK_GEN_INTERVAL 6
#define MAX_TASK_PROCESSING_TIME 1

#define TOTAL_TASKS_COUNT 1000
#define TASK_REPEAT 5
#define PRINT_QUEUE_INFO_EVERY_N 100

typedef struct
{
    void *queue;
    error_codes_e (*push)(void *queue, task_t el);
    error_codes_e (*pop)(void *queue, task_t *el);
    bool (*is_empty)(void *queue);
    size_t (*get_queue_len)(void *queue);
} abstract_queue_t;

typedef enum
{
    IDLE,
    BUSY
} processor_state_e;

typedef enum
{
    WAITING,
    READY
} generator_state_e;

typedef struct
{
    task_t current_task;
    processor_state_e state;
    double started_moment;
} processor_t;

typedef struct
{
    double interval;
    double task_time;
    double prev_ready_moment; 
    generator_state_e state;
} generator_t;

typedef struct
{
    size_t curr_queue_len;
    size_t processor_runs_count;
    size_t pushed_tasks;
    size_t processed_tasks;
    double total_time;
    double total_processor_idle_time;
    double average_queue_len;
    double average_task_in_queue_time;
} simulation_info_t;

#endif /* COMMON_H */
