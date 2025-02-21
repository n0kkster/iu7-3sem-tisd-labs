#include "calculations.h"

// Функция умножения двух действительных чисел
error_codes_e multiply_real(real_t *real1, real_t *real2, real_t *result)
{
    uint8_t shift = 0;  // Сдвиг для хранения промежуточных результатов умножения
    uint8_t i = MAX_MANTISSA_SIZE - 1;  // Индекс для контроля переполнений мантиссы

    // Разворот мантисс для удобства умножения
    reverse_mantissa(&real1->mantissa);
    reverse_mantissa(&real2->mantissa);

    // Приведение чисел к нормализованной форме
    rev_normalize(real1);
    rev_normalize(real2);

    // Основной цикл умножения мантисс
    for (uint8_t i = 0; i < real2->mantissa.size; i++)
    {
        for (uint8_t j = 0; j < real1->mantissa.size; j++)
            result->mantissa.digits[j + shift] += real2->mantissa.digits[i] * real1->mantissa.digits[j]; 
        shift++;  // Сдвиг индексов для корректного сложения результатов
    }

    // Установка размера мантиссы результата
    result->mantissa.size = real1->mantissa.size + shift - 1;
    shift_digits(&result->mantissa);  // Корректировка цифр мантиссы

    // Проверка на переполнение и перенос значений
    while (result->mantissa.digits[result->mantissa.size - 1] > BASE)
    {
        result->mantissa.digits[result->mantissa.size] = result->mantissa.digits[result->mantissa.size - 1] / BASE;
        result->mantissa.digits[result->mantissa.size - 1] %= BASE;
        result->mantissa.size++;
    }

    // Разворот мантиссы обратно
    reverse_mantissa(&result->mantissa);

    // Вычисление нового порядка (экспоненты)
    result->exponent = real1->exponent + real2->exponent;
    // Определение знака результата
    result->mantissa_sign = real1->mantissa_sign == real2->mantissa_sign;
    // Обновление позиции десятичной точки
    result->mantissa.dot_index = result->mantissa.size;

    normalize(result);  // Нормализация результата

    // Проверка на превышение максимального размера мантиссы
    if (result->mantissa.size > MAX_MANTISSA_SIZE)
    {
        result->mantissa.size = MAX_MANTISSA_SIZE;
        // Округление мантиссы, если следующая цифра >= 5
        if (result->mantissa.digits[MAX_MANTISSA_SIZE] >= 5)
        {
            result->mantissa.digits[i]++;
            // Переносы при округлении
            while (result->mantissa.digits[i] >= BASE)
            {
                if (i == 0)
                {
                    result->exponent += get_denom(result->mantissa.digits[0]);
                    result->mantissa.digits[i] /= BASE * get_denom(result->mantissa.digits[0]);
                    break;
                }
                result->mantissa.digits[i - 1] += result->mantissa.digits[i] / BASE;
                result->mantissa.digits[i] %= BASE;
                i--;
            }
        }
    }

    // Удаление конечных нулей из мантиссы
    i = result->mantissa.size - 1;
    while (result->mantissa.digits[i] == 0)
    {
        result->mantissa.size--;
        i--;
    }

    // Проверка на превышение допустимого значения экспоненты
    if (abs(result->exponent) > MAX_EXPONENT_VAL)
        return ERROR_EXPONENT_TOO_BIG;

    return ERROR_SUCCESS;
}

// Функция для переноса значений мантиссы при переполнении
void shift_digits(mantissa_t *mantissa)
{
    for (uint8_t i = 0; i < mantissa->size - 1; i++)
    {
        mantissa->digits[i + 1] += mantissa->digits[i] / BASE;
        mantissa->digits[i] %= BASE;
    }
}

// Функция для получения знаменателя числа
int get_denom(int x)
{
    int denom = 0;
    while (x >= BASE)
        x /= BASE * ++denom;
    return denom;
}

// Функция для разворота мантиссы
void reverse_mantissa(mantissa_t *mantissa)
{
    for (size_t i = 0; i < mantissa->size / 2; i++)
        XORSWAP(mantissa->digits[i], mantissa->digits[mantissa->size - 1 - i]);
}

// Удаление цифры мантиссы по индексу
void remove_digit(mantissa_t *mantissa, size_t index)
{
    while (index < (uint8_t)(mantissa->size - 1))
    {
        mantissa->digits[index] = mantissa->digits[index + 1];
        index++;
    }
    mantissa->size--;
}

// Функция нормализации числа (приведение к стандартной форме)
void normalize(real_t *real)
{
    size_t i = 0;
    // Удаление ведущих нулей
    while (real->mantissa.digits[i] == 0)
    {
        real->exponent--;
        remove_digit(&real->mantissa, i);
    }

    // Коррекция экспоненты с учетом позиции десятичной точки
    real->exponent += real->mantissa.dot_index;
    real->mantissa.dot_index = 0;
}

// Функция обратной нормализации для подготовки к вычислениям
void rev_normalize(real_t *real)
{
    real->mantissa.dot_index = real->mantissa.size;  // Обновление индекса десятичной точки
    real->exponent -= real->mantissa.size;  // Коррекция экспоненты
}

// Проверка, является ли число нулем
bool is_zero(real_t *real)
{
    for (int8_t i = 0; i < real->mantissa.size; i++)
        if (real->mantissa.digits[i] != 0)
            return false;
    return true;
}
