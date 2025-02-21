#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "errors.h"
#include "matrix.h"
#include "actions.h"

#define ESC "\x1b"
#define RESET ESC"[0m"
#define GREEN ESC"[38;5;193m"
#define YELLOW ESC"[38;5;226m"
#define BLUE ESC"[38;5;111m"
#define LINE_START YELLOW"|"GREEN" "
#define LINE_END YELLOW ESC"[%ldG|"GREEN"\n"

#define MAX_STRING_SIZE 25

void print_menu(void);
error_codes_e read_menu(action_e *action);
void print_matrix(float **matrix, size_t n, size_t m);
void print_sparse_matrix(sparse_matrix_t *sparse_matrix);
void print_vector(float *vec, size_t vlen);
void print_sparse_vector(sparse_vector_t *sparse_vector);

#endif /* IO_H */
