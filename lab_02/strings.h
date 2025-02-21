#ifndef STRINGS_H
#define STRINGS_H

#include <ctype.h>
#include <stdlib.h>
#include "errors.h"

error_codes_e parse_uint32(char *buffer, unsigned int *val);
error_codes_e parse_uint8(char *buffer, unsigned char *val);
void string_tolower(char buffer[]);

#endif