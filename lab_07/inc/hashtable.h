#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "errors.h"

#define MAX_COLLISIONS 4
                         
typedef struct list_node_t
{
    long el;
    struct list_node_t *next;
} list_node_t;

typedef enum
{
    OPEN,
    CLOSED
} hashing_e;

typedef struct 
{
    list_node_t **data;
    hashing_e hashing;
    long size;
} hashtable_t;

long calc_hash(hashtable_t *ht, long x);
error_codes_e insert_hashtable(void *vht, long el);
void free_hashtable(hashtable_t *ht);
void print_hashtable(hashtable_t *ht);
int insert_to_list(list_node_t **head, long el);
void remove_from_list(list_node_t **head, list_node_t *el);
list_node_t *find_in_list(list_node_t *head, long el, int *cmps);
void print_list(list_node_t *head);
void free_list(list_node_t **head);
void init_hashtable(hashtable_t **ht, hashing_e hashing, long size);
void *find_hashtable(void *vht, long el);
int calc_compares_hashtable(hashtable_t *ht, long el);
error_codes_e remove_from_hashtable(void *vht, void *vnode);
void *search_hashtable(void *vht, long el);
void hashtable_to_graph(void *vht, FILE *f);
void remove_closed(hashtable_t *ht, long hash, long el);
list_node_t *find_closed(hashtable_t *ht, long hash, long el, int *cmps);
hashtable_t *restruct_closed(hashtable_t *ht);
hashtable_t *restruct_open(hashtable_t *ht);
size_t calc_hashtable_size(hashtable_t *ht);
int calc_list_len(list_node_t *head);
long next_prime(long num);

#endif /* HASHTABLE_H */
