#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include "errors.h"
#include "tree.h"

error_codes_e read_tree_from_file(char *filename, node_t **tree);

#endif /* FILES_H */
