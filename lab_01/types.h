#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Определение максимального размера мантиссы (до 40 цифр)
#define MAX_MANTISSA_SIZE 40

// Максимальное допустимое значение экспоненты
#define MAX_EXPONENT_VAL 99999

// Размер мантиссы для первого числа (до 35 цифр)
#define MANT1_SIZE 35

// Структура для хранения мантиссы
#pragma pack(push, 1)
typedef struct mantissa
{
    int digits[MAX_MANTISSA_SIZE + MANT1_SIZE + 1];  // Массив для хранения цифр мантиссы
    uint8_t size;    // Текущий размер мантиссы (количество значащих цифр)
    int8_t dot_index;  // Индекс десятичной точки в мантиссе
} mantissa_t;
#pragma pack(pop)

// Структура для хранения вещественного числа
#pragma pack(push, 1)
typedef struct real
{
    bool mantissa_sign;  // Знак мантиссы (true — положительное число, false — отрицательное)
    mantissa_t mantissa; // Мантисса числа
    int exponent;        // Экспонента числа
} real_t;
#pragma pack(pop)

#endif
