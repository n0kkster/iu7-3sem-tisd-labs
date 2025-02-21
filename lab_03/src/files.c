#include "files.h"

// Функция для чтения строки из файла
error_codes_e read_string(FILE *f, char buffer[], size_t size)
{
    if (!f)
        return ERROR_INCORRECT_FILE_PTR;

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

error_codes_e read_matrix_dimensions(FILE *f, long *n, long *m)
{
    char buffer[MAX_STRING_SIZE + 2], *ep;
    error_codes_e rc;

    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
        return rc;
    
    *n = strtol(buffer, &ep, 10);
    if (ep == buffer || *ep == '\0')
        return ERROR_PARSING_NUMBER;

    *m = strtol(ep, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER;

    return ERROR_SUCCESS;
}

error_codes_e read_matrix(FILE *f, float **matrix, size_t n, size_t m)
{
    float el;
    char buffer[2];

    if (!f)
        return ERROR_INCORRECT_FILE_PTR;

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            if (fscanf(f, "%f", &el) != 1)
                return ERROR_INVALID_DATA;
            matrix[i][j] = el;
        }
    }

    if (!fgets(buffer, 2, f))
        return ERROR_READING_FILE;

    if (buffer[0] != '\n')
        return ERROR_INVALID_DATA;

    return ERROR_SUCCESS;
}

error_codes_e read_vector_len(FILE *f, long *n)
{
    char buffer[MAX_STRING_SIZE + 2], *ep;
    error_codes_e rc;

    if ((rc = read_string(f, buffer, MAX_STRING_SIZE + 2)) != ERROR_SUCCESS)
    {
        puts(buffer);
        return rc;
    }
    
    *n = strtol(buffer, &ep, 10);
    if (*ep != '\0')
        return ERROR_PARSING_NUMBER;

    return ERROR_SUCCESS;
}

error_codes_e read_vector(FILE *f, float *vec, size_t vlen)
{
    float el;

    if (!f)
        return ERROR_INCORRECT_FILE_PTR;

    for (size_t i = 0; i < vlen; i++)
    {
        if (fscanf(f, "%f", &el) != 1)
            return ERROR_INVALID_DATA;
        vec[i] = el;
    }

    if (f != stdin)
        if ((fscanf(f, "%f", &el) == 1) || !feof(f))
            return ERROR_INVALID_DATA;

    return ERROR_SUCCESS;
}
