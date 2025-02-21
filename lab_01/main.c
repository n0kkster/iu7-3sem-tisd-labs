#include <stdio.h>
#include "error_codes.h"
#include "types.h"
#include "io.h"
#include "calculations.h"

int main(void)
{
    // Определение переменных для хранения действительных чисел и результата
    real_t real1 = { 0 }, real2 = { 0 }, result = { 0 };
    // Переменная для хранения кода ошибки
    error_codes_e rc = ERROR_SUCCESS;

    // Вывод информации о программе
    printf("Программа выполняет операцию умножения действительного числа на действительное число.\n");
    printf("Длины мантиссы первого сомножителя не превышает 35 цифр, второго - 40 цифр. Длина порядка не превышает 5 цифр.\n");
    
    // ====== Ввод первого действительного числа ======
    rc = read_real(&real1, MANT1_SIZE);  // Ввод первого числа с ограничением на длину мантиссы

    // Обработка возможной ошибки при вводе первого числа
    if (handle_error(rc))
        return rc;  // Завершение программы, если была ошибка
    // =================================================


    // ====== Ввод второго действительного числа ======
    rc = read_real(&real2, MAX_MANTISSA_SIZE);  // Ввод второго числа с ограничением на длину мантиссы

    // Обработка возможной ошибки при вводе второго числа
    if (handle_error(rc))
        return rc;  // Завершение программы, если была ошибка
    // =================================================

    // Проверка, является ли одно из чисел нулем
    if (is_zero(&real1) || is_zero(&real2))
    {
        printf("+0e0\n");  // Если одно из чисел равно нулю, результат - 0
        return ERROR_SUCCESS;  // Завершение программы с успешным кодом
    }

    // Нормализация чисел
    normalize(&real1);
    normalize(&real2);

    // Умножение двух действительных чисел
    rc = multiply_real(&real1, &real2, &result);
    
    // Обработка возможной ошибки при умножении
    if (handle_error(rc))
        return rc;  // Завершение программы, если была ошибка

    // Вывод результата умножения
    print_real(&result);

    return rc;  // Завершение программы с кодом завершения
}
