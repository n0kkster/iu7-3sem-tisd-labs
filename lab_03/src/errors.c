#include "errors.h"

error_codes_e handle_error(error_codes_e rc)
{
    printf(RED);
    switch (rc)
    {
        case ERROR_READING_MENU:
            puts("Ошибка чтения пункта меню!");
            break;
        case ERROR_STRING_TOO_LONG:
            puts("Длина считанной строки превышает максимальную!");
            break;
        case ERROR_PARSING_NUMBER:
            puts("Ошибка чтения числа из строки!");
            break;
        case ERROR_INVALID_MENU_NUM:
            puts("Введен неверный пункт меню!");
            break;
        case ERROR_READING_FILE:
            puts("Ошибка чтения файла!");
            break;
        case ERROR_EMPTY_STRING:
            puts("Считана пустая строка!");
            break;
        case ERROR_EMPTY_FILE:
            puts("Файл пуст!");
            break;
        case ERROR_INCORRECT_FILE_PTR:
            puts("Некорректный указатель на файл!");
            break;
        case ERROR_OPENING_FILE:
            puts("Невозможно открыть файл!");
            break;
        case ERROR_ALLOCATING_MEM:
            puts("Невозможно выделить память!");
            break;
        case ERROR_INVALID_DATA:
            puts("Были обнаружены некорректные данные!");
            break;
        case ERROR_SUCCESS:
            break;
    }
    printf(RESET);
    return rc;
}
