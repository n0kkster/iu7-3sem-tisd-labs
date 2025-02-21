#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "error_codes.h"
#include "types.h"

// Максимальный размер буфера для чтения ввода
#define MAX_BUFFER_SIZE 128

// Функция для чтения вещественного числа с консоли
error_codes_e read_real(real_t *real, uint8_t max_mantissa_size);

// Функция для чтения мантиссы из строки
error_codes_e read_mantiss(real_t *real, uint8_t max_mantissa_size, char *buffer);

// Функция для чтения экспоненты из строки
error_codes_e read_exponent(real_t *real, char *buffer);

// Функция вывода инструкции для ввода вещественного числа
void ask_for_number(uint8_t mant_size);

// Функция для вывода вещественного числа на экран
void print_real(real_t *real);

#endif
