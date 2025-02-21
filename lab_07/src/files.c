#include "files.h"

error_codes_e read_data_from_file(char *filename, void *data, error_codes_e (*insert)(void *, long))
{
    FILE *f;
    long el;
    int read = 0;
    error_codes_e rc;
    
    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;

    while ((fscanf(f, "%ld", &el)) == 1)
    {
        read++;
        if ((rc = insert(data, el)) != ERROR_SUCCESS)
            return rc;
    }

    if (!feof(f))
        return ERROR_READING_FILE;
    
    fclose(f);

    if (!read)
        return ERROR_READING_FILE;

    return ERROR_SUCCESS;
}

long count_nums_in_file(char *filename)
{
    FILE *f;
    long el;
    long read = 0;
    
    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;

    while ((fscanf(f, "%ld", &el)) == 1)
        read++;

    if (!feof(f))
        return ERROR_READING_FILE;
    
    fclose(f);

    return read;
}
