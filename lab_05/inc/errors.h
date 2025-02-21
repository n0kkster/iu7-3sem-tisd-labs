#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

#define ESC "\x1b"
#define RESET ESC"[0m"
#define RED ESC"[38;5;196m"

typedef enum
{
    ERROR_SUCCESS,
    ERROR_QUEUE_EMPTY,
    ERROR_ALLOCATING_MEM,
    ERROR_QUEUE_FULL,
    ERROR_READING_MENU,
    ERROR_STRING_TOO_LONG,
    ERROR_PARSING_NUMBER,
    ERROR_INCORRECT_ACTION,
    ERROR_OUT_OF_RANGE
} error_codes_e;

error_codes_e handle_error(error_codes_e rc);

#endif /* ERRORS_H */
