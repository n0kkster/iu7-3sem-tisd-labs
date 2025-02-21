#include "actions.h"

error_codes_e handle_read_from_file(car_t cars[], key_t keys[], size_t *read_count)
{
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    size_t cars_count;

    printf("Введите имя файла: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = get_structs_count(buffer, &cars_count)) != ERROR_SUCCESS)
        return rc;

    if (cars_count > MAX_CARS_COUNT)
        return ERROR_TOO_MANY_ENTRIES;

    if ((rc = read_table_from_file(buffer, cars, read_count)) != ERROR_SUCCESS)
        return rc;

    for (size_t i = 0; i < *read_count; i++)
    {
        keys[i].table_index = i;
        keys[i].price = cars[i].price;
    }

    printf("Считано %zu записей.\n", *read_count);

    return rc;
}

error_codes_e handle_find(car_t cars[], size_t count)
{
    char buffer[MAX_STRING_SIZE + 2];
    unsigned int min_price, max_price;
    error_codes_e rc;

    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return ERROR_SUCCESS;
    }

    printf("Введите минимальную цену: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;
    if ((rc = parse_uint32(buffer, &min_price)) != ERROR_SUCCESS)
        return rc;

    printf("Введите максимальную цену: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;
    if ((rc = parse_uint32(buffer, &max_price)) != ERROR_SUCCESS)
        return rc;

    printf("Введите марку, по которой будет произведен поиск: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    while (count--)
    {
        if (strcmp(cars[count].brand, buffer) == 0 && \
            cars[count].condition == USED && \
            cars[count].price >= min_price && \
            cars[count].price <= max_price && \
            cars[count].data.used.owners_count == 1 && \
            cars[count].data.used.repairs_count == 0)
        {
            print_struct(&cars[count]);
            if (strcmp(cars[count].brand, "Russia") != 0 && \
                strcmp(cars[count].brand, "USSR") != 0 && \
                cars[count].can_be_maintained)
                puts("Данный автомобиль может быть обслужен у дилера.");
        }
    }

    return ERROR_SUCCESS;
}

void handle_sort_records(car_t cars[], size_t count)
{
    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return;
    }

    if (count == 1)
    {
        puts("Таблица содержит одну запись!");
        return;
    }

    sort_records(cars, count);
    print_table(cars, count);
}

void sort_records(car_t cars[], size_t count)
{
    car_t car;

    for (size_t i = 0; i < count - 1; i++)
    {
        for (size_t j = 0; j < count - i - 1; j++)
        {
            if (cars[j].price > cars[j + 1].price)
            {
                car = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = car;
            }
        }
    }
}

void handle_sort_keys(key_t keys[], size_t count)
{
    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return;
    }

    if (count == 1)
    {
        puts("Таблица содержит одну запись!");
        return;
    }


    sort_keys(keys, count);
    print_key_table(keys, count);
}

void sort_keys(key_t keys[], size_t count)
{
    key_t key;
    
    for (size_t i = 0; i < count - 1; i++)
    {
        for (size_t j = 0; j < count - i - 1; j++)
        {
            if (keys[j].price > keys[j + 1].price)
            {
                key = keys[j];
                keys[j] = keys[j + 1];
                keys[j + 1] = key;
            }
        }
    }
}

void handle_print_by_keys(car_t cars[], key_t keys[], size_t count)
{
    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return;
    }

    for (size_t i = 0; i < count; i++)
        print_struct(&cars[keys[i].table_index]);
}

error_codes_e handle_add_record(car_t cars[], key_t keys[], size_t *count)
{
    error_codes_e rc;
    car_t car;
    key_t key;
    char buffer[MAX_STRING_SIZE + 2];

    if (*count >= MAX_CARS_COUNT)
    {
        puts("Таблица содержит максимальное количество структур!");
        return ERROR_SUCCESS;
    }

    printf("Все вводимые строки не должны быть длинее %d символов!\n", MAX_STRING_SIZE);
    printf("Все вводимые числа должны входить в диапазон от 0 до %u!\n", __UINT32_MAX__);
    printf("Введите марку автомобиля: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc == ERROR_READING_FILE ? ERROR_FILE_IS_EMPTY : rc;
    strcpy(car.brand, buffer);

    printf("Введите страну производства автомобиля: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    strcpy(car.origin, buffer);

    printf("Может ли автомобиль быть обслужен (yes/no)?: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    string_tolower(buffer);
    car.can_be_maintained = strcmp(buffer, "yes") == 0;

    printf("Введите цену автомобиля: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint32(buffer, &car.price)))
        return rc;

    printf("Введите цвет автомобиля: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    strcpy(car.color, buffer);
    
    printf("Автомобиль новый или БУ (new/used)?: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    string_tolower(buffer);
    car.condition = (condition_e)(strcmp(buffer, "used") == 0);

    if (car.condition == NEW)
    {   
        printf("Введите гарантию в годах: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint8(buffer, &car.data.new.warranty)))
            return rc;
    }
    else
    {
        printf("Введите год выпуска автомобиля: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint32(buffer, &car.data.used.year)))
            return rc;

        printf("Введите пробег автомобиля: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint32(buffer, &car.data.used.mileage)))
            return rc;

        printf("Введите количество владельцев автомобиля: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint8(buffer, &car.data.used.owners_count)))
            return rc;
        
        printf("Введите количество ремонтов автомобиля: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint8(buffer, &car.data.used.repairs_count)))
            return rc;
    }

    key.price = car.price;
    key.table_index = *count;

    cars[*count] = car;
    keys[*count] = key;

    *count += 1;
    
    printf("Введите имя файла: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    if ((rc = write_table_to_file(buffer, cars, *count)) != ERROR_SUCCESS)
        return rc;

    return ERROR_SUCCESS; 
}

error_codes_e handle_delete_record(car_t cars[], key_t keys[], size_t *count)
{
    error_codes_e rc;
    char buffer[MAX_STRING_SIZE + 2];
    unsigned int price;
    size_t index, key_index = 0;
    bool found = false;

    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return ERROR_SUCCESS;
    }

    printf("Все вводимые числа должны входить в диапазон от 0 до %u!\n", __UINT32_MAX__);
    printf("Введите цену автомобиля: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint32(buffer, &price)))
        return rc;

    for (index = 0; index < *count; index++)
    {
        if (cars[index].price == price)
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        while (keys[key_index].table_index != index) key_index++;
        
        index++;
        key_index++;

        while (index < *count)
        {
            cars[index - 1] = cars[index];
            index++;
        }

        while (key_index < *count)
        {
            keys[key_index - 1] = keys[key_index];
            key_index++;
        }
        *count -= 1;

        printf("Введите имя файла: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
            return rc;

        if ((rc = write_table_to_file(buffer, cars, *count)) != ERROR_SUCCESS)
            return rc;
    }

    return ERROR_SUCCESS;
}

void shuffle(void *array, size_t n, size_t size) 
{
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; ++i) 
        {
            size_t rnd = (size_t)rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

int comparator_cars(const void *a, const void *b)
{   
    return (int)((int)(((car_t *)a)->price) - (int)(((car_t *)b)->price));
}

int comparator_keys(const void *a, const void *b)
{   
    return (int)((int)(((key_t *)a)->price) - (int)(((key_t *)b)->price));
}

unsigned long long microseconds_now(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

error_codes_e handle_collect_data(car_t cars[], key_t keys[], size_t count)
{
    unsigned int measures;
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;
    unsigned long long begin, end;
    unsigned long long table_time_bubble = 0, table_time_qsort = 0, key_time_bubble = 0, key_time_qsort = 0; 

    if (count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);
        return ERROR_SUCCESS;
    }

    printf("Введите количество замеров: ");
    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;
    if ((rc = parse_uint32(buffer, &measures)) != ERROR_SUCCESS)
        return rc;

    for (size_t i = 0; i < measures; i++)
    {
        shuffle(cars, count, sizeof(car_t));
        begin = microseconds_now();
        sort_records(cars, count);
        end = microseconds_now();
        table_time_bubble += end - begin;
    }
    table_time_bubble /= measures;

    for (size_t i = 0; i < measures; i++)
    {
        shuffle(keys, count, sizeof(key_t));
        begin = microseconds_now();
        sort_keys(keys, count);
        end = microseconds_now();
        key_time_bubble += end - begin;
    }
    key_time_bubble /= measures;

    for (size_t i = 0; i < measures; i++)
    {
        shuffle(keys, count, sizeof(key_t));
        begin = microseconds_now();
        qsort(keys, count, sizeof(key_t), comparator_keys);
        end = microseconds_now();
        key_time_qsort += end - begin;
    }
    key_time_qsort /= measures;

    for (size_t i = 0; i < measures; i++)
    {
        shuffle(cars, count, sizeof(car_t));
        begin = microseconds_now();
        qsort(cars, count, sizeof(car_t), comparator_cars);
        end = microseconds_now();
        table_time_qsort += end - begin;
    }
    table_time_qsort /= measures;

    printf(YELLOW"-----------------------------------------------------------------------------\n");
    printf("|\x1b[15G|Таблица записей, мкс\x1b[45G|Таблица ключей, мкс\x1b[65G|Прирост, %%\x1b[77G|\n");
    printf("|bubble sort\x1b[15G|%llu\x1b[45G|%llu\x1b[65G|%f\x1b[77G|\n", table_time_bubble, key_time_bubble, (table_time_bubble / (float)key_time_bubble - 1) * 100);
    printf("|qsort\x1b[15G|%llu\x1b[45G|%llu\x1b[65G|%f\x1b[77G|\n", table_time_qsort, key_time_qsort, (table_time_qsort / (float)key_time_qsort - 1) * 100);
    printf(YELLOW"-----------------------------------------------------------------------------\n"RESET);

    printf(YELLOW"----------------------------------------------------------------------\n");
    printf("|Таблица записей, байт\x1b[30G|Таблица ключей, байт\x1b[55G|Разница, раз\x1b[70G|\n");
    printf("|%zu\x1B[30G|%zu\x1B[55G|%f\x1B[70G|\n", sizeof(car_t) * count, sizeof(key_t) * count + sizeof(car_t) * count, (sizeof(key_t) + sizeof(car_t)) / (float)sizeof(car_t));
    printf(YELLOW"----------------------------------------------------------------------\n");

    return ERROR_SUCCESS;
}
