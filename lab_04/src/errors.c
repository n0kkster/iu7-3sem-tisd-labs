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
        case ERROR_INCORRECT_ACTION:
            puts("Введен неверный пункт меню!");
            break;
        case ERROR_ALLOCATING_MEM:
            puts("Невозможно выделить память!");
            break;
        case ERROR_STACK_OVERFLOW:
            puts("Стек переполнен!");
            break;
        case ERROR_EMPTY_STACK:
            puts("Стек пуст!");
            break;
        case ERROR_SUCCESS:
            break;
    }
    printf(RESET);
    return rc;
}
