#ifndef ERROR_CODES_E
#define ERROR_CODES_E

#include <stdio.h>

#define ESC "\x1b"
#define RESET ESC"[0m"
#define RED ESC"[38;5;196m"

typedef enum
{
    ERROR_SUCCESS,
    ERROR_READING_MENU,
    ERROR_STRING_TOO_LONG,
    ERROR_FILE_IS_NULL,
    ERROR_READING_FILE,
    ERROR_EMPTY_STRING,
    ERROR_PARSING_NUMBER,
    ERROR_OUT_OF_RANGE,
    ERROR_OPENING_FILE,
    ERROR_FILE_IS_EMPTY,
    ERROR_TOO_MANY_ENTRIES,
    ERROR_INVALID_STRUCT
} error_codes_e;

error_codes_e handle_error(error_codes_e rc);

#endif
