#include "actions.h"

error_codes_e handle_read_tree(node_t **tree)
{
    char *buffer;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(read_tree_from_file(buffer, tree), free(buffer););
    free(buffer);

    return ERROR_SUCCESS;
}

error_codes_e handle_insert_node(node_t **tree)
{
    char *buffer;
    long el;

    puts("Введите целое число: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);
    ENSURE(insert(tree, el),);

    return ERROR_SUCCESS;
}

error_codes_e handle_find_node(node_t *tree)
{
    char *buffer;
    long el;
    node_t *found;

    puts("Введите целое число, которое должно быть найдено: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);

    found = search(tree, el);
    if (found)
    {
        printf("Элемент найден! Адрес ноды: %p\n", (void *)found);

        if (found->left)
            printf("Значение левого потомка: %ld\n", found->left->key);
        else
            printf("Левого потомка нет!\n");
        
        if (found->right)
            printf("Значение правого потомка: %ld\n", found->right->key);
        else
            printf("Правого потомка нет!\n");
    }
    else
        puts("Элемент не найден!");
    
    return ERROR_SUCCESS;
}

error_codes_e handle_print_tree_info(node_t *tree)
{
    tree_info_t tree_info = { 0 };
    tree_info.realloc_factor = 1;
    tree_info.nodes_on_depth = calloc(sizeof(*tree_info.nodes_on_depth), BLOCK_SIZE);

    if (!tree_info.nodes_on_depth)
        return ERROR_ALLOCATING_MEM;

    get_tree_info(tree, &tree_info);

    for (int i = 0; i < tree_info.tree_depth; i++)
    {
        printf("Глубина вершины: %d\n", i);
        printf("Количество узлов на глубине %d: %d\n", i, tree_info.nodes_on_depth[i]);
    }

    free(tree_info.nodes_on_depth);

    return ERROR_SUCCESS;
}

error_codes_e handle_create_graph(node_t *tree)
{
    FILE *f;

    if ((f = fopen("graph.gv", "w")) == NULL)
        return ERROR_OPENING_FILE;

    fprintf(f, "digraph Tree {\n");
    tree_to_graph(tree, f);
    fprintf(f, "}");
    fclose(f);

    system("dot -T svg graph.gv -o graph.svg");

    return ERROR_SUCCESS;
}

error_codes_e handle_remove_node(node_t **tree)
{
    char *buffer;
    long el;
    node_t *found;

    puts("Введите целое число, которое должно быть удалено: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);

    found = search(*tree, el);
    
    if (found == NULL)
    {
        puts("Такого элемента нет в дереве!");
        return ERROR_SUCCESS;
    }

    ENSURE(remove_node(tree, found),);
    return ERROR_SUCCESS;
}

error_codes_e handle_gen_random_file(void)
{
    long min, max, count;
    char *buffer;
    FILE *f;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer),);

    if ((f = fopen(buffer, "w")) == NULL)
        return ERROR_OPENING_FILE;

    puts("Введите нижнюю границу генерации: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &min), free(buffer); fclose(f););
    free(buffer);

    puts("Введите верхнюю границу генерации: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &max), free(buffer); fclose(f););
    free(buffer);

    puts("Введите количество чисел: ");
    ENSURE(read_string(stdin, &buffer),);
    ENSURE(validate_long(buffer, &count), free(buffer); fclose(f););
    free(buffer);

    for (long i = 0; i < count; i++)
        fprintf(f, "%ld ", rand() % (max - min + 1) + min);
    fprintf(f, "\n");

    fclose(f);
    return ERROR_SUCCESS;
}

unsigned long long micros(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

error_codes_e handle_speedtest(void)
{
    FILE *f;
    node_t *tree = NULL;
    char *buffer;
    unsigned long beg, end;
    double time_add_file = 0, time_add_tree = 0;
    long num, max = 100, min = -100;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer),);

    if ((f = fopen(buffer, "w")) == NULL)
    {
        free(buffer);
        return ERROR_OPENING_FILE;
    }
    fclose(f);

    for (size_t i = 0; i < MEASURES; i++)
    {
        num = rand() % (max - min + 1) + min;
        beg = micros();
        f = fopen(buffer, "w");
        fprintf(f, "%ld ", num);
        fclose(f);
        end = micros();
        time_add_file += end - beg;
    }
    time_add_file /= MEASURES;

    for (size_t i = 0; i < MEASURES; i++)
    {
        num = rand() % (max - min + 1) + min;
        // num = i;
        beg = micros();
        insert(&tree, num);
        end = micros();
        time_add_tree += end - beg;
    }
    time_add_tree /= MEASURES;
    free_tree(tree);
    free(buffer);

    printf("Время вставки элемента в дерево: %lf мкс.\n", time_add_tree);
    printf("Время вставки элемента в файл: %lf мкс.\n", time_add_file);

    return ERROR_SUCCESS;
}
