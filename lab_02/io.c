#include "io.h"  // Подключение заголовочного файла, содержащего прототипы функций

// Функция для вывода меню пользователю
void print_menu()
{
    printf(BLUE);  // Изменение цвета текста на синий
    puts("----------------------------------------------------");
    puts("Программы для работы с таблицей записей автомобилей.");
    puts("1. Считать записи в таблицу из файла.");
    puts("2. Вывести список не новых машин указанной марки с одним предыдущим"
        " собственником, отсутствием ремонтов в указанном диапазоне цен."
        "\nТакже найти среди них иномарки с поддержкой обслуживания.");
    puts("3. Отсортировать таблицу записей по цене и вывести ее.");
    puts("4. Отсортировать таблицу ключей по цене и вывести ее.");
    puts("5. Вывести исходную таблицу в упорядоченном виде с использованием упорядоченной таблицы ключей.");
    puts("6. Добавить запись в конец таблицы.");
    puts("7. Удалить запись из таблицы.");
    puts("8. Собрать количественную информацию о работе программы.");
    puts("9. Вывести таблицу записей.");
    puts("0. Выйти из программы.");
    puts("----------------------------------------------------");
    puts("Введите пункт меню: ");
    printf(RESET);  // Сброс цвета текста
}

// Функция для чтения пункта меню, выбранного пользователем
error_codes_e read_menu(action_e *action)
{
    char buffer[MAX_STRING_SIZE + 2];  // Буфер для хранения ввода
    char *ep;  // Указатель на символ, не удавшийся для преобразования

    // Чтение ввода пользователя
    if (!fgets(buffer, MAX_STRING_SIZE + 2, stdin))
        return ERROR_READING_MENU;  // Ошибка при чтении меню

    // Удаление символа новой строки
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Проверка длины строки
    if (strlen(buffer) > MAX_STRING_SIZE)
        return ERROR_STRING_TOO_LONG;  // Ошибка при слишком длинной строке

    if (strlen(buffer) == 0)
        return ERROR_READING_MENU;  // Ошибка при пустой строке

    // Преобразование строки в целое число (выбор действия)
    *action = (action_e)strtol(buffer, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER;  // Ошибка при преобразовании числа

    return ERROR_SUCCESS;  // Успешное завершение
}

// Функция для вывода информации о конкретной структуре автомобиля
void print_struct(car_t *car)
{
    printf(YELLOW"-------------------------------------------------\n");

    printf(GREEN);  // Зелёный цвет текста
    printf(LINE_START "Марка: %s"LINE_END, car->brand);  // Вывод марки автомобиля
    printf(LINE_START "Страна производитель: %s"LINE_END, car->origin);  // Страна производитель
    printf(LINE_START "%s быть обслужена"LINE_END, car->can_be_maintained ? "Может" : "Не может");  // Поддержка обслуживания
    printf(LINE_START "Цена: %d"LINE_END, car->price);  // Цена автомобиля
    printf(LINE_START "Цвет: %s"LINE_END, car->color);  // Цвет автомобиля
    printf(LINE_START "Состояние: %s"LINE_END, car->condition == NEW ? "новое" : "БУ");  // Состояние автомобиля
    if (car->condition == NEW)
        printf(LINE_START "Гарантия: %u"LINE_END, car->data.new.warranty);  // Гарантия для новых машин
    else if (car->condition == USED)
    {
        printf(LINE_START "Год выпуска: %d"LINE_END, car->data.used.year);  // Год выпуска
        printf(LINE_START "Пробег: %d"LINE_END, car->data.used.mileage);  // Пробег
        printf(LINE_START "Кол-во владельцев: %u"LINE_END, car->data.used.owners_count);  // Количество владельцев
        printf(LINE_START "Кол-во ремонтов: %u"LINE_END, car->data.used.repairs_count);  // Количество ремонтов
    }
    else
        printf("Некорректная структура!\n");  // Ошибка при некорректной структуре
    printf(YELLOW"-------------------------------------------------\n"RESET);  // Завершение вывода
}

// Функция для вывода информации о ключе (индексе в таблице)
void print_key(key_t *key)
{
    printf(YELLOW"-------------------------------------------------\n");
    printf(LINE_START "Индекс в таблице: %zu"LINE_END, key->table_index);  // Индекс ключа
    printf(LINE_START "Цена: %d"LINE_END, key->price);  // Цена автомобиля, соответствующего ключу
    printf(YELLOW"-------------------------------------------------\n"RESET);  // Завершение вывода
}

// Функция для вывода всей таблицы автомобилей
void print_table(car_t cars[], size_t cars_count)
{
    if (cars_count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);  // Если таблица пуста
        return;
    }

    for (size_t i = 0; i < cars_count; i++)
    {
        print_struct(&cars[i]);  // Вывод информации о каждом автомобиле
        puts("");
    }
}

// Функция для вывода таблицы ключей
void print_key_table(key_t keys[], size_t keys_count)
{
    if (keys_count == 0)
    {
        printf(RED"Таблица пуста!\n"RESET);  // Если таблица ключей пуста
        return;
    }

    for (size_t i = 0; i < keys_count; i++)
    {
        print_key(&keys[i]);  // Вывод информации о каждом ключе
        puts("");
    }
}

// Функция для записи таблицы автомобилей в файл
error_codes_e write_table_to_file(char *filename, car_t cars[], size_t cars_count)
{
    FILE *f;

    // Открытие файла для записи
    if ((f = fopen(filename, "w")) == NULL)
        return ERROR_OPENING_FILE;  // Ошибка открытия файла

    for (size_t i = 0; i < cars_count; i++)
    {
        // Проверка корректности структуры
        if (cars[i].condition != NEW && cars[i].condition != USED)
        {
            fclose(f);
            return ERROR_INVALID_STRUCT;  // Ошибка некорректной структуры
        }

        // Запись информации об автомобиле в файл
        fprintf(f, "%s\n", cars[i].brand);
        fprintf(f, "%s\n", cars[i].origin);
        fprintf(f, "%s\n", cars[i].can_be_maintained ? "Yes" : "No");
        fprintf(f, "%d\n", cars[i].price);
        fprintf(f, "%s\n", cars[i].color);
        fprintf(f, "%s\n", cars[i].condition == NEW ? "New" : "Used");
        if (cars[i].condition == NEW)
            fprintf(f, "%u\n", cars[i].data.new.warranty);  // Запись данных для новых машин
        else
        {
            fprintf(f, "%d\n", cars[i].data.used.year);  // Год выпуска для б/у машин
            fprintf(f, "%d\n", cars[i].data.used.mileage);  // Пробег
            fprintf(f, "%u\n", cars[i].data.used.owners_count);  // Количество владельцев
            fprintf(f, "%u\n", cars[i].data.used.repairs_count);  // Количество ремонтов
        }
    }

    fclose(f);  // Закрытие файла

    return ERROR_SUCCESS;  // Успешное завершение
}
