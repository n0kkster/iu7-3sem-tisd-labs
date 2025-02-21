#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

#define ESC "\x1b"
#define RESET ESC"[0m"
#define RED ESC"[38;5;196m"

typedef enum
{
    ERROR_SUCCESS,
    ERROR_OPENING_FILE,
    ERROR_READING_FILE,
    ERROR_ALLOCATING_MEM,
    ERROR_READING_MENU,
    ERROR_PARSING_NUMBER,
    ERROR_INCORRECT_ACTION,
    ERROR_READING_STRING,
    ERROR_EMPTY_STRING
} error_codes_e;

error_codes_e handle_error(error_codes_e rc);

#endif /* ERRORS_H */
