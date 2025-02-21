#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "io.h"

// Макрос для обмена значениями двух переменных с использованием XOR-операции
#define XORSWAP(a, b) do {(a) ^= (b); (b) ^= (a); (a) ^= (b);} while (0)

// Основа для мантиссы (десятичная система)
#define BASE 10

// Функция умножения двух действительных чисел
error_codes_e multiply_real(real_t *real1, real_t *real2, real_t *result);

// Функция нормализации действительного числа (устранение ведущих нулей, корректировка порядка)
void normalize(real_t *real);

// Функция удаления цифры из мантиссы по заданному индексу
void remove_digit(mantissa_t *mantissa, size_t index);

// Функция переворота мантиссы (для работы с числами справа налево)
void reverse_mantissa(mantissa_t *mantissa);

// Функция сдвига цифр мантиссы для учета переносов между разрядами
void shift_digits(mantissa_t *mantissa);

// Функция обратной нормализации (для подготовки числа к операциям умножения)
void rev_normalize(real_t *real);

// Функция вычисления значения знаменателя для масштабирования мантиссы
int get_denom(int x);

// Функция проверки, является ли число нулем (проверка всех цифр мантиссы)
bool is_zero(real_t *real);

#endif
