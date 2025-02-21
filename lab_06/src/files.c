#include "files.h"

error_codes_e read_tree_from_file(char *filename, node_t **tree)
{
    FILE *f;
    error_codes_e rc;
    long el;
    int read = 0;
    
    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;

    while ((fscanf(f, "%ld", &el)) == 1)
    {
        read++;
        if ((rc = insert(tree, el)) != ERROR_SUCCESS)
        {
            free_tree(*tree);
            return rc;
        }
    }
    if (!feof(f))
        return ERROR_READING_FILE;
    fclose(f);

    if (!read)
        return ERROR_READING_FILE;

    return ERROR_SUCCESS;
}
