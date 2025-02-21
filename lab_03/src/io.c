#include "io.h"

// Функция для вывода меню пользователю
void print_menu()
{
    printf(BLUE);  // Изменение цвета текста на синий
    puts("----------------------------------------------------");
    puts("Программа для умножения разреженного вектора на разреженную матрицу.");
    puts("1. Считать данные из файла.");
    puts("2. Считать данные с клавиатуры");
    puts("3. Ввести матрицу координатным способом.");
    puts("4. Вывести данные в стандартном виде.");
    puts("5. Вывести данные в упакованном виде.");
    puts("6. Выполнить \"быстрое\" умножение и вывести результат.");
    puts("7. Выполнить стандартное умножение и вывести результат.");
    puts("8. Провести сравнение скорости.");
    puts("0. Выйти из программы.");
    puts("----------------------------------------------------");
    puts("Введите пункт меню: ");
    printf(RESET);  // Сброс цвета текста
}

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

void print_matrix(float **matrix, size_t n, size_t m)
{
    int max = find_max_abs(matrix, n, m);
    int len = (int)(floor(log10(max))) + 12;
    
    printf(YELLOW);
    for (size_t i = 0; i < m * len; i++)
        printf("-");
    puts("");
    
    for (size_t i = 0; i < n; i++)
    {
        printf(LINE_START);
        for (size_t j = 0; j < m; j++)
            printf("%-*f ", len - 2, matrix[i][j]);
        printf(LINE_END, m * len);
    }

    printf(YELLOW);
    for (size_t i = 0; i < m * len; i++)
        printf("-");
    printf(RESET"\n");
}

void print_sparse_matrix(sparse_matrix_t *sparse_matrix)
{
    printf("A: ");
    for (size_t i = 0; i < sparse_matrix->a_count; i++)
        printf("%f ", sparse_matrix->A[i]);
    puts("");

    printf("IA: ");
    for (size_t i = 0; i < sparse_matrix->a_count; i++)
        printf("%zu ", sparse_matrix->IA[i]);
    puts("");

    printf("JA: ");
    for (size_t i = 0; i < sparse_matrix->ja_count; i++)
        printf("%ld ", sparse_matrix->JA[i]);
    puts("");
}

void print_vector(float *vec, size_t vlen)
{
    printf(GREEN);
    for (size_t i = 0; i < vlen; i++)
        printf("%f ", vec[i]);
    printf(RESET"\n");
}

void print_sparse_vector(sparse_vector_t *sparse_vector)
{
    printf("B: ");
    for (size_t i = 0; i < sparse_vector->len; i++)
        printf("%f ", sparse_vector->B[i]);
    printf("\nJB: ");
    for (size_t i = 0; i < sparse_vector->len; i++)
        printf("%zu ", sparse_vector->JB[i]);
    puts("");
}
