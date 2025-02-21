#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "errors.h"
#include "graph.h"

error_codes_e read_data_from_file(char *filename, graph_t *graph);
bool arr_find(int *arr, int val, size_t arr_size);

#endif /* FILES_H */
