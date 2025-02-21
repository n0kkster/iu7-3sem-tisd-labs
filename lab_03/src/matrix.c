#include "matrix.h"

// Функция для выделения памяти под двумерный массив (матрицу) размером n x m.
float** allocate_matrix(size_t n, size_t m)
{
    // Выделяем память под массив указателей на строки.
    float **matrix = malloc(n * sizeof(float *));
    if (!matrix)
        return NULL; // Возвращаем NULL, если память не удалось выделить.

    // Выделяем память для каждой строки.
    for (size_t i = 0; i < n; i++)
    {
        matrix[i] = malloc(m * sizeof(float));
        if (!matrix[i])
        {
            // Если не удалось выделить память для строки, освобождаем ранее выделенные ресурсы.
            free_matrix(matrix, i);
            return NULL;
        }
    }
    return matrix; // Возвращаем указатель на матрицу.
}

// Функция для освобождения памяти, выделенной под матрицу.
void free_matrix(float **matrix, size_t n)
{
    if (matrix == NULL)
        return; // Если матрица уже пустая, ничего не делаем.

    // Освобождаем память для каждой строки.
    for (size_t i = 0; i < n; i++)
        free(matrix[i]);
    // Освобождаем память под массив указателей на строки.
    free(matrix);
}

// Функция для нахождения максимального по модулю элемента в матрице.
int find_max_abs(float **matrix, size_t n, size_t m)
{
    int max = 0; // Инициализируем максимальное значение нулем.

    // Проходим по всем элементам матрицы.
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            // Если текущий элемент больше по модулю, обновляем значение max.
            if (abs((int)matrix[i][j]) > max)
                max = abs((int)matrix[i][j]);
        }
    }
    return max == 0 ? 1 : max; // Возвращаем 1, если все элементы были нулями.
}

// Функция для подсчета количества ненулевых элементов в матрице.
size_t count_non_zero_elements(float **matrix, size_t n, size_t m)
{
    size_t cnt = 0; // Инициализируем счетчик ненулевых элементов.

    // Проходим по всем элементам матрицы.
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            // Увеличиваем счетчик, если элемент не равен нулю.
            if (!float_eq_zero(matrix[i][j]))
                cnt++;
        }
    }
    return cnt; // Возвращаем количество ненулевых элементов.
}

// Функция для подсчета количества ненулевых элементов в векторе.
size_t count_non_zero_elements_vector(float *vec, size_t vlen)
{
    size_t cnt = 0; // Инициализируем счетчик ненулевых элементов.

    // Проходим по всем элементам вектора.
    for (size_t i = 0; i < vlen; i++)
        if (!float_eq_zero(vec[i])) // Проверяем, является ли элемент ненулевым.
            cnt++;

    return cnt; // Возвращаем количество ненулевых элементов.
}

// Преобразование обычной матрицы в разреженную форму.
error_codes_e to_sparse_matrix(float **matrix, sparse_matrix_t *sparse_matrix, size_t n, size_t m)
{
    size_t a_index = 0, j_index = 0, start = 0;
    size_t non_zero_count = count_non_zero_elements(matrix, n, m); // Считаем количество ненулевых элементов.
    bool has_non_zero = false;
    
    // Выделяем память для массива ненулевых значений A.
    sparse_matrix->A = (float *)malloc(non_zero_count * sizeof(float));
    if (!sparse_matrix->A)
        return ERROR_ALLOCATING_MEM; // Ошибка при выделении памяти.

    // Выделяем память для массива индексов строк IA.
    sparse_matrix->IA = (size_t *)malloc(non_zero_count * sizeof(size_t));
    if (!sparse_matrix->IA)
    {
        free_sparse_matrix(sparse_matrix); // Освобождаем память в случае ошибки.
        return ERROR_ALLOCATING_MEM;
    }

    // Выделяем память для массива индексов столбцов JA.
    sparse_matrix->JA = (long *)malloc(m * sizeof(long));
    if (!sparse_matrix->JA)
    {
        free_sparse_matrix(sparse_matrix); // Освобождаем память в случае ошибки.
        return ERROR_ALLOCATING_MEM;
    }

    // Заполняем разреженную матрицу.
    for (size_t j = 0; j < m; j++)
    {
        start = a_index; // Запоминаем начальный индекс для текущего столбца.
        has_non_zero = false;
        for (size_t i = 0; i < n; i++)
        {
            if (!float_eq_zero(matrix[i][j])) // Если элемент не нулевой.
            {
                sparse_matrix->A[a_index] = matrix[i][j]; // Записываем значение.
                sparse_matrix->IA[a_index++] = i; // Записываем индекс строки.
                has_non_zero = true;
            }
        }
        // Записываем индекс начала столбца или -1, если столбец пуст.
        sparse_matrix->JA[j_index++] = has_non_zero ? (long)start : -1;
    }

    // Обновляем счетчики ненулевых элементов.
    sparse_matrix->a_count = a_index;
    sparse_matrix->ja_count = j_index;

    return ERROR_SUCCESS; // Успешное завершение.
}

// Освобождаем память, выделенную для разреженной матрицы.
void free_sparse_matrix(sparse_matrix_t *sparse_matrix)
{
    free(sparse_matrix->A);
    free(sparse_matrix->IA);
    free(sparse_matrix->JA);
}

// Освобождаем память, выделенную для разреженного вектора.
void free_sparse_vector(sparse_vector_t *sparse_vector)
{
    free(sparse_vector->B);
    free(sparse_vector->JB);
}

// Преобразование обычного вектора в разреженный формат.
error_codes_e to_sparse_vector(float *vector, sparse_vector_t *sparse_vector, size_t vlen)
{
    size_t j_index = 0;
    size_t non_zero_count = count_non_zero_elements_vector(vector, vlen); // Считаем количество ненулевых элементов.

    // Выделяем память для ненулевых значений вектора.
    sparse_vector->B = (float *)malloc(non_zero_count * sizeof(float));
    if (!sparse_vector->B)
        return ERROR_ALLOCATING_MEM; // Ошибка выделения памяти.

    // Выделяем память для индексов ненулевых значений вектора.
    sparse_vector->JB = (size_t *)malloc(non_zero_count * sizeof(size_t));
    if (!sparse_vector->JB)
    {
        free_sparse_vector(sparse_vector); // Освобождаем память в случае ошибки.
        return ERROR_ALLOCATING_MEM;
    }

    // Заполняем разреженный вектор.
    for (size_t i = 0; i < vlen; i++)
    {
        if (!float_eq_zero(vector[i])) // Если элемент не равен нулю.
        {
            sparse_vector->B[j_index] = vector[i]; // Записываем значение.
            sparse_vector->JB[j_index++] = i; // Записываем индекс.
        }
    }
    sparse_vector->len = j_index; // Обновляем длину вектора.

    return ERROR_SUCCESS; // Успешное завершение.
}

// Функция для проверки, является ли число практически равным нулю.
bool float_eq_zero(float a)
{
    return fabsf(a) <= 1e-6f; // Используем маленькое значение для сравнения с нулем.
}

// Поиск максимального значения в массиве.
size_t max(size_t *a, size_t len)
{
    size_t max = 0;
    for (size_t i = 0; i < len; i++)
        if (a[i] > max)
            max = a[i];
        
    return max; // Возвращаем максимальное значение.
}

// Умножение разреженной матрицы на разреженный вектор с результатом в виде разреженного вектора.
error_codes_e fast_multiply(sparse_matrix_t *m, sparse_vector_t *v, sparse_vector_t *res, float **res_vec, size_t vlen)
{   
    size_t IP_len = MAX(max(m->IA, m->a_count) + 1, max(v->JB, v->len) + 1), res_ind = 0;
    long b = 0;
    float h = 0;
    long *IP = (long *)malloc(IP_len * sizeof(size_t)); // Выделяем память под временный массив IP.
    error_codes_e rc;
    size_t k;

    if (!IP)
        return ERROR_ALLOCATING_MEM; // Ошибка выделения памяти.

    // Выделяем память для результирующего вектора.
    if (!(*res_vec = (float *)malloc(vlen * sizeof(float))))
    {
        free(IP);
        return ERROR_ALLOCATING_MEM;
    }

    // Умножаем разреженную матрицу на вектор.
    for (size_t j = 0; j < m->ja_count; j++)
    {
        if (m->JA[j] == -1)
        {
            // Если столбец нулевой, просто записываем 0 в результат
            (*res_vec)[res_ind++] = 0;
            continue;
        }

        h = 0;
        for (size_t i = 0; i < IP_len; i++)
            IP[i] = -1;

        if (j + 1 >= m->ja_count)
            b = m->a_count;
        else
        {
            k = j + 1;
            while ((b = m->JA[k]) == -1 && k < m->ja_count)
                k++;
            if (k == m->ja_count)
                b = m->a_count;
        }
        for (long i = m->JA[j]; i < b; i++)
            IP[m->IA[i]] = i;

        // Выполняем умножение на ненулевые элементы вектора.
        for (size_t i = 0; i < v->len; i++)
        {
            if (IP[v->JB[i]] != -1)
                // h += m->A[IP[v->JB[i]]] * v->B[i];
                h += *((*m).A + *(IP + *((*v).JB + i)))**((*v).B + i);
        }
        (*res_vec)[res_ind++] = h;
    }

    // Заполняем оставшуюся часть результата нулями.
    while (res_ind < vlen)
        (*res_vec)[res_ind++] = 0;

    // Преобразуем результат в разреженный формат.
    if ((rc = to_sparse_vector(*res_vec, res, vlen)) != ERROR_SUCCESS)
    {
        free(IP);
        free(*res_vec);
        return rc;
    }

    free(IP);

    return ERROR_SUCCESS;  
}

// Стандартное умножение обычной матрицы на вектор.
error_codes_e standart_multiply(float **matrix, float *vec, float **res, size_t n, size_t m)
{
    // Выделяем память для результата.
    if (!(*res = (float *)calloc(m, sizeof(float))))
        return ERROR_ALLOCATING_MEM; // Ошибка выделения памяти.

    // Выполняем умножение матрицы на вектор.
    for (size_t j = 0; j < m; j++)
        for (size_t i = 0; i < n; i++)
            (*res)[j] += matrix[i][j] * vec[i];

    return ERROR_SUCCESS; // Успешное завершение.
}
