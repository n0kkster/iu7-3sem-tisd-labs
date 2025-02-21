#include "actions.h"

error_codes_e validate_double(char *buffer, double *el)
{
    char *ep;

    *el = strtod(buffer, &ep);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER; 

    if (fabs(*el) <= 1e-6 || *el < 0)
        return ERROR_OUT_OF_RANGE;

    return ERROR_SUCCESS;
}

error_codes_e validate_long(char *buffer, long *el)
{
    char *ep;

    *el = strtol(buffer, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER; 
    
    if (*el <= 0)
        return ERROR_OUT_OF_RANGE;

    return ERROR_SUCCESS;
}

error_codes_e handle_set_params(simulation_params_t *params)
{
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    double el;
    long long_el;

    puts("Все введенные числа должны быть положительными!");

    puts("Введите максимальный интервал генерации задачи (вещественное):");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_double(buffer, &el)) != ERROR_SUCCESS)
        return rc;
    
    params->max_task_generation_interval = el;

    puts("Введите максимальное время выполнения задачи (вещественное):");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_double(buffer, &el)) != ERROR_SUCCESS)
        return rc;

    params->max_task_processing_time = el;

    puts("Введите общее количество задач (целое):");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_long(buffer, &long_el)) != ERROR_SUCCESS)
        return rc;

    params->total_tasks_count = long_el;

    puts("Введите число повторений одной задачи (целое):");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_long(buffer, &long_el)) != ERROR_SUCCESS)
        return rc;

    params->task_repeat = long_el;

    puts("Введите частоту вывода информации об очереди (целое):");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_long(buffer, &long_el)) != ERROR_SUCCESS)
        return rc;

    params->print_queue_info_frequency = long_el;

    return ERROR_SUCCESS;
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

size_t calc_size(node_t *queue_list)
{
    size_t sz = 0;
    while (queue_list)
    {
        sz += sizeof(*queue_list);
        queue_list = queue_list->next;
    }
    return sz;
}

error_codes_e handle_efficiency_test(void)
{
    queue_arr_t queue_arr = { 0 };
    node_t *queue_list = NULL;
    task_t task, out_task;
    unsigned long beg, end;
    double push_arr_time = 0, pop_arr_time = 0, push_list_time = 0, pop_list_time = 0;

    task.execution_time = 0.152;
    task.executions = 100;
    task.start_time = 2.3;

    for (size_t i = 0; i < MEASURES / QUEUE_LENGTH; i++)
    {
        beg = micros();
        for (size_t j = 0; j < QUEUE_LENGTH; j++)
            ENSURE(push_arr(&queue_arr, task));
        end = micros();
        push_arr_time += (end - beg) / (double)QUEUE_LENGTH;

        for (size_t j = 0; j < QUEUE_LENGTH; j++)
            ENSURE(pop_arr(&queue_arr, &out_task));
    }
    push_arr_time /= MEASURES / QUEUE_LENGTH;

    for (size_t i = 0; i < MEASURES / QUEUE_LENGTH; i++)
    {
        for (size_t j = 0; j < QUEUE_LENGTH; j++)
            ENSURE(push_arr(&queue_arr, task));
        beg = micros();
        for (size_t j = 0; j < QUEUE_LENGTH; j++)
            ENSURE(pop_arr(&queue_arr, &out_task));
        end = micros();
        pop_arr_time += (end - beg) / (double)QUEUE_LENGTH;
    }
    pop_arr_time /= MEASURES / QUEUE_LENGTH;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        ENSURE(push_list(&queue_list, task));
    end = micros();
    push_list_time = (end - beg) / (double)MEASURES;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        ENSURE(pop_list(&queue_list, &out_task));
    end = micros();
    pop_list_time = (end - beg) / (double)MEASURES;

    printf("Время добавления элемента в очередь на массиве: %lf\n", push_arr_time);
    printf("Время добавления элемента в очередь на списке: %lf\n", push_list_time);
    printf("Время удаления элемента из очереди на массиве: %lf\n", pop_arr_time);
    printf("Время удаления элемента из очереди на списке: %lf\n", pop_list_time);

    free_queue(queue_list);
    queue_arr.in = 0;
    queue_arr.out = 0;

    printf("Количество элементов в очереди | Размер очереди на списке | Размер очереди на массиве | Соотношение\n");

    for (size_t size = 10; size <= QUEUE_LENGTH; size += 10)
    {
        for (size_t i = 0; i < size; i++)
            ENSURE(push_arr(&queue_arr, task))
        for (size_t i = 0; i < size; i++)
            ENSURE(push_list(&queue_list, task))

        printf("%15zu\t%30zu\t%25zu\t%15.4f\n", size, calc_size(queue_list), sizeof(queue_arr), (float)sizeof(queue_arr) / calc_size(queue_list));

        free_queue(queue_list);
        queue_list = NULL;
        queue_arr.in = 0;
        queue_arr.out = 0;

    } 
    return ERROR_SUCCESS;
}
