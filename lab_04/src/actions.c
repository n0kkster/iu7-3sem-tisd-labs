#include "actions.h"

error_codes_e handle_push_beg(stack_arr_t *stack)
{
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    double el;

    puts("Введите вещественное число:");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_double(buffer, &el)) != ERROR_SUCCESS)
        return rc;

    if ((rc = push_beg(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_pop_beg(stack_arr_t *stack, double *el)
{
    error_codes_e rc;

    if ((rc = pop_beg(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_peek_beg(stack_arr_t *stack, double *el)
{
    error_codes_e rc;

    if ((rc = peek_beg(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_push_end(stack_arr_t *stack)
{
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    double el;

    puts("Введите вещественное число:");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_double(buffer, &el)) != ERROR_SUCCESS)
        return rc;

    if ((rc = push_end(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_pop_end(stack_arr_t *stack, double *el)
{
    error_codes_e rc;

    if ((rc = pop_end(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_peek_end(stack_arr_t *stack, double *el)
{
    error_codes_e rc;

    if ((rc = peek_end(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

void handle_print_arr_beg(stack_arr_t *stack)
{
    if (stack->sp1 == -1)
        printf(RED"Стек пуст!"RESET);

    printf(GREEN);
    for (long i = stack->sp1; i > -1; i--)
    {
        printf("%lf ", stack->arr[i]);
    }
    printf("\n");
    printf(RESET);
}

void handle_print_arr_end(stack_arr_t *stack)
{
    if (stack->sp2 == STACK_DEPTH)
        printf(RED"Стек пуст!"RESET);

    printf(GREEN);
    for (long i = stack->sp2; i < STACK_DEPTH; i++)
    {
        printf("%lf ", stack->arr[i]);
    }
    printf("\n");
    printf(RESET);
}

error_codes_e handle_push_list(list_node **stack)
{
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    double el;

    puts("Введите вещественное число:");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = validate_double(buffer, &el)) != ERROR_SUCCESS)
        return rc;

    if ((rc = push(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_pop_list(list_node **stack, list_node **deleted, double *el)
{
    error_codes_e rc;

    if ((rc = pop(stack, deleted, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

error_codes_e handle_peek_list(list_node *stack, double *el)
{
    error_codes_e rc;

    if ((rc = peek(stack, el)) != ERROR_SUCCESS)
        return rc;
    
    return ERROR_SUCCESS;
}

void handle_print_list_stack(list_node *stack)
{
    if (!stack)
    {
        printf(RED"Стек пуст!\n"RESET);
        return;
    }
    printf(GREEN);
    while (stack)
    {
        printf("%lf (%p); ", stack->data, (void *)stack);
        stack = stack->next;
    }
    printf("\n"RESET);
}

void handle_print_deleted(list_node *deleted)
{
    if (!deleted)
    {
        printf(RED"Список удаленных пуст!\n"RESET);
        return;
    }
    printf(GREEN);

    while (deleted)
    {
        printf("%p\n", (void *)deleted);
        deleted = deleted->next;
    }
    printf(RESET);
}

    error_codes_e validate_double(char *buffer, double *el)
    {
        char *ep;

        *el = strtod(buffer, &ep);
        if (*ep != '\0')
            return ERROR_PARSING_NUMBER; 

        return ERROR_SUCCESS;
    }

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

size_t calc_size(list_node *stack_list)
{
    size_t sz = 0;
    while (stack_list)
    {
        sz += sizeof(*stack_list);
        stack_list = stack_list->next;
    }
    return sz;
}

void handle_speedtest()
{
    stack_arr_t stack_arr;
    list_node *stack_list = NULL, *deleted = NULL;
    double el;

    stack_arr.sp1 = -1;
    stack_arr.sp2 = STACK_DEPTH;

    unsigned long beg, end;
    double push_arr_time, pop_arr_time, push_list_time, pop_list_time;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        push_beg(&stack_arr, 999);
    end = micros();
    push_arr_time = (end - beg) / (double)MEASURES;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        pop_beg(&stack_arr, &el);
    end = micros();
    pop_arr_time = (end - beg) / (double)MEASURES;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        push(&stack_list, 666);
    end = micros();
    push_list_time = (end - beg) / (double)MEASURES;

    beg = micros();
    for (size_t i = 0; i < MEASURES; i++)
        pop(&stack_list, &deleted, &el);
    end = micros();
    pop_list_time = (end - beg) / (double)MEASURES;

    printf("push arr: %lf, pop arr: %lf, push list: %lf, pop list: %lf\n", push_arr_time, pop_arr_time, push_list_time, pop_list_time);

    free_list(stack_list);
    free_list(deleted);

    stack_list = NULL;
    stack_arr.sp1 = -1;
    stack_arr.sp2 = STACK_DEPTH;

    for (size_t size = 10; size < STACK_DEPTH + 1; size += 10)
    {
        for (size_t i = 0; i < size; i++)
            push(&stack_list, 666);
        for (size_t i = 0; i < size; i++)
            push_beg(&stack_arr, 666);
        
        printf("%zu\t%zu\t%zu\t%.4f\n", size, calc_size(stack_list), sizeof(stack_arr), (float)sizeof(stack_arr) / calc_size(stack_list));
        free_list(stack_list);
        stack_list = NULL;
        stack_arr.sp1 = -1;
        stack_arr.sp2 = STACK_DEPTH;
    }
}