#include "hashtable.h"

bool is_prime(long num) 
{
    if (num <= 1) 
        return false;

    if (num <= 3) 
        return true;

    if (num % 2 == 0 || num % 3 == 0) 
        return false;

    for (int i = 5; i <= sqrt(num); i += 6) 
        if (num % i == 0 || num % (i + 2) == 0) 
            return false;

    return true;
}

long next_prime(long num) 
{
    if (num <= 1) 
        return 2;

    while (!is_prime(++num));

    return num;
}

long calc_hash(hashtable_t *ht, long x)
{
    return ((x % ht->size) + ht->size) % ht->size;
}

void insert_closed(hashtable_t **ht, long hash, long el)
{
    long c = 0;
    while (c < MAX_COLLISIONS)
    {
        if ((*ht)->data[(hash + c) % (*ht)->size] == NULL)
        {
            (*ht)->data[(hash + c) % (*ht)->size] = malloc(sizeof(list_node_t));
            (*ht)->data[(hash + c) % (*ht)->size]->el = el;
            (*ht)->data[(hash + c) % (*ht)->size]->next = NULL;
            return;
        }

        if ((*ht)->data[(hash + c) % (*ht)->size]->el == el)
            return;
        c++;
    }

    *ht = restruct_closed(*ht);
    insert_closed(ht, calc_hash(*ht, el), el);
}

hashtable_t *restruct_closed(hashtable_t *ht)
{
    long new_size = next_prime((long)(ht->size * 1.4));
    // printf("Делаем реструктуризацию! Старый размер: %ld, новый: %ld\n", ht->size, new_size);
    hashtable_t *ht_new;
    init_hashtable(&ht_new, ht->hashing, new_size);

    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
            insert_closed(&ht_new, calc_hash(ht_new, ht->data[i]->el), ht->data[i]->el);
    }
    
    free_hashtable(ht);

    return ht_new;
}

hashtable_t *restruct_open(hashtable_t *ht)
{
    long new_size = next_prime((long)(ht->size * 1.4));
    // printf("Делаем реструктуризацию! Старый размер: %ld, новый: %ld\n", ht->size, new_size);
    hashtable_t *ht_new;
    list_node_t *head;
    long hash;

    init_hashtable(&ht_new, ht->hashing, new_size);

    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
        {
            head = ht->data[i];
            while (head)
            {
                hash = calc_hash(ht_new, head->el);
                insert_to_list(&ht_new->data[hash], head->el);
                head = head->next;
            }
        }
    }
    
    free_hashtable(ht);

    return ht_new;
}

error_codes_e insert_hashtable(void *vht, long el)
{
    hashtable_t **ht = (hashtable_t **)vht;
    long hash = calc_hash(*ht, el);

    if ((*ht)->hashing == OPEN)
    {
        if ((insert_to_list(&(*ht)->data[hash], el)) > MAX_COLLISIONS)
            *ht = restruct_open(*ht);
    }
    else
        insert_closed(ht, hash, el);

    return ERROR_SUCCESS;
}

error_codes_e remove_from_hashtable(void *vht, void *vnode)
{
    hashtable_t *ht = (hashtable_t *)vht;

    list_node_t *node = (list_node_t *)vnode;
    long hash = calc_hash(ht, node->el);

    if (ht->hashing == OPEN)
        remove_from_list(&ht->data[hash], node);
    else
        remove_closed(ht, hash, node->el);

    return ERROR_SUCCESS;
}

void remove_closed(hashtable_t *ht, long hash, long el)
{
    long c = 0;
    while (ht->data[(hash + c) % ht->size]->el != el)
        c++;
    
    free(ht->data[(hash + c)]);
    ht->data[(hash + c)] = NULL;
}

void *search_hashtable(void *vht, long el)
{
    hashtable_t *ht = (hashtable_t *)vht;

    int cmps;
    long hash = calc_hash(ht, el);
    list_node_t *node;
    if (ht->hashing == OPEN)
        node = find_in_list(ht->data[hash], el, &cmps);
    else
        node = find_closed(ht, hash, el, &cmps);

    return node;
}

list_node_t *find_closed(hashtable_t *ht, long hash, long el, int *cmps)
{
    int c = 0;    
    list_node_t *curr = NULL, *found = NULL;

    *cmps = 1;

    while (ht->data[(hash + c) % ht->size])
    {
        c++;
        curr = ht->data[(hash + c) % ht->size];
        if (curr && curr->el == el)
        {
            found = curr;
            break;
        }

        (*cmps)++;
    }

    return found;
}

void *find_hashtable(void *vht, long el)
{
    hashtable_t *ht = (hashtable_t *)vht;

    int cmps;
    long hash = calc_hash(ht, el);
    list_node_t *node;
    
    if (ht->hashing == OPEN)
        node = find_in_list(ht->data[hash], el, &cmps);
    else
        node = find_closed(ht, hash, el, &cmps);

    if (node)
        printf("Элемент найден! Хеш элемента: %ld, адрес в ноде: %p, количество сравнений: %d\n", hash, (void *)node, cmps);
    else
        printf("Элемент не найден!\n");

    return node;
}

int calc_compares_hashtable(hashtable_t *ht, long el)
{
    int cmps;
    long hash = calc_hash(ht, el);

    if (ht->hashing == OPEN)
        find_in_list(ht->data[hash], el, &cmps);
    else
        find_closed(ht, hash, el, &cmps);

    return cmps;
}

void print_hashtable(hashtable_t *ht)
{
    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
        {   
            printf("|%zu| -> ", i);
            if (ht->hashing == OPEN)
                print_list(ht->data[i]);
            else
                printf("%ld\n", ht->data[i]->el);
        }
    }
}

void hashtable_to_graph(void *vht, FILE *f)
{
    hashtable_t *ht = (hashtable_t *)vht;
    list_node_t *head;
    bool is_first = true;

    fprintf(f, "rankdir=LR\n");
    fprintf(f, "node [ shape=record ];\n");
    
    fprintf(f, "struct1 [\n");
    fprintf(f, "\tlabel=\"");

    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
        {
            if (is_first)
            {
                is_first = false;
                fprintf(f, "<port%zu>%zu", i, i);
            }        
            else
                fprintf(f, "|<port%zu>%zu", i, i);
        }
    }

    fprintf(f, "\";\n");
    fprintf(f, "]");

    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
        {
            fprintf(f, "subgraph cluster_record_%zu {\n", i);
            fprintf(f, "\tnode [ shape=square ];\n");
            fprintf(f, "\t%ld", ht->data[i]->el);

            head = ht->data[i]->next;
            while (head)
            {
                fprintf(f, "->%ld", head->el);
                head = head->next;
            }
            fprintf(f, ";\n}\n");

            fprintf(f, "edge [ style=dashed ];\n");
            fprintf(f, "struct1:port%zu->\"%ld\";\n", i, ht->data[i]->el);
        }
    }
}

size_t calc_hashtable_size(hashtable_t *ht)
{
    size_t size = sizeof(*ht);
    list_node_t *head;
    for (long i = 0; i < ht->size; i++)
    {
        if (ht->hashing == OPEN)
        {
            head = ht->data[i];
            size += sizeof(head);
            while (head)
            {
                size += sizeof(*head);
                head = head->next;
            }
        }
        else
            size += sizeof(int);
    }

    return size;
}

void free_hashtable(hashtable_t *ht)
{
    for (long i = 0; i < ht->size; i++)
    {
        if (ht->data[i])
            free_list(&ht->data[i]);
    }
    free(ht->data);
    free(ht);
}

void init_hashtable(hashtable_t **ht, hashing_e hashing, long size)
{
    *ht = malloc(sizeof(hashtable_t));
    (*ht)->size = size;
    (*ht)->hashing = hashing;

    if (size)
    {
        (*ht)->data = malloc(sizeof(list_node_t *) * (*ht)->size);
        for (long i = 0; i < (*ht)->size; i++)
            (*ht)->data[i] = NULL;
    }
}

void free_list(list_node_t **head)
{
    list_node_t *old;

    while (*head)
    {
        old = *head;
        (*head) = (*head)->next;
        free(old);
    }
    *head = NULL;
}

void print_list(list_node_t *head)
{
    while (head)
    {
        printf("%ld (%p) -> ", head->el, (void *)head);
        head = head->next;
    }
    printf("NULL\n");
}

int insert_to_list(list_node_t **head, long el)
{
    list_node_t *new;
    int cmps;

    if (!find_in_list(*head, el, &cmps))
    {
        new = malloc(sizeof(list_node_t));
        new->next = *head;
        new->el = el;
        *head = new;
    }

    return calc_list_len(*head);
}

int calc_list_len(list_node_t *head)
{
    int len = 0;
    while (head)
    {
        head = head->next;
        len++;
    }
    return len;
}

void remove_from_list(list_node_t **head, list_node_t *el)
{
    list_node_t *old, *temp;

    if (!el || !head)
        return;

    if (*head == el)
    {
        old = *head;
        *head = (*head)->next;
        free(old);
        return;
    }

    temp = *head;

    while (temp && temp->next)
    {
        if (temp->next == el)
        {
            old = temp->next->next;
            free(temp->next);
            temp->next = old;
        }
        temp = temp->next;
    }
}

list_node_t *find_in_list(list_node_t *head, long el, int *cmps)
{
    *cmps = 1;

    while (head)
    {
        if (head->el == el)
            return head;
        
        (*cmps)++;
        head = head->next;
    }

    return head;
}
