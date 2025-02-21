#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "types.h"
#include "errors.h"

#define ESC "\x1b"
#define RESET ESC"[0m"
#define GREEN ESC"[38;5;193m"
#define YELLOW ESC"[38;5;226m"
#define BLUE ESC"[38;5;111m"
#define LINE_START YELLOW"|"GREEN" "
#define LINE_END YELLOW ESC"[49G|"GREEN"\n"

void print_menu();
error_codes_e read_menu(action_e *action);
void print_struct(car_t *car);
void print_table(car_t cars[], size_t cars_count);
void print_key(key_t *key);
void print_key_table(key_t keys[], size_t keys_count);
error_codes_e write_table_to_file(char *filename, car_t cars[], size_t cars_count);

#endif
