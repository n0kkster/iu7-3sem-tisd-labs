#include "actions.h"

error_codes_e handle_read_data_from_file(graph_t *graph)
{
    char *buffer;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(read_data_from_file(buffer, graph), free(buffer););
    free(buffer);

    return ERROR_SUCCESS;
}

error_codes_e handle_draw_graph(graph_t *graph, char *filename)
{
    FILE *f;
    char *buffer;
    const char *format = "dot -T svg graphs/%s.gv -o graphs/%s.svg";

    buffer = malloc((strlen(filename) + 1 + 3 + 7) * sizeof(char));
    if (!buffer)
        return ERROR_ALLOCATING_MEM;

    sprintf(buffer, "graphs/%s.gv", filename);

    if ((f = fopen(buffer, "w")) == NULL)
        return ERROR_OPENING_FILE;

    fprintf(f, "digraph MyGraph {\n");
    fprintf(f, "rankdir=\"LR\";\n");
    draw_graph(graph, f);
    fprintf(f, "}");
    fclose(f);

    buffer = realloc(buffer, (strlen(format) + 2 * strlen(filename) + 1) * sizeof(char));
    if (!buffer)
        return ERROR_ALLOCATING_MEM;

    sprintf(buffer, format, filename, filename);

    system(buffer);
    free(buffer);

    printf("Граф создан в файле graphs/%s.svg\n", filename);

    return ERROR_SUCCESS;
}

void handle_add_edge(graph_t *graph)
{
    int src, dst;
    puts("Введите две вершины графа, между котороми будет создана двухсторонная дорога:");

    if (scanf("%d%d", &src, &dst) != 2)
    {
        puts("Некорректный ввод!");
        return;
    }

    getchar();

    insert_edge(graph, src, dst);
}

void handle_remove_edge(graph_t *graph)
{
    int src, dst;
    puts("Введите две вершины графа, между котороми будет удалена двухсторонная дорога:");

    if (scanf("%d%d", &src, &dst) != 2)
    {
        puts("Некорректный ввод!");
        return;
    }

    getchar();

    remove_edge(graph, src, dst);
    remove_edge(graph, dst, src);
}

void reverse_arr(int *arr, int len)
{
    int t;
    for (int i = 0; i < len / 2; i++)
    {
        t = arr[i];
        arr[i] = arr[len - i - 1];
        arr[len - i - 1] = t;
    }
}

void handle_find_eulerian_path(graph_t *graph)
{
    graph_t *temp = copy_graph(graph);
    int *path, path_size, i;

    path = find_eulerian_path(temp, &path_size);

    if (path_size == 0)
    {
        printf("В графе нет Эйлерова пути!\n");
        free_graph(&temp);
        return;
    }

    reverse_arr(path, path_size);

    for (i = 0; i < path_size - 1; i++)
        printf("%d -> ", path[i]);
    printf("%d\n", path[i]);

    create_animation(graph, path, path_size);

    free_graph(&temp);
    free(path);
}

error_codes_e create_animation(graph_t *graph, int *path, int path_len)
{
    FILE *f;
    char *buffer;
    const char *format = "dot -T png graphs/frames/frame_%.3d.gv -o graphs/frames/frame_%.3d.png";

    for (int frame = 0; frame < path_len - 1; frame++)
    {   
        buffer = malloc((strlen("graphs/frames/frame_.gv") + 1 + 10) * sizeof(char));
        if (!buffer)
            return ERROR_ALLOCATING_MEM;

        sprintf(buffer, "graphs/frames/frame_%.3d.gv", frame);

        if ((f = fopen(buffer, "w")) == NULL)
            return ERROR_OPENING_FILE;

        create_frame(graph, f, path, frame);

        fclose(f);

        buffer = realloc(buffer, (strlen(format) + 2 * 10 + 1) * sizeof(char));
        if (!buffer)
            return ERROR_ALLOCATING_MEM;

        sprintf(buffer, format, frame, frame);

        system(buffer);
        free(buffer);  
    }

    system("ffmpeg -y -hide_banner -loglevel error -framerate "FRAMERATE" -i graphs/frames/frame_%03d.png -c:v libx264 -r 30 graphs/animation.mp4");

    printf("Анимация создана в файле graphs/animation.mp4\n");

    return ERROR_SUCCESS;
}

void create_frame(graph_t *graph, FILE *f, int *path, int frame)
{
    assoc_node_t *head_a = graph->adjList->head;
    node_t *head_b;

    fprintf(f, "digraph MyGraph {\n");
    fprintf(f, "rankdir=\"LR\";\n");

    while (head_a)
    {
        head_b = head_a->vertex->value;
        while (head_b)
        {
            fprintf(f, "\"%d\" -> \"%d\"", head_a->vertex->key, head_b->v);

            for (int ft = 0; ft <= frame; ft++)
                if (path[ft] == head_a->vertex->key && path[ft + 1] == head_b->v)
                    fprintf(f, "[color=\"red\"]");

            fprintf(f, ";\n");

            head_b = head_b->next;
        }
        head_a = head_a->next;
    }

    fprintf(f, "}");
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

error_codes_e handle_speedtest(void)
{
    graph_t *graph;
    int *path, path_size;
    unsigned long long beg, end;
    double time = 0;

    for (size_t i = 0; i < MEASURES; i++)
    {
        if (create_graph(&graph) != ERROR_SUCCESS)
            return ERROR_ALLOCATING_MEM;
        ENSURE(read_data_from_file("do_not_remove.txt", graph), free_graph(&graph););

        beg = micros();
        path = find_eulerian_path(graph, &path_size);
        end = micros();
        time += end - beg;

        free(path);
        free_graph(&graph);
    }

    time /= (double)MEASURES;

    printf("Время поиска Эйлерова пути в графе: %.3lf мкс.\n", time);

    return ERROR_SUCCESS;
}
