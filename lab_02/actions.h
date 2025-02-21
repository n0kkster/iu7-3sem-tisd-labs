#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include "files.h"
#include "types.h"
#include "errors.h"
#include "io.h"


error_codes_e handle_read_from_file(car_t cars[], key_t keys[], size_t *read_count);
error_codes_e handle_find(car_t cars[], size_t count);
void handle_sort_records(car_t cars[], size_t count);
void handle_sort_keys(key_t keys[], size_t count);
void handle_print_by_keys(car_t cars[], key_t keys[], size_t count);
error_codes_e handle_add_record(car_t cars[], key_t keys[], size_t *count);
error_codes_e handle_delete_record(car_t cars[], key_t keys[], size_t *count);
error_codes_e handle_collect_data(car_t cars[], key_t keys[], size_t count);
void sort_keys(key_t keys[], size_t count);
void sort_records(car_t cars[], size_t count);

#endif
