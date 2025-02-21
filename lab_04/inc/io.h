#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"
#include "actions.h"

#define MAX_STRING_SIZE 25

#define ESC "\x1b"
#define RESET ESC"[0m"
#define GREEN ESC"[38;5;193m"
#define YELLOW ESC"[38;5;226m"
#define BLUE ESC"[38;5;111m"
#define LINE_START YELLOW"|"GREEN" "
#define LINE_END YELLOW ESC"[%ldG|"GREEN"\n"

void print_menu(void);
error_codes_e read_menu(action_e *action);
error_codes_e read_string(FILE *f, char *buffer, size_t size);

#endif /* IO_H */
