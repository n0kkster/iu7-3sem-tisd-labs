#include <stdio.h>
#include <stdbool.h>
#include "io.h"
#include "matrix.h"
#include "errors.h"
#include "actions.h"

int main(void)
{
    sparse_matrix_t sparse_matrix = { 0 };
    sparse_vector_t sparse_vector = { 0 };
    sparse_vector_t res = { 0 };
    error_codes_e rc;
    action_e action;

    bool inited = false, res_calculated = false;

    float **matrix = NULL;
    long n, m;

    float *vector = NULL;
    long vlen;

    float *res_vec = NULL;

    do
    {
        print_menu();
        if ((rc = read_menu(&action)) != ERROR_SUCCESS)
            return handle_error(rc);

        switch (action)
        {
            case READ_FROM_FILE:
                if ((rc = handle_read_from_file(&matrix, &sparse_matrix, &vector, &sparse_vector, &n, &m, &vlen, NULL)) != ERROR_SUCCESS)
                {
                    free_matrix(matrix, n);
                    free(vector);
                    free_sparse_matrix(&sparse_matrix);
                    free_sparse_vector(&sparse_vector);
                    return handle_error(rc);
                }
                inited = true;
                break;
            case READ_FROM_USER:
                if ((rc = handle_read_from_user(&matrix, &sparse_matrix, &vector, &sparse_vector, &n, &m, &vlen)) != ERROR_SUCCESS)
                {
                    free_matrix(matrix, n);
                    free(vector);
                    free_sparse_matrix(&sparse_matrix);
                    free_sparse_vector(&sparse_vector);
                    return handle_error(rc);
                } 
                inited = true;
                break;
            case INPUT_COORD:
                if ((rc = handle_input_coord(&matrix, &sparse_matrix, &vector, &sparse_vector, &n, &m, &vlen)) != ERROR_SUCCESS)
                {
                    free_matrix(matrix, n);
                    free(vector);
                    free_sparse_matrix(&sparse_matrix);
                    free_sparse_vector(&sparse_vector);
                    return handle_error(rc);
                } 
                inited = true;
                break;
            case PRINT_STANDART:
                if (!inited)
                {
                    puts("Матрица и вектор неинициализированы.");
                    break;
                }
                print_matrix(matrix, n, m);
                print_vector(vector, vlen);
                break;
            case PRINT_PACKED:
                if (!inited)
                {
                    puts("Матрица и вектор неинициализированы.");
                    break;
                }
                print_sparse_matrix(&sparse_matrix);
                print_sparse_vector(&sparse_vector);
                break;
            case MULTIPLY_FAST:
                if (vlen != n)
                {
                    puts("Невозможно выполнить умножение! Длина вектора должна быть равна количеству строк матрицы.");
                    break;
                }
                if (!inited)
                {
                    puts("Матрица и вектор неинициализированы.");
                    break;
                }
                if ((rc = fast_multiply(&sparse_matrix, &sparse_vector, &res, &res_vec, m)) != ERROR_SUCCESS)
                {
                    free_matrix(matrix, n);
                    free(vector);
                    free_sparse_matrix(&sparse_matrix);
                    free_sparse_vector(&sparse_vector);
                    free(res_vec);
                    free_sparse_vector(&res);
                    return handle_error(rc);
                } 
                res_calculated = true;
                print_sparse_vector(&res);
                // print_vector(res_vec, m);
                break;
            case MULTIPLY_STANDART:
                if (vlen != n)
                {
                    puts("Невозможно выполнить умножение! Длина вектора должна быть равна количеству строк матрицы.");
                    break;
                }
                if (!inited)
                {
                    puts("Матрица и вектор неинициализированы.");
                    break;
                }
                if ((rc = standart_multiply(matrix, vector, &res_vec, n, m)) != ERROR_SUCCESS)
                {
                    free_matrix(matrix, n);
                    free(vector);
                    free_sparse_matrix(&sparse_matrix);
                    free_sparse_vector(&sparse_vector);
                    free(res_vec);
                    return handle_error(rc);
                } 
                print_vector(res_vec, m);
                break;
            case SPEEDTEST:
                if ((rc = handle_speedtest()) != ERROR_SUCCESS)
                {
                    if (inited)
                    {
                        free_sparse_matrix(&sparse_matrix);
                        free_sparse_vector(&sparse_vector);
                        free_matrix(matrix, n);
                        free(vector);
                    }
                    if (res_calculated)
                    {
                        free_sparse_vector(&res);
                        free(res_vec);
                    }

                    return handle_error(rc);
                }
                break;
            case EXIT:
                break;
            default:
                free_matrix(matrix, n);
                free(vector);
                free_sparse_matrix(&sparse_matrix);
                free_sparse_vector(&sparse_vector);
                return handle_error(ERROR_INVALID_MENU_NUM);
        }
    } while (action != EXIT);

    if (inited)
    {
        free_sparse_matrix(&sparse_matrix);
        free_sparse_vector(&sparse_vector);
        free_matrix(matrix, n);
        free(vector);
    }

    if (res_calculated)
    {
        free_sparse_vector(&res);
        free(res_vec);
    }
    
    return ERROR_SUCCESS;
}
