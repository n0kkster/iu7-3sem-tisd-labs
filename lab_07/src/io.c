#include "io.h"

void print_menu(void)
{
    printf(BLUE);
    puts("----------------------------------------------------");
    puts("Программа для работы с бинарным деревом поиска, АВЛ деревом и хеш таблицей.");
    puts("1. Считать данные из файла.");
    puts("2. Построить АВЛ дерево по ДДП.");
    puts("3. Вывести структуру на экран.");
    puts("4. Вставить элемент в структуру.");
    puts("5. Удалить элемент из структуры.");
    puts("6. Найти элемент в структуре.");
    puts("7. Заполнить файл случайными числами.");
    puts("8. Измерить время операций.");
    puts("9. Нарисовать структуру в виде графа.");
    puts("10. Переключить хеширование в хеш таблице.");
    puts("0. Выйти из программы.");
    puts("----------------------------------------------------");
    printf(RESET);
}

error_codes_e read_data_choice(data_type_e *dt)
{
    char *buffer;
    long num;
    error_codes_e rc;

    printf(BLUE);
    puts("----------------------------------------------------");
    puts("Введите номер типа данных, с которым необходимо произвести действие:");
    puts("1. Двоичное дерево поиска.");
    puts("2. АВЛ дерево.");
    puts("3. Хеш-таблица.");
    puts("----------------------------------------------------");
    printf(RESET);

    if ((rc = read_string(stdin, &buffer)) != ERROR_SUCCESS)
        return rc;

    ENSURE(validate_long(buffer, &num), free(buffer););
    free(buffer);

    if (num > HASHTABLE)
        return ERROR_INCORRECT_ACTION;

    *dt = (data_type_e)num;
    return ERROR_SUCCESS;
}


error_codes_e read_string(FILE *f, char **buffer)
{
    size_t n = 0;
    *buffer = NULL;

    if (getline(buffer, &n, f) == -1)
    {
        free(*buffer);
        return ERROR_READING_STRING;
    }

    (*buffer)[strcspn(*buffer, "\n")] = '\0';  // Удаляем символ новой строки в конце.

    n = strlen(*buffer);

    if (n == 0)
    {
        free(*buffer);
        return ERROR_EMPTY_STRING;  // Если строка пустая, возвращаем ошибку пустой строки.
    }

    *buffer = realloc(*buffer, n + 1);

    if (*buffer == NULL)
        return ERROR_ALLOCATING_MEM;

    return ERROR_SUCCESS;  // Возвращаем успешный код.
}

error_codes_e read_menu(action_e *action)
{
    char *buffer;
    long num;
    error_codes_e rc;

    if ((rc = read_string(stdin, &buffer)) != ERROR_SUCCESS)
        return rc;

    ENSURE(validate_long(buffer, &num), free(buffer););
    free(buffer);
    
    if (num < 0 || num >= ACTION_COUNT)
        return ERROR_INCORRECT_ACTION;

    *action = (action_e)num;

    return ERROR_SUCCESS;
}

error_codes_e validate_long(char *buffer, long *num)
{
    char *ep;

    *num = strtol(buffer, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER; 
    
    return ERROR_SUCCESS;
}
