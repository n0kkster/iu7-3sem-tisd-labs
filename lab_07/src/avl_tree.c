#include "avl_tree.h"

error_codes_e insert_avl_tree(void *vavl_tree, long el)
{
    avl_node_t *leaf, **avl_tree = (avl_node_t **)vavl_tree;
    error_codes_e rc;

    if (*avl_tree == NULL)
    {
        leaf = malloc(sizeof(avl_node_t));

        leaf->key = el;
        leaf->left = NULL;
        leaf->right = NULL;
        set_height(leaf);
        *avl_tree = leaf;
        return ERROR_SUCCESS;
    }

    if ((*avl_tree)->key == el)
        return ERROR_SUCCESS;

    if ((*avl_tree)->key < el)
    {
        if ((rc = insert_avl_tree(&(*avl_tree)->right, el)) == ERROR_SUCCESS)
            (*avl_tree)->right = balance((*avl_tree)->right);
        else
            return rc;
    }
    else
    {
        if ((rc = insert_avl_tree(&(*avl_tree)->left, el)) == ERROR_SUCCESS)
            (*avl_tree)->left = balance((*avl_tree)->left);
        else 
            return rc;
    }

    *avl_tree = balance(*avl_tree);

    return ERROR_SUCCESS;
}

void print_avl_tree_infix(avl_node_t *avl_tree)
{
    if (avl_tree)
    {
        print_avl_tree_infix(avl_tree->left);
        printf("%ld ", avl_tree->key);
        print_avl_tree_infix(avl_tree->right);
    }
}

void print_avl_tree_prefix(avl_node_t *avl_tree)
{
    if (avl_tree)
    {
        printf("%ld ", avl_tree->key);
        print_avl_tree_prefix(avl_tree->left);
        print_avl_tree_prefix(avl_tree->right);
    }
}

void print_avl_tree_postfix(avl_node_t *avl_tree)
{
    if (avl_tree)
    {
        print_avl_tree_postfix(avl_tree->left);
        print_avl_tree_postfix(avl_tree->right);
        printf("%ld ", avl_tree->key);
    }
}

void free_avl_tree(avl_node_t *avl_tree)
{
    if (avl_tree == NULL)
        return;

    free_avl_tree(avl_tree->left);
    free_avl_tree(avl_tree->right);

    free(avl_tree);
}

void *search_avl_tree(void *vavl_tree, long el)
{
    avl_node_t *avl_tree = (avl_node_t *)vavl_tree;

    if (avl_tree == NULL || avl_tree->key == el)
        return avl_tree;

    if (avl_tree->key < el)
        return search_avl_tree(avl_tree->right, el);
    else
        return search_avl_tree(avl_tree->left, el);
}

int calc_compares_avl_tree(avl_node_t *tree, long el, int compares)
{
    if (tree == NULL || tree->key == el)
        return compares;
    
    if (tree->key < el)
        return calc_compares_avl_tree(tree->right, el, compares + 1);
    else
        return calc_compares_avl_tree(tree->left, el, compares + 1);
}

void get_avl_tree_info(void *vavl_tree, tree_info_t *tree_info)
{
    int tmp;
    avl_node_t *tree = (avl_node_t *)vavl_tree;

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

void avl_tree_to_graph(void *vavl_tree, FILE *f)
{
    avl_node_t *avl_tree = (avl_node_t *)vavl_tree;

    if (avl_tree == NULL)
        return;

    if (avl_tree->left)
        fprintf(f, "\"%ld\" -> \"%ld\";\n", avl_tree->key, avl_tree->left->key);
    else if (avl_tree->right)
    {
        fprintf(f, "\"N%ld\" [style=\"invis\"];\n", avl_tree->key);
        fprintf(f, "\"%ld\" -> \"N%ld\" [style=\"invis\"];\n", avl_tree->key, avl_tree->key);
    }

    if (avl_tree->right)
        fprintf(f, "\"%ld\" -> \"%ld\";\n", avl_tree->key, avl_tree->right->key);
    else if (avl_tree->left)
    {
        fprintf(f, "\"N%ld\" [style=\"invis\"];\n", avl_tree->key);
        fprintf(f, "\"%ld\" -> \"N%ld\" [style=\"invis\"];\n", avl_tree->key, avl_tree->key);
    }

    avl_tree_to_graph(avl_tree->left, f);
    avl_tree_to_graph(avl_tree->right, f);
}

static avl_node_t *find_predecessor(avl_node_t *avl_tree, avl_node_t *node)
{
    avl_node_t *found;

    if (avl_tree == NULL)
        return NULL;

    if (avl_tree->left == node || avl_tree->right == node)
        return avl_tree;

    if ((found = find_predecessor(avl_tree->left, node)) != NULL)
        return found;
    else
        return find_predecessor(avl_tree->right, node);
}

static avl_node_t *get_max_node(avl_node_t *avl_tree)
{
    if (!avl_tree->right)
        return NULL;

    while (avl_tree->right)
        avl_tree = avl_tree->right;

    return avl_tree;
}

avl_node_t *balance_all(avl_node_t *avl_tree)
{
    if (!avl_tree)
        return NULL;

    avl_tree->left = balance_all(avl_tree->left);
    avl_tree->right = balance_all(avl_tree->right);

    return balance(avl_tree);
}

error_codes_e remove_node_avl_tree(void *vavl_tree, void *vnode)
{
    avl_node_t *max_node, *predecessor, *temp;
    avl_node_t *node = (avl_node_t *)vnode;
    avl_node_t **avl_tree = (avl_node_t **)vavl_tree;

    if (*avl_tree == node)
    {
        if (node->left == NULL && node->right == NULL)
        {
            free(node);
            *avl_tree = NULL;
            return ERROR_SUCCESS;
        }

        if (node->left && node->right == NULL)
        {
            *avl_tree = node->left;
            free(node);
            return ERROR_SUCCESS;
        }

        if (node->right && node->left == NULL)
        {
            *avl_tree = node->right;
            free(node);
            return ERROR_SUCCESS;
        }

        max_node = get_max_node(node->left);
        if (max_node == NULL)
        {
            max_node = node->left;
            *avl_tree = max_node;
            max_node->right = node->right;
            free(node);
            return ERROR_SUCCESS;
        }

        temp = find_predecessor(*avl_tree, max_node);

        *avl_tree = max_node;
        max_node->right = node->right;
        temp->right = max_node->left;
        max_node->left = node->left;
        free(node);
        return ERROR_SUCCESS;
    }

    predecessor = find_predecessor(*avl_tree, node);

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

    temp = find_predecessor(*avl_tree, max_node);

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

int get_height(avl_node_t *avl_tree)
{
    return avl_tree ? avl_tree->height : 0;
}

void set_height(avl_node_t *avl_tree)
{
    int lh = get_height(avl_tree->left);
    int rh = get_height(avl_tree->right);

    avl_tree->height = (lh > rh ? lh : rh) + 1;
}

int bal(avl_node_t *avl_tree)
{
    return get_height(avl_tree->right) - get_height(avl_tree->left);
}

avl_node_t *rotateright(avl_node_t *avl_tree)
{
    avl_node_t *left = avl_tree->left;
    avl_tree->left = left->right;
    left->right = avl_tree;
    set_height(avl_tree);
    set_height(left);
    return left;
}

avl_node_t *rotateleft(avl_node_t *avl_tree)
{
    avl_node_t *right = avl_tree->right;
    avl_tree->right = right->left;
    right->left = avl_tree;
    set_height(avl_tree);
    set_height(right);
    return right;
}

avl_node_t *balance(avl_node_t *avl_tree)
{
    set_height(avl_tree);
    if (bal(avl_tree) == 2)
    {
        if (bal(avl_tree->right) < 0)
            avl_tree->right = rotateright(avl_tree->right);
        return rotateleft(avl_tree);
    }
    if (bal(avl_tree) == -2)
    {
        if (bal(avl_tree->left) > 0)
            avl_tree->left = rotateleft(avl_tree->left);
        return rotateright(avl_tree);
    }
    return avl_tree;
}

