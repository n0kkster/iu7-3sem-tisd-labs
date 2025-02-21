#include "files.h"

error_codes_e read_data_from_file(char *filename, graph_t *graph)
{
    FILE *f;
    int s, d;

    if ((f = fopen(filename, "r")) == NULL)
        return ERROR_OPENING_FILE;

    while (fscanf(f, "%d%d", &s, &d) == 2)
        insert_edge(graph, s, d);

    fclose(f);

    if (graph->vertices == 0)
        return ERROR_READING_FILE;

    return ERROR_SUCCESS;
}

bool arr_find(int *arr, int val, size_t arr_size)
{
    for (size_t i = 0; i < arr_size; i++)
        if (arr[i] == val)
            return true;
    return false;
}