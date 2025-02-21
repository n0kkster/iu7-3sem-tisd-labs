#include "tree.h"

error_codes_e insert(node_t **tree, long el)
{
    node_t *leaf, *temp;

    if (*tree == NULL)
    {
        leaf = malloc(sizeof(node_t));

        leaf->key = el;
        leaf->left = NULL;
        leaf->right = NULL;
        *tree = leaf;
        return ERROR_SUCCESS;
    }

    temp = *tree;

    if (temp->key == el)
        return ERROR_SUCCESS;

    if (temp->key < el)
        return insert(&temp->right, el);
    else
        return insert(&temp->left, el); 
}

void print_tree_infix(node_t *tree)
{
    if (tree)
    {
        print_tree_infix(tree->left);
        printf("%ld ", tree->key);
        print_tree_infix(tree->right);
    }
}

void print_tree_prefix(node_t *tree)
{
    if (tree)
    {
        printf("%ld ", tree->key);
        print_tree_prefix(tree->left);
        print_tree_prefix(tree->right);
    }
}

void print_tree_postfix(node_t *tree)
{
    if (tree)
    {
        print_tree_postfix(tree->left);
        print_tree_postfix(tree->right);
        printf("%ld ", tree->key);
    }
}

void free_tree(node_t *tree)
{
    if (tree == NULL)
        return;

    free_tree(tree->left);
    free_tree(tree->right);

    free(tree);
}

node_t* search(node_t *tree, long el)
{
    if (tree == NULL || tree->key == el)
        return tree;
    
    if (tree->key < el)
        return search(tree->right, el);
    else
        return search(tree->left, el);
}

void get_tree_info(node_t *tree, tree_info_t *tree_info)
{
    int tmp;

    if (tree == NULL)
        return;

    if (tree_info->curr_depth >= tree_info->realloc_factor * BLOCK_SIZE)
    {
        tree_info->nodes_on_depth = realloc(tree_info->nodes_on_depth, BLOCK_SIZE * ++tree_info->realloc_factor);
        for (int i = BLOCK_SIZE * (tree_info->realloc_factor - 1); i < BLOCK_SIZE * tree_info->realloc_factor; i++)
            tree_info->nodes_on_depth[i] = 0;
    }
    
    tree_info->nodes_on_depth[tree_info->curr_depth]++;
    tree_info->curr_depth++;

    tmp = tree_info->curr_depth;
    tree_info->tree_depth = MAX(tree_info->curr_depth, tree_info->tree_depth);

    get_tree_info(tree->left, tree_info);
    tree_info->curr_depth = tmp;
    get_tree_info(tree->right, tree_info);
}

void tree_to_graph(node_t *tree, FILE *f)
{
    if (tree == NULL)
        return;
    
    if (tree->left)
        fprintf(f, "\"%ld\" -> \"%ld\";\n", tree->key, tree->left->key);
    
    if (tree->right)
        fprintf(f, "\"%ld\" -> \"%ld\";\n", tree->key, tree->right->key);

    tree_to_graph(tree->left, f);
    tree_to_graph(tree->right, f);
}

node_t* find_predecessor(node_t *tree, node_t *node)
{
    node_t *found;

    if (tree == NULL)
        return NULL;

    if (tree->left == node || tree->right == node)
        return tree;

    if ((found = find_predecessor(tree->left, node)) != NULL)
        return found;
    else
        return find_predecessor(tree->right, node);
}

node_t* get_max_node(node_t *tree)
{
    if (!tree->right)
        return NULL;
    
    while (tree->right)
        tree = tree->right;

    return tree;
}

error_codes_e remove_node(node_t **tree, node_t *node)
{
    node_t *max_node, *predecessor, *temp;

    if (*tree == node)
    {
        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            *tree = NULL;
            return ERROR_SUCCESS;
        }

        if (node->left && node->right == NULL)
        {
            *tree = node->left;
            free(node);
            return ERROR_SUCCESS;
        }

        if (node->right && node->left == NULL)
        {
            *tree = node->right;
            free(node);
            return ERROR_SUCCESS;
        }

        max_node = get_max_node(node->left);
        if (max_node == NULL)
        {
            max_node = node->left;
            *tree = max_node;
            max_node->right = node->right;
            free(node);
            return ERROR_SUCCESS;
        }

        temp = find_predecessor(*tree, max_node);

        *tree = max_node;
        max_node->right = node->right;
        temp->right = max_node->left;
        max_node->left = node->left;
        free(node);
        return ERROR_SUCCESS;
    }

    predecessor = find_predecessor(*tree, node);

    if (node->left == NULL && node->right == NULL)
    {
        if (predecessor->left == node)
            predecessor->left = NULL;
        else
            predecessor->right = NULL;
        
        free(node);
        
        return ERROR_SUCCESS;
    }

    if (node->left && node->right == NULL)
    {
        if (predecessor->left == node)
            predecessor->left = node->left;
        else
            predecessor->right = node->left;

        free(node);
        return ERROR_SUCCESS;
    }

    if (node->right && node->left == NULL)
    {
        if (predecessor->left == node)
            predecessor->left = node->right;
        else
            predecessor->right = node->right;

        free(node);
        return ERROR_SUCCESS;
    }

    max_node = get_max_node(node->left);
    if (max_node == NULL)
    {
        max_node = node->left;
        if (predecessor->left == node)
            predecessor->left = max_node;
        else
            predecessor->right = max_node;
        max_node->right = node->right;
        free(node);
        return ERROR_SUCCESS;
    }

    temp = find_predecessor(*tree, max_node);

    if (predecessor->left == node)
        predecessor->left = max_node;
    else
        predecessor->right = max_node;

    max_node->right = node->right;
    temp->right = max_node->left;
    max_node->left = node->left;
    free(node);

    return ERROR_SUCCESS;
}
