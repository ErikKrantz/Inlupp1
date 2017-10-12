#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "utils.h"
#include "list.h"
#include "lager.h"

//ett binärt sökträd av varor
// Trädet består av Varor och varje vara har en lista av Hyllor som lagrar information om plats och antal.

struct tree_node
{
  tree_node_t *left;
  tree_node_t *right;
  K key;
  T value;
};

struct tree_root
{
  tree_node_t *root;
};


tree_node_t *get_root(tree_root_t *tree)
{
  return tree->root;
}

tree_node_t *get_left(tree_node_t *tree)
{
  return tree->left;
}

tree_node_t *get_right(tree_node_t *tree)
{
  return tree->right;
}

T *get_value_root(tree_root_t *tree)
{
  return tree->root->value;
}

T *get_value_node(tree_node_t *tree)
{
  return tree->value;
}




/// Creates a new tree
///
/// \returns: empty tree
tree_root_t *tree_new()        // UNTESTED
{
  tree_root_t *empty_tree = calloc(1, sizeof(tree_root_t)); 
  empty_tree->root = calloc(1, sizeof(tree_node_t));
  return empty_tree;
}


/*
/// This function is used in tree_delete() to allow the trees which are
/// the sole owners of their data to free the data on delete.
typedef void(*tree_action)(K key, T elem);
*/


void tree_delete_node(tree_node_t *tree, tree_action cleanup)
{
  while(tree!=NULL)
    {
      tree_delete_node(tree->left, (tree_action) tree_delete_node);
      tree_delete_node(tree->right, (tree_action) tree_delete_node);
      free(tree);
    }
}

/// Remove a tree along with all T elements.
///
/// \param tree the tree
/// \param cleanup a function that takes a key and element as
///        argument, to be used to free memory. If this param is
///        NULL, no cleanup of keys or elements will happen.

void tree_delete(tree_root_t *tree, tree_action cleanup)  // UNTESTED
{
  while(tree->root!=NULL)
    {
      tree_delete_node(tree->root->left, (tree_action) tree_delete_node);
      tree_delete_node(tree->root->right,(tree_action) tree_delete_node);
      free(tree);
    }
}



int tree_size_node(tree_node_t *tree)
{
  int size = 0;
  if (tree==NULL)
    {
      return size;
    }
  ++size;
  return size + tree_size_node(tree->left) + tree_size_node(tree->right);
}


/// Get the size of the tree
///
/// \returns: the number of nodes in the tree

int tree_size(tree_root_t *tree)  // UNTESTED
{
  int size = 0;
  if (tree->root==NULL)
    {
      return size;
    }
  ++size;
  return size + tree_size_node(tree->root->left) + tree_size_node(tree->root->right);  
}


int tree_depth_node(tree_node_t *tree)
{
  int depth = 0;
  if (tree==NULL)
    {
      return depth;
    }
  else
    {
      ++depth;
      if (tree_depth_node(tree->left)>tree_depth_node(tree->right))
	{
	  return tree_depth_node(tree->left);
	}
      else
	{
	  return tree_depth_node(tree->right);
	}
    }
}


int tree_depth(tree_root_t *tree) // UNTESTED
{
  int depth = 0;
  if (tree->root==NULL)
    {
      return depth;
    }
  else
    {
      ++depth;
      if (tree_depth_node(tree->root->left)>tree_depth_node(tree->root->right))
	{
	  return tree_depth_node(tree->root->left);
	}
      else
	{
	  return tree_depth_node(tree->root->right);
	}
    }
}

bool tree_has_key_node(tree_node_t *tree, K key)
{
  if (tree==NULL)
    {
      return false;
    }
  if (!tree->value)
    {
      return false;
    }

  char *name = tree->key;

  if (!strcmp(name, key))
    {
      return true;
    }
  
  return tree_has_key_node(tree->left, key) || tree_has_key_node(tree->right, key);
}
/// Checks whether a key is used in a tree
///
/// \param tree pointer to the tree
/// \param key the key to check for inclusion in the tree
/// \returns: true if key is a key in the tree
bool tree_has_key(tree_root_t *tree, K key) // UNTESTED
{
  if (!tree->root->value)
    {
      return false;
    }
  char *name = tree->root->key;
  if (!strcmp(name, key))
    {
      return true;
    }
  else
    {
      return tree_has_key_node(tree->root->left, key) || tree_has_key_node(tree->root->right, key);
    }
}





K *get_key_node(tree_node_t *tree, K key)
{
  if (tree->key == key)
    {
      return &(tree->key);
    }
  else
    {
      if (tree_has_key_node(tree->left, key))
        {
          return get_key_node(tree->left, key);
        }
      if (tree_has_key_node(tree->right, key))
        {
          return get_key_node(tree->right, key);          
        }
    }
  return NULL;
}


K *get_key_root(tree_root_t *tree, K key)
{
  return get_key_node(tree->root, key);
}


// TESTED
bool lexi_comp(char *first, char *second)  //true if first string orders first lexigographically
{
  if(strcmp(first,second)==0)
    {
      printf("Both strings are equal, this function is not supposed to be called in this case\n");
      return false;
    }
  
  int shortest_key;
  if (strlen(first)>strlen(second))
    {
      shortest_key = strlen(second);
    }
  else
    {
      shortest_key = strlen(first);
    }

  for(int i=0; i<shortest_key; i++)
    {
      if ((int) first[i]>(int) second[i])         // jämför ascii-värden och lägg elem antingen till vänster eller till höger om nuvarande nod
	{
	  return false;
	}
      if ((int) first[i]<(int) second[i])
	{
	  return true;
	}
    }
  return true;
}

bool tree_insert_node(tree_node_t *tree, K key, T elem)
{
  if (tree->value==NULL)
    {
      tree->value = elem;
      tree->key = key;
    }
  else
    {
      char *node_key = tree->key;
      if (tree_has_key_node(tree,key))
	{
	  return false;
	}
      else
	{
	  if(lexi_comp(key, node_key))
	    {
	      if (tree->left == NULL)
		{
		  tree_node_t *left = calloc(1,sizeof(tree_node_t));
		  tree->left = left;
		}
	      tree_insert_node(tree->left, key, elem);
	    }
	  else
	    {
	      if (tree->right == NULL)
		{
		  tree_node_t *right = calloc(1,sizeof(tree_node_t));
		  tree->right = right;
		}
	      tree_insert_node(tree->right, key, elem);
	    }
	}
    }
  return true;
}


/// Insert element into the tree. Returns false if the key is already used.
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended
/// \param elem the element
/// \returns: true if successful, else false
bool tree_insert(tree_root_t *tree, K key, T elem) // UNTESTED
{
  if (tree->root->value == NULL)                     // if tree is empty
    {
      tree->root->key = key;
      tree->root->value = elem;                      // set tree's item to elem 
    }
  
  else 
    {
      char *node_key = tree->root->key;
      if (tree_has_key(tree,key))    // if item already exists, kollar alla subträd för varje kallning, gör bara koll på nuvarande nod istället
	{
	  return false;
	}
      else
	{
	  if(lexi_comp(key, node_key))
	    {
	      if (tree->root->left == NULL)
		{
		  tree_node_t *left = calloc(1,sizeof(tree_node_t));
		  tree->root->left = left;
		}
	      tree_insert_node(tree->root->left, key, elem);
	    }
	  else
	    {
	      if (tree->root->right == NULL)
		{
		  tree_node_t *right = calloc(1,sizeof(tree_node_t));
		  tree->root->right = right;
		}
	      tree_insert_node(tree->root->right, key, elem);
	    }
	}
    }
  return true;
}


T tree_get_node(tree_node_t *tree, K key)
{
  if (!tree->value)
    {
      return tree->value;
    }
  
  char *node_key = tree->key;
  if (strcmp(node_key, key) == 0)
    {
      return tree->value;
    }
  else
    {
      if (lexi_comp(node_key, key))
	{
	  return tree_get_node(tree->right, key);
	}
      else
	{
	  return tree_get_node(tree->left, key);
	}
    }
}

/// Returns the element for a given key in tree.
/// (The implementation may assume that the key exists, or reserve
/// e.g. NULL as a value to indicate failure)
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \returns: the element associated wity key key
T tree_get(tree_root_t *tree, K key)      // assumes that the key exists
{
  if (tree->root->value==NULL)
    {
      return NULL;
    }
  
  char *node_key = tree->root->key;
  if (strcmp(node_key, key)==0)       // if strings are equal:
    {
      return tree->root->value;           // return current nodes item 
    }
  else
    {
      if (lexi_comp(node_key, key))   // if node_key orders first lexicographically:
      {
        if (tree->root->right == NULL)
          {
            return NULL;
          }
	return tree_get_node(tree->root->right, key); // call tree_insert with tree->right   // SEG FAULT ^^^^^^^^^
      }
      else
      {
        if (tree->root->right == NULL)
          {
            return NULL;
          }
        return tree_get_node(tree->root->left, key);  // call tree_insert with tree-> left
      }
    }
}


T tree_remove(tree_root_t *tree, K key)
{
  return 0;
}


void node_keys(tree_node_t *node, K *arr, int index)
{
  if (node==NULL)
    {
      return;
    }
  node_keys(node->left, arr, index);
  index += tree_size_node(node->left);
  arr[index] = node->key;  //stod node->left->key
  ++index;
  node_keys(node->right, arr, index);
}

/// Returns an array holding all the keys in the tree
/// in ascending order.
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() keys
K *tree_keys(tree_root_t *tree)
{
  int size = tree_size(tree);
  K *arr = calloc(size, sizeof(K));
  if (tree->root->value==NULL)
    {
      return arr;
    }
  node_keys(tree->root, arr, 0);
  return arr;
}

void node_elements(tree_node_t *node, T *arr, int index)
{
  if (node==NULL)
    {
      return;
    }
  node_elements(node->left, arr, index);
  index += tree_size_node(node->left);
  arr[index] = node->left->value;
  ++index;
  node_elements(node->right, arr, index);
}

/// Returns an array holding all the elements in the tree
/// in ascending order of their keys (which are not part
/// of the value).
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() elements
T *tree_elements(tree_root_t *tree)
{
  int size = tree_size(tree);
  T *arr = calloc(size, sizeof(T));
  if (tree->root==NULL)
    {
      return arr;
    }
  node_elements(tree->root, arr, 0);
  return arr;
}


/// Applies a function to all elements in the tree in a specified order.
/// Example (using shelf as key):
///
///     tree_t *t = tree_new();
///     tree_insert(t, "A25", some_item);
///     int number = 0;
///     tree_apply(t, inorder, print_item, &number);
///
/// where print_item is a function that prints the number and increments it,
/// and prints the item passed to it.
///
/// \param tree the tree
/// \param order the order in which the elements will be visited
/// \param fun the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
void tree_apply(tree_root_t *tree, enum tree_order order, tree_action2 fun, void *data)
{
}



