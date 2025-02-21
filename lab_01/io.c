#include "io.h"

// Функция для запроса ввода действительного числа
void ask_for_number(uint8_t mant_size)
{
    // Вывод инструкций для пользователя
    printf("Введите действительное число в формате [+-]m.n[Ee][+-]K; m + n <= %u, |K| <= 99999:\n", mant_size);
    printf("+");
    // Вывод шаблона для указания длины мантиссы
    for (int i = 1; i < mant_size + 1; i++)
    {
        if (i % 10 == 0)
            printf("%d", i / 10);  // Печать номера десятков
        else
            printf("-");  // Печать разделителя
    }
    printf("\n");
}

// Функция для чтения действительного числа
error_codes_e read_real(real_t *real, uint8_t max_mantissa_size)
{
    char buffer[MAX_BUFFER_SIZE];  // Буфер для ввода
    size_t buf_len;
    size_t e_index = 0;
    error_codes_e rc;

    ask_for_number(max_mantissa_size);  // Запрос ввода у пользователя

    // Чтение строки с входными данными
    if (!fgets(buffer, MAX_BUFFER_SIZE, stdin))
        return ERROR_READING_DATA;

    buffer[strcspn(buffer, "\n")] = '\0';  // Удаление символа новой строки
    buf_len = strlen(buffer);

    // Поиск позиции экспоненты (e или E)
    e_index = strcspn(buffer, "e");
    if (e_index == strlen(buffer))
        e_index = strcspn(buffer, "E");

    buffer[e_index] = '\0';  // Разделение строки на мантиссу и экспоненту
    
    // Чтение мантиссы
    if ((rc = read_mantiss(real, max_mantissa_size, buffer)) != ERROR_SUCCESS)
        return rc;

    // Если экспоненты нет, установить ее в 0
    if (e_index == buf_len)
        real->exponent = 0;
    else
    {
        // Чтение экспоненты
        if ((rc = read_exponent(real, buffer + e_index + 1)) != ERROR_SUCCESS)
            return rc;   
    }

    return rc;
}

// Функция для чтения мантиссы
error_codes_e read_mantiss(real_t *real, uint8_t max_mantissa_size, char *buffer)
{
    uint8_t mant_size = 0;
    int i = 0;
    bool has_dot = false;  // Флаг наличия десятичной точки
    int has_sign = 0;  // Флаг наличия знака числа

    // Определение знака числа
    if (buffer[i] == '+' || isdigit(buffer[i]))
        real->mantissa_sign = 1;  // Положительное число
    else if (buffer[i] == '-')
        real->mantissa_sign = 0;  // Отрицательное число
    else
        return ERROR_INVALID_DATA;  // Ошибка при неверном вводе

    // Пропуск знака в строке
    if (buffer[i] == '+' || buffer[i] == '-')
    {
        i++;
        has_sign = 1;
    } 

    real->mantissa.dot_index = -1;  // Инициализация позиции десятичной точки

    // Цикл чтения цифр мантиссы
    while (buffer[i])
    {
        if (!isdigit(buffer[i]))
        {
            // Если символ - десятичная точка, установить ее позицию
            if (buffer[i] == '.' && !has_dot)
            {
                has_dot = true;
                real->mantissa.dot_index = i - has_sign;
                i++;
            }
            else
                return ERROR_INVALID_DATA;  // Ошибка при некорректных символах
        }
        else
            real->mantissa.digits[mant_size++] = buffer[i++] - '0';  // Добавление цифры в мантиссу

        if (mant_size > max_mantissa_size)
        {
            return ERROR_MANTISSA_TOO_BIG;  // Ошибка при превышении допустимого размера мантиссы
        }
    }

    if (mant_size == 0)
        return ERROR_INVALID_DATA;  // Ошибка, если мантисса пустая

    real->mantissa.size = mant_size;

    // Если точка отсутствует, поставить ее в конец мантиссы
    if (real->mantissa.dot_index == -1)
        real->mantissa.dot_index = real->mantissa.size;
        
    return ERROR_SUCCESS;
}

// Функция для чтения экспоненты
error_codes_e read_exponent(real_t *real, char *buffer)
{
    char *ep;

    if (strlen(buffer) == 0)
        return ERROR_INVALID_DATA;  // Ошибка при пустой экспоненте

    real->exponent = strtol(buffer, &ep, 10);  // Преобразование строки в число
    if (*ep != '\0')
        return ERROR_INVALID_DATA;  // Ошибка при неверных символах в экспоненте

    if (abs(real->exponent) > MAX_EXPONENT_VAL)
        return ERROR_EXPONENT_TOO_BIG;  // Ошибка при превышении допустимого значения экспоненты
    
    return ERROR_SUCCESS;
}

// Функция для вывода действительного числа
void print_real(real_t *real)
{
    // Печать знака числа
    printf(real->mantissa_sign ? "+" : "-");

    // Если десятичная точка находится в начале, вывести 0
    if (real->mantissa.dot_index == 0)
        printf("0");

    // Печать мантиссы с учетом позиции десятичной точки
    for (int8_t i = 0; i < real->mantissa.size; i++)
    {
        if (i == real->mantissa.dot_index)
            printf(".");  // Вставка десятичной точки
        printf("%u", real->mantissa.digits[i]);
    }
    // Печать экспоненты
    printf("e");
    printf("%d\n", real->exponent);
}
