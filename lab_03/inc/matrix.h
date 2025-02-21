#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include "errors.h"

#define MAX(a, b) (a) > (b) ? (a) : (b) 

typedef struct
{
    float *A;
    size_t *IA;
    long *JA;
    size_t a_count;
    size_t ja_count;
} sparse_matrix_t;

typedef struct
{
    float *B;
    size_t *JB;
    size_t len;
} sparse_vector_t;

float** allocate_matrix(size_t n, size_t m);
void free_matrix(float **matrix, size_t n);
int find_max_abs(float **matrix, size_t n, size_t m);
error_codes_e to_sparse_matrix(float **matrix, sparse_matrix_t *sparse_matrix, size_t n, size_t m);
bool float_eq_zero(float a);
void free_sparse_matrix(sparse_matrix_t *sparse_matrix);
void free_sparse_vector(sparse_vector_t *sparse_vector);
error_codes_e to_sparse_vector(float *vector, sparse_vector_t *sparse_vector, size_t vlen);
error_codes_e fast_multiply(sparse_matrix_t *m, sparse_vector_t *v, sparse_vector_t *res, float **res_vec, size_t vlen);
error_codes_e standart_multiply(float **matrix, float *vec, float **res, size_t n, size_t m);

#endif /* MATRIX_H */
