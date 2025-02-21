#include <stdio.h>    // Подключение стандартной библиотеки ввода/вывода
#include <time.h>     // Подключение библиотеки для работы с временем
#include "types.h"    // Подключение пользовательских типов данных
#include "errors.h"   // Подключение кодов ошибок
#include "actions.h"  // Подключение определений действий

int main(void)
{
    // Массив структур автомобилей
    car_t cars[MAX_CARS_COUNT];
    // Массив ключей для автомобилей
    key_t keys[MAX_CARS_COUNT];
    // Переменная для хранения кодов ошибок
    error_codes_e rc;
    // Переменная для хранения текущего действия пользователя
    action_e action;
    // Количество автомобилей
    size_t cars_count = 0;

    // Инициализация генератора случайных чисел текущим временем
    srand(time(NULL));

    // Основной цикл программы
    do
    {
        // Вывод меню
        print_menu();
        // Чтение выбранного пункта меню и обработка ошибки, если она есть
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
            return handle_error(rc);
        
        // Обработка действия, выбранного пользователем
        switch (action)
        {
            // Чтение данных из файла
            case READ_FROM_FILE:
                if ((rc = handle_read_from_file(cars, keys, &cars_count)) != ERROR_SUCCESS)
                    return handle_error(rc);
                break;
            // Поиск автомобиля
            case FIND:
                if ((rc = handle_find(cars, cars_count)) != ERROR_SUCCESS)
                    return handle_error(rc);
                break;
            // Сортировка таблицы записей автомобилей
            case SORT_RECORD_TABLE:
                handle_sort_records(cars, cars_count);
                break;
            // Сортировка таблицы ключей автомобилей
            case SORT_KEY_TABLE:
                handle_sort_keys(keys, cars_count);
                break;
            // Вывод записей автомобилей по отсортированным ключам
            case PRINT_RECORDS_BY_KEYS:
                handle_print_by_keys(cars, keys, cars_count);
                break;
            // Добавление новой записи об автомобиле
            case ADD_RECORD:
                if ((rc = handle_add_record(cars, keys, &cars_count)) != ERROR_SUCCESS)
                    return handle_error(rc);
                break;
            // Удаление записи об автомобиле
            case DELETE_RECORD:
                if ((rc = handle_delete_record(cars, keys, &cars_count)) != ERROR_SUCCESS)
                    return handle_error(rc);
                break;
            // Печать таблицы автомобилей
            case PRINT_TABLE:
                print_table(cars, cars_count);
                break;
            // Сбор дополнительной информации о данных автомобилей
            case COLLECT_INFO:
                if ((rc = handle_collect_data(cars, keys, cars_count)) != ERROR_SUCCESS)
                    return handle_error(rc);
                break;
            // Выход из программы
            case EXIT:
                break;
            // Обработка ошибки при некорректном вводе действия
            default:
                return handle_error(ERROR_READING_MENU);
        }
    } while (action != EXIT); // Цикл продолжается, пока не выбрано действие выхода
    
    return ERROR_SUCCESS; // Завершение программы с успешным кодом
}
