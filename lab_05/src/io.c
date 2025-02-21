#include "io.h"

bool verbose = false, mem_verbose = false;

void print_menu(void)
{
    printf(BLUE);
    puts("----------------------------------------------------");
    puts("Программа для симуляции пула задач с использованием очереди");
    puts("1. Изменить параметры симуляции.");
    puts("2. Запустить \"реальную\" симуляцию с очередью на массиве.");
    puts("3. Запустить \"реальную\" симуляцию с очередью на списке.");
    puts("4. Запустить обычную симуляцию с очередью на массиве.");
    puts("5. Запустить обычную симуляцию с очередью на списке.");
    puts("6. Вкл./выкл. подробный режим.");
    puts("7. Вкл./Выкл. вывод информации об использованной памяти (только для очереди на списке).");
    puts("8. Провести замеры времени и памяти.");
    puts("0. Выйти из программы.");
    puts("----------------------------------------------------");
    printf(RESET);
}

void toggle_verbose(void)
{
    verbose = !verbose;
}

void toggle_mem_verbose(void)
{
    mem_verbose = !mem_verbose;
}

error_codes_e read_string(FILE *f, char *buffer, size_t size)
{
    // Чтение ввода пользователя
    if (!fgets(buffer, size, f))
        return ERROR_READING_MENU;  // Ошибка при чтении меню

    // Удаление символа новой строки
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Проверка длины строки
    if (strlen(buffer) > MAX_STRING_SIZE)
        return ERROR_STRING_TOO_LONG;  // Ошибка при слишком длинной строке

    if (strlen(buffer) == 0)
        return ERROR_READING_MENU;  // Ошибка при пустой строке

    return ERROR_SUCCESS;
}

error_codes_e read_menu(action_e *action)
{
    char buffer[MAX_STRING_SIZE + 2];  // Буфер для хранения ввода
    char *ep;  // Указатель на символ, не удавшийся для преобразования
    long num;
    error_codes_e rc;

    if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;

    // Преобразование строки в целое число (выбор действия)
    num = strtol(buffer, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER;  // Ошибка при преобразовании числа

    if (num < 0 || num >= ACTION_COUNT)
        return ERROR_INCORRECT_ACTION;
    
    *action = (action_e)num;

    return ERROR_SUCCESS;  // Успешное завершение
}
