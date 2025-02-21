#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

#define ESC "\x1b"
#define RESET ESC"[0m"
#define RED ESC"[38;5;196m"

typedef enum
{
    ERROR_SUCCESS,
    ERROR_READING_MENU,
    ERROR_PARSING_NUMBER,
    ERROR_ALLOCATING_MEM,
    ERROR_STRING_TOO_LONG,
    ERROR_INCORRECT_ACTION,
    ERROR_STACK_OVERFLOW,
    ERROR_EMPTY_STACK,
} error_codes_e;

error_codes_e handle_error(error_codes_e rc);

#endif /* ERRORS_H */
