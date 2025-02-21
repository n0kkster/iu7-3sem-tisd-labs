#include "files.h"  // Подключение заголовочного файла с функциями работы с файлами

// Функция для чтения таблицы автомобилей из файла
error_codes_e read_table_from_file(char *filename, car_t cars[], size_t *cars_count)
{
    FILE *f;
    car_t car;  // Переменная для хранения текущей структуры автомобиля
    error_codes_e rc;  // Переменная для хранения кода ошибки
    size_t read_count = 0;  // Счётчик прочитанных записей

    // Открытие файла для чтения
    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;  // Возврат ошибки при невозможности открыть файл

    // Чтение структур автомобилей из файла
    while ((rc = read_struct_from_file(f, &car)) == ERROR_SUCCESS)
    {
        cars[read_count++] = car;  // Запись автомобиля в массив
    }
    *cars_count = read_count;  // Запись общего количества прочитанных автомобилей

    // Проверка на пустой файл
    if (rc == ERROR_FILE_IS_EMPTY && read_count)
        rc = ERROR_SUCCESS;  // Если были прочитаны записи, файл не считается пустым

    fclose(f);  // Закрытие файла

    return rc;  // Возврат кода ошибки или успеха
}

// Функция для получения количества записей (автомобилей) в файле
error_codes_e get_structs_count(char *filename, size_t *cars_count)
{
    FILE *f;
    car_t car;  // Временная переменная для чтения структуры
    error_codes_e rc;
    size_t read_count = 0;  // Счётчик записей

    // Открытие файла для чтения
    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;  // Ошибка при открытии файла

    // Подсчёт числа структур в файле
    while ((rc = read_struct_from_file(f, &car)) == ERROR_SUCCESS)
        read_count++;  // Увеличение счётчика при успешном чтении
    *cars_count = read_count;  // Запись количества прочитанных структур

    // Обработка случая пустого файла
    if (rc == ERROR_FILE_IS_EMPTY && read_count)
        rc = ERROR_SUCCESS;  // Файл не считается пустым, если были прочитаны записи

    fclose(f);  // Закрытие файла

    return rc;  // Возврат кода ошибки или успеха
}

// Функция для чтения структуры автомобиля из файла
error_codes_e read_struct_from_file(FILE *f, car_t *car)
{
    char buffer[MAX_STRING_SIZE + 2];  // Буфер для хранения строки
    error_codes_e rc;

    if (!f)
        return ERROR_FILE_IS_NULL;  // Ошибка, если файл не был передан

    // Чтение и запись марки автомобиля
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc == ERROR_READING_FILE ? ERROR_FILE_IS_EMPTY : rc;
    strcpy(car->brand, buffer);

    // Чтение и запись страны производителя
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    strcpy(car->origin, buffer);

    // Чтение и запись поддержки обслуживания
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    string_tolower(buffer);  // Преобразование строки в нижний регистр
    car->can_be_maintained = strcmp(buffer, "yes") == 0;

    // Чтение и запись цены автомобиля
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint32(buffer, &car->price)))
        return rc;

    // Чтение и запись цвета автомобиля
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    strcpy(car->color, buffer);

    // Чтение и запись состояния (новая или б/у машина)
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    string_tolower(buffer);
    car->condition = (condition_e)(strcmp(buffer, "used") == 0);

    // Обработка новой машины
    if (car->condition == NEW)
    {
        if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
            return rc;
        if ((rc = parse_uint8(buffer, &car->data.new.warranty)))
            return rc;

        return ERROR_SUCCESS;  // Успех для новой машины
    }

    // Обработка б/у машины
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint32(buffer, &car->data.used.year)))
        return rc;

    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint32(buffer, &car->data.used.mileage)))
        return rc;

    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint8(buffer, &car->data.used.owners_count)))
        return rc;
    
    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)))
        return rc;
    if ((rc = parse_uint8(buffer, &car->data.used.repairs_count)))
        return rc;

    return ERROR_SUCCESS;  // Успешное чтение структуры
}

// Функция для чтения строки из файла
error_codes_e read_string(FILE *f, char buffer[], size_t size)
{
    // Чтение строки с проверкой на конец файла
    if (!fgets(buffer, size, f))
        return ERROR_READING_FILE;

    // Удаление символа новой строки
    buffer[strcspn(buffer, "\n")] = '\0';

    // Проверка на превышение допустимой длины строки
    if (strlen(buffer) > MAX_STRING_SIZE)
        return ERROR_STRING_TOO_LONG;
    
    // Проверка на пустую строку
    if (strlen(buffer) == 0)
        return ERROR_EMPTY_STRING;

    return ERROR_SUCCESS;  // Успешное чтение строки
}
