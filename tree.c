#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "utils.h"
#include "list.h"


struct tree
{
  tree_t *left;
  tree_t *right;
  goods_t *item;
};

