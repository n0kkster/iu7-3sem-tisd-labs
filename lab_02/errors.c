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
            puts("Длина считанной строки преывышает максимальную!");
            break;
        case ERROR_FILE_IS_NULL:
            puts("Файл не существует!");
            break;
        case ERROR_READING_FILE:
            puts("Ошибка чтения данных из файла!");
            break;
        case ERROR_EMPTY_STRING:
            puts("Считана пустая строка!");
            break;
        case ERROR_PARSING_NUMBER:
            puts("Ошибка чтения числа из строки!");
            break;
        case ERROR_OUT_OF_RANGE:
            puts("Введенное число не входит в указанный диапазон!");
            break;
        case ERROR_OPENING_FILE:
            puts("Файл не может быть открыт!");
            break;
        case ERROR_FILE_IS_EMPTY:
            puts("Файл пуст!");
            break;
        case ERROR_TOO_MANY_ENTRIES:
            puts("В файле слишком много записей!");
            break;
        case ERROR_INVALID_STRUCT:
            puts("Обнаружена некорректная структура!");
            break;  
        case ERROR_SUCCESS:
            break;
    }
    printf(RESET);
    return rc;
}