#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "io.h"
#include "errors.h"

error_codes_e read_string(FILE *f, char buffer[], size_t size);
error_codes_e read_matrix_dimensions(FILE *f, long *n, long *m);
error_codes_e read_matrix(FILE *f, float **matrix, size_t n, size_t m);
error_codes_e read_vector_len(FILE *f, long *n);
error_codes_e read_vector(FILE *f, float *vec, size_t vlen);

#endif /* FILES_H */
