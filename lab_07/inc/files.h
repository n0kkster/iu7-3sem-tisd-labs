#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "errors.h"
#include "tree.h"

error_codes_e read_data_from_file(char *filename, void *data, error_codes_e (*insert)(void *, long));
long count_nums_in_file(char *filename);

#endif /* FILES_H */
