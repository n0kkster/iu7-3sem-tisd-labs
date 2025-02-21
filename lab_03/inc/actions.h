#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <sys/time.h>

#define MEASURES 100

typedef enum
{
    EXIT,
    READ_FROM_FILE,
    READ_FROM_USER,
    INPUT_COORD,
    PRINT_STANDART,
    PRINT_PACKED,
    MULTIPLY_FAST,
    MULTIPLY_STANDART,
    SPEEDTEST
} action_e;

#include "errors.h"
#include "matrix.h"
#include "files.h"
#include "io.h"

error_codes_e handle_read_from_file(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen, char *filename);
error_codes_e handle_read_from_user(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen);
error_codes_e handle_input_coord(float ***matrix, sparse_matrix_t *sparse_matrix, float **vec, sparse_vector_t *sparse_vector, long *n, long *m, long *vlen);
error_codes_e handle_speedtest(void);

#endif /* ACTIONS_H */
