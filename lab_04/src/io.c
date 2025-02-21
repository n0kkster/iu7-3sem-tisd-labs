#include "io.h"

void print_menu(void)
{
    printf(BLUE);
    puts("----------------------------------------------------");
    puts("Программа для работы со стеком в виде массива и односвязного списка. В одном массиве хранится 2 стека, один с начала, второй с конца.");
    puts("1. Добавить элемент в стек в начале (массив).");
    puts("2. Удалить элемент из стека в начале (массив).");
    puts("3. Просмотреть верхний элемент стека в начале (массив).");
    puts("4. Вывести содержимое стека в начале (массив).");
    puts("5. Добавить элемент в стек в конце (массив).");
    puts("6. Удалить элемент из стека в конце (массив).");
    puts("7. Просмотреть верхний элемент стека в конце (массив).");
    puts("8. Вывести содержимое стека в конце (массив).");
    puts("9. Добавить элемент в стек (список).");
    puts("10. Удалить элемент из стека (список).");
    puts("11. Просмотреть верхний элемент стека (список).");
    puts("12. Вывести содержимое стека (список).");
    puts("13. Вывести удаленные элементы стека (список).");
    puts("14. Провести замеры времени.");
    puts("0. Выйти из программы.");
    puts("----------------------------------------------------");
    printf(RESET);
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