#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ENSURE(x, ...) do { error_codes_e rc; if ((rc = x) != ERROR_SUCCESS) {__VA_ARGS__ return rc;} } while(0);

#define FRAMERATE "2"
#define MEASURES 1000

typedef enum
{
    EXIT,
    READ_DATA_FROM_FILE,
    PRINT_GRAPH,
    FIND_EULERIAN_PATH,
    CHECK_CONNECTED,
    ADD_EDGE,
    REMOVE_EDGE,
    SPEEDTEST,
    DRAW_GRAPH,
    ACTION_COUNT
} action_e;

#include "errors.h"
#include "files.h"
#include "io.h"
#include "graph.h"

error_codes_e handle_read_data_from_file(graph_t *graph);
error_codes_e handle_draw_graph(graph_t *graph, char *filename);
void handle_find_eulerian_path(graph_t *graph);
void create_frame(graph_t *graph, FILE *f, int *path, int frame);
void handle_add_edge(graph_t *graph);
void handle_remove_edge(graph_t *graph);
error_codes_e create_animation(graph_t *graph, int *path, int path_len);

#endif /* ACTIONS_H */
