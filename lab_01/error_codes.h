#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <stdio.h>

// Перечисление кодов ошибок, которые могут возникнуть в программе
typedef enum error_codes
{
    ERROR_SUCCESS,         // Ошибок нет, успешное выполнение
    ERROR_READING_DATA,    // Ошибка чтения данных
    ERROR_INVALID_DATA,    // Некорректный формат введенных данных
    ERROR_EXPONENT_TOO_BIG,// Экспонента превышает допустимое значение
    ERROR_MANTISSA_TOO_BIG // Мантисса превышает допустимый размер
} error_codes_e;

// Функция для обработки ошибок, выводит сообщение об ошибке на экран
error_codes_e handle_error(error_codes_e rc);

#endif
