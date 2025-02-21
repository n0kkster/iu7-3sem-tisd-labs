#include "simulation.h"

// Псевдо таймер работы симуляции
static double pseudo_clock = 0;

// Генератор заявок
static generator_t generator;

// Обработчик заявок
static processor_t processor;

// Очередь заявок
static abstract_queue_t queue;

// Информация о работе симуляции
static simulation_info_t simulation_info;

// Параметры симуляции
static simulation_params_t simulation_params;

// Функция для подсчета погрешности по общему времени работы
double calc_error_total_time(void)
{
    double error = (simulation_info.total_time / (double)(simulation_params.total_tasks_count * (simulation_params.max_task_generation_interval / 2.0)) - 1) * 100;
    return error > 0 ? error : -error;
}

// Функция для подсчета погрешности по количеству выполненных задач
double calc_error_tasks_count(void)
{
    double expected_tasks_count = simulation_info.total_time / (simulation_params.max_task_generation_interval / 2.0);
    double error = (simulation_info.pushed_tasks / expected_tasks_count - 1) * 100;
    return error > 0 ? error : -error;
}

// Функция для подсчета погрешности по сумме времени работы и простоя 
double calc_error_sum_times(void)
{
    double expected_total_time = simulation_info.processor_runs_count * simulation_params.max_task_processing_time / 2.0 + simulation_info.total_processor_idle_time;
    double error = (simulation_info.total_time / expected_total_time - 1) * 100;
    return error > 0 ? error : -error;
}

// Функция для инициализации параметров симуляции
void init_simulation(void *queue_ptr, error_codes_e (*push)(void *, task_t), error_codes_e (*pop)(void *, task_t *), bool (*is_empty)(void *), size_t (*get_queue_len)(void *))
{
    // Обнуление таймера
    pseudo_clock = 0;

    // Создание структуры для текущей задачи и ее обнуление
    task_t task;
    task.executions = 0;
    task.execution_time = 0;
    task.start_time = 0;

    // Заполнение абстрактной очереди указателями на очередь и на необходимые функции
    queue.queue = queue_ptr;
    queue.push = push;
    queue.pop = pop;
    queue.is_empty = is_empty;
    queue.get_queue_len = get_queue_len;

    // Инициализация генератора начальными значениями
    generator.state = WAITING;
    generator.task_time = gen_random_less_than(simulation_params.max_task_processing_time);
    generator.interval = gen_random_less_than(simulation_params.max_task_generation_interval);
    generator.prev_ready_moment = 0;

    dprintf("[%.3lf]: Генератор:\n\tНовая задача: будет добавлена в очередь в %.3lf е.в. (интервал генерации: %.3lf е.в.), время исполнения: %.3lf е.в.\n", \
    pseudo_clock, pseudo_clock + generator.interval, generator.interval, generator.task_time);

    // Инициализация процессора начальными значениями
    processor.state = IDLE;
    processor.current_task = task;
    processor.started_moment = 0;

    // Обнуление структуры, хранящей информацию о симуляции
    simulation_info.curr_queue_len = 0;
    simulation_info.processor_runs_count = 0;
    simulation_info.total_processor_idle_time = 0;
    simulation_info.total_time = 0;
    simulation_info.pushed_tasks = 0;
    simulation_info.average_queue_len = 0;
    simulation_info.average_task_in_queue_time = 0;
    simulation_info.processed_tasks = 0;
}

// Функция для инициализации параметров симуляции
static void init_params(simulation_params_t *params)
{   
    // Если параметры не были переданы, инициализация стандартными значениями по заданию
    if (params->total_tasks_count == 0)
    {
        simulation_params.max_task_generation_interval = MAX_TASK_GEN_INTERVAL;
        simulation_params.max_task_processing_time = MAX_TASK_PROCESSING_TIME;
        simulation_params.print_queue_info_frequency = PRINT_QUEUE_INFO_EVERY_N;
        simulation_params.task_repeat = TASK_REPEAT;
        simulation_params.total_tasks_count = TOTAL_TASKS_COUNT;
    }
    // Иначе инициализация переданными параметрами
    else
        simulation_params = *params;
}

// Функция для обновления генератора
static error_codes_e update_generator(generator_t *generator)
{
    task_t task;
    error_codes_e rc;

    // Если генератор ожидает времени добавления задачи в очередь...
    if (generator->state == WAITING)
    {
        // ... и оно уже наступило
        if (pseudo_clock - generator->prev_ready_moment >= generator->interval)
        {
            // Перевод генератор в состояние готовности
            generator->state = READY;
        }
    }
    // Если генератор готов добавить задачу в очередь
    if (generator->state == READY)
    {
        // Установка необходимых параметров задачи
        task.executions = 0;
        task.execution_time = generator->task_time;
        task.start_time = pseudo_clock;

        // Добавление задачи в очередь
        if ((rc = queue.push(queue.queue, task)) != ERROR_SUCCESS)
            return rc;

        // Обновление параметров генератора для создания следующей задачи и перевод его в состояние ожидания
        generator->prev_ready_moment = pseudo_clock;
        generator->interval = gen_random_less_than(simulation_params.max_task_generation_interval);
        generator->task_time = gen_random_less_than(simulation_params.max_task_processing_time);
        generator->state = WAITING;

        // Обновление статистки работы симуляции
        simulation_info.pushed_tasks++;
        dprintf("[%.3lf]: Генератор:\n\tЗадача добавлена в очередь!\n", pseudo_clock);
        dprintf("[%.3lf]: Генератор:\n\tНовая задача: будет добавлена в очередь в %.3lf е.в. (интервал генерации: %.3lf е.в.), время испол.: %.3lf е.в.\n\n\n", \
        pseudo_clock, pseudo_clock + generator->interval, generator->interval, generator->task_time);
    }   
    return ERROR_SUCCESS;
}

// Функция для обновления работы процессора
static error_codes_e update_processor(processor_t *processor)
{
    error_codes_e rc;

    // Если процессор простаивает...
    if (processor->state == IDLE)
    {
        // ... и доступных задач нет...
        if (queue.is_empty(queue.queue))
        {
            // ... то увеличить счетчик времени простоя процессора и выйти из функции
            simulation_info.total_processor_idle_time += CLOCK_RESOLUTION;
            return ERROR_SUCCESS;
        }

        // Иначе, если задачи есть, перевести процессор в состояние работы
        processor->state = BUSY;
        
        // Достать задачу из очереди.
        if ((rc = queue.pop(queue.queue, &processor->current_task)) != ERROR_SUCCESS)
            return rc;

        // Обновление параметров процессора
        processor->started_moment = pseudo_clock;
        simulation_info.processor_runs_count++;
        dprintf("[%.3lf]: Процессор:\n\tЗагружена задача: время испол.: %.3lf е.в., будет завершена в %.3lf е.в., запуск процессора #%zu:\n", \
        pseudo_clock, processor->current_task.execution_time, processor->current_task.execution_time + processor->started_moment, simulation_info.processor_runs_count);
    }

    // Если процессор занят обработкой задачи...
    if (processor->state == BUSY)
    {
        // ... и если время ее обработки подошло к концу
        if (pseudo_clock - processor->started_moment >= processor->current_task.execution_time)
        {
            // Перевод процессора в состояние простоя
            processor->state = IDLE;

            // Если количество выполнений текущей задачи не превысило порог исполнений
            if (++processor->current_task.executions < simulation_params.task_repeat)
            {
                dprintf("[%.3lf]: Процессор:\n\tВыгружена задача (время испол.: %.3lf е.в.) в очередь, исполнена %u раз(а)\n", \
                pseudo_clock, processor->current_task.execution_time, processor->current_task.executions);
                
                // То вернуть задачу в очередь
                if ((rc = queue.push(queue.queue, processor->current_task)) != ERROR_SUCCESS)
                    return rc;
            }
            else
            {
                // Иначе обновить статистику работы симуляции
                simulation_info.processed_tasks++;
                simulation_info.average_task_in_queue_time += pseudo_clock - processor->current_task.start_time;

                // При необходимости вывести информацию об очереди.
                if (simulation_info.processed_tasks % simulation_params.print_queue_info_frequency == 0)
                {
                    printf("Обработано задач: %zu\n", simulation_info.processed_tasks);
                    printf("Текущая длина очереди: %zu\n", queue.get_queue_len(queue.queue));
                    printf("Средняя длина очереди: %.3lf\n", simulation_info.average_queue_len / (pseudo_clock / CLOCK_RESOLUTION));
                    printf("Среднее время нахождения заявки в очереди: %.3lf\n", simulation_info.average_task_in_queue_time / simulation_info.processed_tasks);
                }
                    
                dprintf("[%.3lf]: Процессор:\n\tЗавершена задача (время испол.: %.3lf е.в.) после %zu исполнений. Время нахождения в очереди: %.3lf. Выполнено задач: %zu\n\n\n", \
                pseudo_clock, processor->current_task.execution_time, simulation_params.task_repeat, pseudo_clock - processor->current_task.start_time, simulation_info.processed_tasks);
            }
        }
    }

    return ERROR_SUCCESS;
}

// Функция для обновления цикла симуляции
error_codes_e update_simulation(void)
{
    error_codes_e rc;
    
    // Обновление генератора
    if ((rc = update_generator(&generator)) != ERROR_SUCCESS)
        return rc;

    // Обновление процессора
    if ((rc = update_processor(&processor)) != ERROR_SUCCESS)
        return rc;

    // Обновление средней длины очереди
    simulation_info.average_queue_len += queue.get_queue_len(queue.queue);

    // Обновление таймера
    pseudo_clock += CLOCK_RESOLUTION;

    return ERROR_SUCCESS;
}

// Функция запуска симуляции
error_codes_e run_simulation(void *queue_ptr, error_codes_e (*push)(void *, task_t), error_codes_e (*pop)(void *, task_t *), bool (*is_empty)(void *), size_t (*get_queue_len)(void *), simulation_params_t *params)
{
    error_codes_e rc;
    
    // Инициализация параметров симуляции
    init_params(params);

    // Инициализация симуляции
    init_simulation(queue_ptr, push, pop, is_empty, get_queue_len);

    // Обновление цикла симуляции, пока не будет выполнено нужное число задач
    while (simulation_info.processed_tasks < simulation_params.total_tasks_count)
        if ((rc = update_simulation()) != ERROR_SUCCESS)
            return rc;

    // Подсчет необходимых параметров работы симуляции
    simulation_info.total_time = pseudo_clock;
    simulation_info.average_queue_len /= pseudo_clock / CLOCK_RESOLUTION;
    simulation_info.average_task_in_queue_time /= simulation_params.total_tasks_count;

    // Вывод необходимой информации
    printf("Выполнение заняло %.3lf е.в.\n", simulation_info.total_time);
    printf("Процессор простаивал %.3lf е.в.\n", simulation_info.total_processor_idle_time);
    printf("Процессор был запущен %zu раз\n", simulation_info.processor_runs_count);
    printf("Всего задач вошло в очередь: %zu\n", simulation_info.pushed_tasks);
    printf("Всего задач вышло из очереди: %zu\n", simulation_info.processed_tasks);
    printf("Средняя длина очереди: %.3lf\n", simulation_info.average_queue_len);
    printf("Среднее время нахождения заявки в очереди: %.3lf\n", simulation_info.average_task_in_queue_time);

    printf("Погрешность по общему времени выполнения: %.3lf\n", calc_error_total_time());
    printf("Погрешность по числу исполненных задач: %.3lf\n", calc_error_tasks_count());
    printf("Погрешность по сумме времени работы и простоя: %.3lf\n", calc_error_sum_times());

    return ERROR_SUCCESS;
}
