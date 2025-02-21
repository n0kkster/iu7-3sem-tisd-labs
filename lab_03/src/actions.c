#include "actions.h"

error_codes_e handle_read_from_file(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen, char *filename)
{
    FILE *f;
    char buffer[MAX_STRING_SIZE + 2];
    error_codes_e rc;

    if (filename == NULL)
    {
        printf("Введите имя файла: ");
        if ((rc = read_string(stdin, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
            return rc;
    }
    else
        strncpy(buffer, filename, MAX_STRING_SIZE + 1);
    
    if ((f = fopen(buffer, "r")) == NULL)
        return ERROR_OPENING_FILE;  // Ошибка при открытии файла

    if ((rc = read_matrix_dimensions(f, n, m)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc == ERROR_READING_FILE ? ERROR_EMPTY_FILE : rc;
    }

    if (*n <= 0 || *m <= 0)
        return ERROR_INVALID_DATA;

    if (!(*matrix = allocate_matrix(*n, *m)))
    {
        fclose(f);
        return ERROR_ALLOCATING_MEM;
    }    

    if ((rc = read_matrix(f, *matrix, *n, *m)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    if ((rc = to_sparse_matrix(*matrix, sparse_matrix, *n, *m)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    if ((rc = read_vector_len(f, vlen)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    if (*vlen <= 0)
        return ERROR_INVALID_DATA;

    if (!(*vec = malloc(*vlen * sizeof(**vec))))
    {
        fclose(f);
        return ERROR_ALLOCATING_MEM;
    }

    if ((rc = read_vector(f, *vec, *vlen)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    if ((rc = to_sparse_vector(*vec, sparse_vector, *vlen)) != ERROR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    return ERROR_SUCCESS;
}

error_codes_e handle_read_from_user(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen)
{
    error_codes_e rc;

    printf("Введите количество строк и столбцов в 1 строке через пробел: ");
    if ((rc = read_matrix_dimensions(stdin, n, m)) != ERROR_SUCCESS)
        return rc;

    if (*n <= 0 || *m <= 0)
        return ERROR_INVALID_DATA; 

    if (!(*matrix = allocate_matrix(*n, *m)))
        return ERROR_ALLOCATING_MEM;

    if ((rc = read_matrix(stdin, *matrix, *n, *m)) != ERROR_SUCCESS)
        return rc;

    if ((rc = to_sparse_matrix(*matrix, sparse_matrix, *n, *m)) != ERROR_SUCCESS)
        return rc;

    if ((rc = read_vector_len(stdin, vlen)) != ERROR_SUCCESS)
        return rc;

    if (*vlen <= 0)
        return ERROR_INVALID_DATA;

    if (!(*vec = malloc(*vlen * sizeof(**vec))))
        return ERROR_ALLOCATING_MEM;

    if ((rc = read_vector(stdin, *vec, *vlen)) != ERROR_SUCCESS)
        return ERROR_ALLOCATING_MEM;

    if ((rc = to_sparse_vector(*vec, sparse_vector, *vlen)) != ERROR_SUCCESS)
        return ERROR_ALLOCATING_MEM;

    getchar();

    return ERROR_SUCCESS;
}

error_codes_e handle_input_coord(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen)
{
    error_codes_e rc;
    long k, x, y;
    float val;

    puts("Введите <n> <m> <k>; где n, m - размеры матрицы, k - количество ненулевых элементов:");
    if (scanf("%ld%ld%ld", n, m, &k) != 3)
        return ERROR_INVALID_DATA;

    if (*n <= 0 || *m <= 0 || k < 0)
        return ERROR_INVALID_DATA; 

    if ((*matrix = allocate_matrix(*n, *m)) == NULL)
        return ERROR_ALLOCATING_MEM;

    for (long i = 0; i < *n; i++)
        for (long j = 0; j < *m; j++)
            (*matrix)[i][j] = 0.0f;

    printf("Введите %ld троек чисел вида <x> <y> <val>, где val - значение элемента, x - номер строки элемента, y - номер столбца элемента:\n", k);
    while (k--)
    {
        if (scanf("%ld%ld%f", &x, &y, &val) != 3)
            return ERROR_INVALID_DATA;
        
        if (x < 0 || y < 0 || x >= *n || y >= *m)
            return ERROR_INVALID_DATA;

        (*matrix)[x][y] = val;
    }

    puts("Введите <n> <k>, где n - длина вектора, k - количество ненулевых элементов:");
    if (scanf("%ld%ld", vlen, &k) != 2)
        return ERROR_INVALID_DATA;

    if (*vlen <= 0 || k < 0)
        return ERROR_INVALID_DATA; 

    if (!(*vec = malloc(*vlen * sizeof(**vec))))
        return ERROR_ALLOCATING_MEM;

    for (long i = 0; i < *vlen; i++)
            (*vec)[i] = 0;

    printf("Введите %ld пар чисел вида <i> <val>, где val - значение элемента, i - индекс элемента:\n", k);
    while (k--)
    {
        if (scanf("%ld%f", &x, &val) != 2)
            return ERROR_INVALID_DATA;
        
        if (x < 0 || x >= *vlen)
            return ERROR_INVALID_DATA;

        (*vec)[x] = val;
    }

    if ((rc = to_sparse_matrix(*matrix, sparse_matrix, *n, *m)) != ERROR_SUCCESS)
        return rc;

    if ((rc = to_sparse_vector(*vec, sparse_vector, *vlen)) != ERROR_SUCCESS)
        return ERROR_ALLOCATING_MEM;

    getchar();

    return ERROR_SUCCESS;
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

size_t calc_size(sparse_matrix_t *sm, sparse_vector_t *sv)
{
    size_t matrix_size = 0, vec_size = 0;
    
    matrix_size = sizeof(*sm) + sm->a_count * (sizeof(*sm->A) + sizeof(*sm->IA)) + sm->ja_count * sizeof(*sm->JA);
    vec_size = sizeof(*sv) + sv->len * (sizeof(*sv->B) + sizeof(*sv->JB));
    
    return matrix_size + vec_size;
}

error_codes_e handle_speedtest(void)
{
    error_codes_e rc;

    sparse_matrix_t sparse_matrix = { 0 };
    sparse_vector_t sparse_vector = { 0 };
    sparse_vector_t res = { 0 };

    float **matrix = NULL;
    float *vector = NULL;
    float *res_vec = NULL;

    long n, m;
    long vlen;

    int sizes[] = {100, 500, 1000};
    int size = 0;

    char buffer[MAX_STRING_SIZE + 2] = { 0 };

    unsigned long long fast_time, standart_time, beg, end;

    for (size_t i = 0; i < 3; i++)
    {
        size = sizes[i];
        printf("------------------------------------------------------------\n");
        printf("|%10s%24d%25s\n", "|", size, "|");
        printf("|         |-------------------------------------------------\n");
        printf("|%5s%5s%15s%10s%15s%9s\n", "%", "|", "fast", "|", "standart", "|");
        printf("|         |-------------------------------------------------\n");
        printf("|%10s%7s%5s%9s%4s%7s%5s%9s%3s\n", "|", "time", "|", "memory", "|", "time", "|", "memory", "|");
        printf("------------------------------------------------------------\n");

        for (int percent = 0; percent <= 100; percent++)
        {
            snprintf(buffer, MAX_STRING_SIZE + 2, "test_data/%d.%d.txt", size, percent);
            if ((rc = handle_read_from_file(&matrix, &sparse_matrix, &vector, &sparse_vector, &n, &m, &vlen, buffer)) != ERROR_SUCCESS)
                return rc;

            fast_time = 0;
            for (size_t measure = 0; measure < MEASURES; measure++)
            {
                beg = micros();
                fast_multiply(&sparse_matrix, &sparse_vector, &res, &res_vec, m);
                end = micros();
                free_sparse_vector(&res);
                free(res_vec);
                fast_time += end - beg;
            }
            fast_time /= MEASURES;

            standart_time = 0;
            for (size_t measure = 0; measure < MEASURES; measure++)
            {
                beg = micros();
                standart_multiply(matrix, vector, &res_vec, n, m);
                end = micros();
                free(res_vec);
                standart_time += end - beg;
            }
            standart_time /= MEASURES;
            printf("|%5d%5s%6llu%6s%9zu%4s%6llu%6s%8ld%4s\n", percent, "|", \
            fast_time, "|", calc_size(&sparse_matrix, &sparse_vector), "|", standart_time, "|", n * m * sizeof(**matrix) + n * sizeof(*matrix), "|");
        }
        printf("------------------------------------------------------------\n");
    }

    return ERROR_SUCCESS;
}
