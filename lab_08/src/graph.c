#include "graph.h"

error_codes_e create_graph(graph_t **graph)
{
    *graph = malloc(sizeof(graph_t));
    if (!graph)
        return ERROR_ALLOCATING_MEM;

    (*graph)->vertices = 0;
    (*graph)->edges = 0;
    (*graph)->adjList = assoc_array_create();

    if (!(*graph)->adjList)
        return ERROR_ALLOCATING_MEM;

    return ERROR_SUCCESS;
}

void insert_edge(graph_t *graph, int src, int dst)
{
    assoc_node_t *vnode;
    graph->vertices += assoc_array_insert(graph->adjList, src, dst);
    graph->vertices += assoc_array_insert(graph->adjList, dst, src);

    vnode = find(graph->adjList->head, src);
    vnode->vertex->in_degree++;
    vnode->vertex->out_degree++;

    vnode = find(graph->adjList->head, dst);
    vnode->vertex->in_degree++;
    vnode->vertex->out_degree++;

    graph->edges += 2;
}

void mark_visited(graph_t *graph, int v)
{
    assoc_node_t *head = graph->adjList->head;

    while (head)
    {
        if (head->vertex->key == v)
        {
            head->vertex->visited = true;
            return;
        }
        head = head->next;
    }
}

bool is_visited(graph_t *graph, int v)
{
    return find(graph->adjList->head, v)->vertex->visited;
}

void reset_visited_marks(graph_t *graph)
{
    assoc_node_t *head = graph->adjList->head;

    while (head)
    {
        head->vertex->visited = false;
        head = head->next;
    }
}

void dfs(graph_t *graph, int v)
{
    node_t *temp;

    mark_visited(graph, v);

    temp = find(graph->adjList->head, v)->vertex->value;

    while (temp)
    {
        if (!is_visited(graph, temp->v))
            dfs(graph, temp->v);

        temp = temp->next;
    }
}

void dfs_euler(graph_t *graph, int u, int *path, int *index)
{
    vertex_t *vertex = find(graph->adjList->head, u)->vertex;
    int v;

    while (vertex && vertex->value)
    {
        v = vertex->value->v;
        remove_edge(graph, u, v);
        dfs_euler(graph, v, path, index);
    }

    path[(*index)++] = u;
}

void remove_edge(graph_t *graph, int src, int dst)
{
    assoc_node_t *head = graph->adjList->head;
    node_t *curr, *old;

    while (head)
    {
        if (head->vertex->key == src)
        {
            curr = head->vertex->value;

            if (curr->v == dst)
            {
                old = curr;
                head->vertex->value = head->vertex->value->next;
                free(old);
                graph->edges--;
                break;
            }

            while (curr->next)
            {
                if (curr->next->v == dst)
                {
                    old = curr->next;
                    curr->next = curr->next->next;
                    free(old);
                    graph->edges--;
                    break;
                }
                curr = curr->next;
            }
        }
        head = head->next;
    }

    // if (graph->adjList->head->vertex->value == NULL)
    // {
    //     temp = graph->adjList->head;
    //     free_vertex(graph->adjList->head->vertex);
    //     graph->adjList->head = graph->adjList->head->next;
    //     graph->vertices--;
    //     free(temp);
    // }

    // head = graph->adjList->head;

    // while (head->next)
    // {   
    //     if (head->next->vertex->value == NULL)
    //     {
    //         temp = head->next->next;
    //         free_vertex(head->next->vertex);
    //         free(head->next);
    //         head->next = temp;
    //         graph->vertices--;
    //     }
    //     head = head->next;
    // }
}

bool is_eulerian(graph_t *graph)
{
    assoc_node_t *head = graph->adjList->head;

    int ins = 0, outs = 0, eq = 0;

    while (head)
    {
        if (head->vertex->in_degree - head->vertex->out_degree == 1)
            outs++;
        
        if (head->vertex->out_degree - head->vertex->in_degree == 1)
            ins++;

        if (head->vertex->in_degree == head->vertex->out_degree)
            eq++;

        head = head->next;
    }

    return ins <= 1 && outs <= 1 && eq + ins + outs == graph->vertices;
}

bool is_connected(graph_t *graph)
{
    int start = -1;
    assoc_node_t *head = graph->adjList->head;

    while (head)
    {
        if (head->vertex->out_degree > 0)
        {
            start = head->vertex->key;
            break;
        }
    }

    if (start == -1)
        return true;

    dfs(graph, start);

    head = graph->adjList->head;

    while (head)
    {

        if (!head->vertex->visited)
            return false;
        head = head->next;
    }

    reset_visited_marks(graph);

    return true;
}

int *find_eulerian_path(graph_t *graph, int *path_size)
{   
    int start = -1, index = 0;
    assoc_node_t *head = graph->adjList->head;
    int *path = malloc(graph->edges * 2 * sizeof(int));

    if (!path)
    {
        *path_size = 0;
        return NULL;
    }

    if (!is_connected(graph))
    {
        printf("Граф несвязный!\n");
        *path_size = 0;
        free(path);
        return NULL;
    }

    if (!is_eulerian(graph))
    {
        printf("В графе невозможно построить Эйлеров путь!\n");
        *path_size = 0;
        free(path);
        return NULL;
    }

    while (head)
    {
        if (head->vertex->out_degree - head->vertex->in_degree == 1)
        {
            start = head->vertex->key;
            break;
        }
        head = head->next;
    }

    if (start == -1)
        start = graph->adjList->head->vertex->key;

    dfs_euler(graph, start, path, &index);

    *path_size = index;
    return path;
}

void print_list(node_t *head)
{
    while (head)
    {
        printf("%d -> ", head->v);
        head = head->next;
    }
    printf("NULL\n");
}

void print_graph(graph_t *graph)
{
    assoc_node_t *head = graph->adjList->head;
    while (head)
    {
        printf("%d -> ", head->vertex->key);
        print_list(head->vertex->value);
        head = head->next;
    }

    printf("Кол-во вершин: %d\n", graph->vertices);
    printf("Кол-во ребер: %d\n", graph->edges);
}

void free_graph(graph_t **graph)
{
    if (!*graph)
        return;

    free_assoc_list(&(*graph)->adjList->head);

    free((*graph)->adjList);
    free(*graph);
    *graph = NULL;
}

void draw_graph(graph_t *graph, FILE *f)
{
    assoc_node_t *head_a = graph->adjList->head;
    node_t *head_b;

    while (head_a)
    {
        head_b = head_a->vertex->value;
        while (head_b)
        {
            fprintf(f, "\"%d\" -> \"%d\";\n", head_a->vertex->key, head_b->v);
            head_b = head_b->next;
        }
        head_a = head_a->next;
    }
}

graph_t *copy_graph(graph_t *graph)
{
    graph_t *new = malloc(sizeof(graph_t));
    new->vertices = graph->vertices;
    new->adjList = assoc_array_create();
    new->adjList->head = copy_assoc_list(graph->adjList->head);
    new->edges = graph->edges;

    return new;
}


