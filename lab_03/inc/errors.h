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
    ERROR_STRING_TOO_LONG,
    ERROR_PARSING_NUMBER,
    ERROR_INVALID_MENU_NUM,
    ERROR_READING_FILE,
    ERROR_EMPTY_FILE,
    ERROR_EMPTY_STRING,
    ERROR_INCORRECT_FILE_PTR,
    ERROR_OPENING_FILE,
    ERROR_ALLOCATING_MEM,
    ERROR_INVALID_DATA
} error_codes_e;

error_codes_e handle_error(error_codes_e rc);

#endif /* ERRORS_H */
