#include "errors.h"

error_codes_e handle_error(error_codes_e rc)
{
    printf(RED);
    switch (rc)
    {
        case ERROR_READING_MENU:
            puts("Ошибка чтения пункта меню!");
            break;
        case ERROR_PARSING_NUMBER:
            puts("Ошибка чтения числа из строки!");
            break;
        case ERROR_INCORRECT_ACTION:
            puts("Введен неверный пункт меню!");
            break;
        case ERROR_ALLOCATING_MEM:
            puts("Невозможно выделить память!");
            break;
        case ERROR_OPENING_FILE:
            puts("Ошибка открытия входного файла!");
            break;
        case ERROR_READING_FILE:
            puts("Ошибка чтения данных из файла!");
            break;
        case ERROR_READING_STRING:
            puts("Ошибка чтения строки!");
            break;
        case ERROR_EMPTY_STRING:
            puts("Введена пустая строка!");
            break;
        case ERROR_SUCCESS:
            break;
    }
    printf(RESET);
    return rc;
}
