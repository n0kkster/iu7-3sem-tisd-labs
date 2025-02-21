#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "errors.h"
#include "strings.h"

error_codes_e read_struct_from_file(FILE *f, car_t *car);
error_codes_e read_table_from_file(char *filename, car_t cars[], size_t *cars_count);
error_codes_e read_string(FILE *f, char buffer[], size_t size);
error_codes_e get_structs_count(char *filename, size_t *cars_count);

#endif