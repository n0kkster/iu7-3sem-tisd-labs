#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAX_STRING_SIZE 25
#define MAX_CARS_COUNT 1000

typedef struct 
{
    unsigned char warranty;
} new_t;

typedef struct
{
    unsigned char owners_count;
    unsigned char repairs_count;
    unsigned int year;
    unsigned int mileage;
} used_t;

typedef enum
{
    NEW,
    USED,
} condition_e;

typedef struct
{
    bool can_be_maintained;
    unsigned int price;
    char brand[MAX_STRING_SIZE + 1];
    char origin[MAX_STRING_SIZE + 1];
    char color[MAX_STRING_SIZE + 1];
    condition_e condition;
    union
    {
        new_t new;
        used_t used; 
    } data;
    
} car_t;

typedef struct
{
    unsigned int price;
    size_t table_index;
} key_t;

typedef enum
{
    EXIT,
    READ_FROM_FILE,
    FIND,
    SORT_RECORD_TABLE,
    SORT_KEY_TABLE,
    PRINT_RECORDS_BY_KEYS,
    ADD_RECORD,
    DELETE_RECORD,
    COLLECT_INFO,
    PRINT_TABLE
} action_e;

#endif
