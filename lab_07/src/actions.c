#include "actions.h"

error_codes_e handle_read_data(void *data, error_codes_e (*insert)(void *, long))
{
    char *buffer;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(read_data_from_file(buffer, data, insert), free(buffer););
    free(buffer);

    return ERROR_SUCCESS;
}

error_codes_e handle_read_hashtable(hashtable_t **ht, error_codes_e (*insert)(void *, long), hashing_e hashing)
{
    char *buffer;
    long els_count;
    
    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer), );
    els_count = count_nums_in_file(buffer);

    free_hashtable(*ht);
    init_hashtable(ht, hashing, els_count);

    ENSURE(read_data_from_file(buffer, ht, insert), free(buffer););
    free(buffer);

    return ERROR_SUCCESS;
}

void convert_bst_to_avl(node_t *tree, avl_node_t **avl_tree)
{
    if (tree)
    {
        convert_bst_to_avl(tree->left, avl_tree);
        convert_bst_to_avl(tree->right, avl_tree);
        insert_avl_tree(avl_tree, tree->key);   
    }
}

error_codes_e handle_insert_node(void *data, error_codes_e (*insert)(void *, long))
{
    char *buffer;
    long el;

    puts("Введите целое число: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);
    insert(data, el);

    return ERROR_SUCCESS;
}

error_codes_e handle_find_node(void *data, void *(*search)(void *, long), data_type_e dt)
{
    char *buffer;
    long el;
    void *found;

    puts("Введите целое число, которое должно быть найдено: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);

    found = search(data, el);
    if (found)
    {
        switch (dt)
        {
            case BST:
                printf("Элемент найден! Адрес ноды: %p\n", found);
                if (((node_t *)found)->left)
                    printf("Значение левого потомка: %ld\n", ((node_t *)found)->left->key);
                else
                    printf("Левого потомка нет!\n");

                if (((node_t *)found)->right)
                    printf("Значение правого потомка: %ld\n", ((node_t *)found)->right->key);
                else
                    printf("Правого потомка нет!\n");
                break;
            case AVL:
                printf("Элемент найден! Адрес ноды: %p\n", found);
                if (((avl_node_t *)found)->left)
                    printf("Значение левого потомка: %ld\n", ((avl_node_t *)found)->left->key);
                else
                    printf("Левого потомка нет!\n");

                if (((avl_node_t *)found)->right)
                    printf("Значение правого потомка: %ld\n", ((avl_node_t *)found)->right->key);
                else
                    printf("Правого потомка нет!\n");
                break;
            case HASHTABLE:
                break;
            default:
                break;
        }
    }
    else
        puts("Элемент не найден!");

    return ERROR_SUCCESS;
}

error_codes_e handle_print_tree_info(void *tree, void (*tree_info_fn)(void *, tree_info_t *))
{
    tree_info_t tree_info = {0};
    tree_info.realloc_factor = 1;
    tree_info.nodes_on_depth = calloc(sizeof(*tree_info.nodes_on_depth), BLOCK_SIZE);

    if (!tree_info.nodes_on_depth)
        return ERROR_ALLOCATING_MEM;

    tree_info_fn(tree, &tree_info);

    for (int i = 0; i < tree_info.tree_depth; i++)
    {
        printf("Глубина вершины: %d\n", i);
        printf("Количество узлов на глубине %d: %d\n", i, tree_info.nodes_on_depth[i]);
    }

    free(tree_info.nodes_on_depth);

    return ERROR_SUCCESS;
}

error_codes_e handle_create_graph(void *data, const char *filename, void (*data_to_graph)(void *, FILE *))
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

    fprintf(f, "digraph Tree {\n");
    data_to_graph(data, f);
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

error_codes_e handle_remove_node(void *tree, void *(*search)(void *, long), error_codes_e (*remove_node)(void *, void *), data_type_e dt)
{
    char *buffer;
    long el;
    void *found = NULL;

    puts("Введите целое число, которое должно быть удалено: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(validate_long(buffer, &el), free(buffer););
    free(buffer);

    switch (dt)
    {
        case BST:
            found = search(*((node_t **)tree), el);
            break;
        case AVL:
            found = search(*((avl_node_t **)tree), el);
            break;
        case HASHTABLE:
            found = search(tree, el);
            break;
    }

    if (found == NULL)
    {
        puts("Такого элемента нет в дереве!");
        return ERROR_SUCCESS;
    }

    ENSURE(remove_node(tree, found), );
    return ERROR_SUCCESS;
}

error_codes_e handle_gen_random_file(void)
{
    long min, max, count;
    char *buffer;
    FILE *f;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer), );

    if ((f = fopen(buffer, "w")) == NULL)
        return ERROR_OPENING_FILE;

    free(buffer);

    puts("Введите нижнюю границу генерации: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(validate_long(buffer, &min), free(buffer); fclose(f););
    free(buffer);

    puts("Введите верхнюю границу генерации: ");
    ENSURE(read_string(stdin, &buffer), );
    ENSURE(validate_long(buffer, &max), free(buffer); fclose(f););
    free(buffer);

    puts("Введите количество чисел: ");
    ENSURE(read_string(stdin, &buffer), );
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
    avl_node_t *avl_tree = NULL;
    hashtable_t *ht_open, *ht_closed;
    char *buffer;
    unsigned long beg, end;
    double time_add_file = 0, time_add_tree = 0, time_add_avl_tree = 0;
    double time_add_hashtable_open = 0, time_add_hashtable_closed = 0;
    double time_search_hashtable_open = 0, time_search_hashtable_closed = 0;
    double compares_tree = 0, compares_avl_tree = 0, compares_hashtable_open = 0, compares_hashtable_closed = 0;
    long num, max = 2000, min = -2000;

    puts("Введите имя файла: ");
    ENSURE(read_string(stdin, &buffer), );

    if ((f = fopen(buffer, "w")) == NULL)
    {
        free(buffer);
        return ERROR_OPENING_FILE;
    }
    fclose(f);

    init_hashtable(&ht_open, OPEN, next_prime((long)(MEASURES * 1.3)));
    init_hashtable(&ht_closed, CLOSED, next_prime((long)(MEASURES * 1.3)));

    for (long i = 0; i < MEASURES; i++)
    {
        num = rand() % (max - min + 1) + min;
        // num = i;

        beg = micros();
        f = fopen(buffer, "a");
        fprintf(f, "%ld ", num);
        fclose(f);
        end = micros();
        time_add_file += end - beg;

        beg = micros();
        insert_tree(&tree, num);
        end = micros();
        time_add_tree += end - beg;

        beg = micros();
        insert_avl_tree(&avl_tree, num);
        end = micros();
        time_add_avl_tree += end - beg;

        beg = micros();
        insert_hashtable(&ht_open, num);
        end = micros();
        time_add_hashtable_open += end - beg;

        beg = micros();
        insert_hashtable(&ht_closed, num);
        end = micros();
        time_add_hashtable_closed += end - beg;
    }

    time_add_file /= MEASURES;
    time_add_tree /= MEASURES;
    time_add_avl_tree /= MEASURES;
    time_add_hashtable_open /= MEASURES;
    time_add_hashtable_closed /= MEASURES;

    for (size_t i = 0; i < MEASURES; i++)
    {
        num = rand() % (max - min + 1) + min;

        compares_tree += calc_compares_tree(tree, num, 0);
        compares_avl_tree += calc_compares_avl_tree(avl_tree, num, 0);
        compares_hashtable_open += calc_compares_hashtable(ht_open, num);
        compares_hashtable_closed += calc_compares_hashtable(ht_closed, num);
    }

    compares_tree /= MEASURES;
    compares_avl_tree /= MEASURES;
    compares_hashtable_open /= MEASURES;
    compares_hashtable_closed /= MEASURES;
    
    puts("\nСреднее время вставки элемента: ");
    printf("Дерево: %.3lf мкс.\n", time_add_tree);
    printf("Сбалансированное дерево: %.3lf мкс.\n", time_add_avl_tree);
    printf("Хеш таблица с открытым хешированием: %.3lf мкс.\n", time_add_hashtable_open);
    printf("Хеш таблица с закрытым хешированием: %.3lf мкс.\n", time_add_hashtable_closed);
    printf("Файл: %.3lf мкс.\n", time_add_file);

    puts("\nСреднее количество сравнений при поиске: ");
    printf("Дерево: %.3lf\n", compares_tree);
    printf("Сбалансированное дерево: %.3lf\n", compares_avl_tree);
    printf("Хеш таблица с открытым хешированием: %.3lf\n", compares_hashtable_open);
    printf("Хеш таблица с закрытым хешированием: %.3lf\n", compares_hashtable_closed);

    puts("\nЗанимаемая память: ");
    printf("Хеш таблица с открытым хешированием: %zu\n", calc_hashtable_size(ht_open));
    printf("Хеш таблица с закрытым хешированием: %zu\n", calc_hashtable_size(ht_closed));

    printf("-------------------------------------------------------------\n");
    printf("|    Открытое хеширование     |     Закрытое хеширование    |\n");
    printf("------------------------------|------------------------------\n");
    printf("|    Размер    |  Время, мкс. |    Размер    |  Время, мкс. |\n");

    for (size_t size = 100; size < 1001; size += 100)
    {
        free_hashtable(ht_open);
        free_hashtable(ht_closed);
        
        init_hashtable(&ht_open, OPEN, size);
        init_hashtable(&ht_closed, CLOSED, size);

        for (size_t i = 0; i < size; i++)
        {
            num = rand() % (max - min + 1) + min;
            insert_hashtable(&ht_open, num);
            insert_hashtable(&ht_closed, num);
        }

        for (size_t i = 0; i < MEASURES; i++)
        {
            num = rand() % (max - min + 1) + min;

            beg = micros();
            search_hashtable(ht_open, num);
            end = micros();
            time_search_hashtable_open += end - beg;

            beg = micros();
            search_hashtable(ht_closed, num);
            end = micros();
            time_search_hashtable_closed += end - beg;
        }

        time_search_hashtable_open /= MEASURES;
        time_search_hashtable_closed /= MEASURES;

        printf("|--------------|--------------|--------------|--------------|\n");
        printf("%-7s%-8ld%-6s%-9.3lf%-6s%-9ld%-6s%-9.3lf%s\n", \
        "|", ht_open->size, "|", time_search_hashtable_open, "|", ht_closed->size, "|", time_search_hashtable_closed, "|");        
    }

    printf("-------------------------------------------------------------\n");

    free_tree(tree);
    free_avl_tree(avl_tree);
    free_hashtable(ht_open);
    free_hashtable(ht_closed);
    free(buffer);

    return ERROR_SUCCESS;
}
