#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "lager.h"
#include "utils.h"
#include "list.h"

//ett binärt sökträd av varor 
// saknas typ för node? kolla i headerfil.
// Trädet består av Varor och varje vara har en lista av Hyllor som lagrar information om plats och antal. 
/*
struct tree_node
{
  tree_node_t *left;
  tree_node_t *right;
  goods_t *item;              // goods_t ist? 
};
*/

struct tree             
{
  tree_t *left;
  tree_t *right;
  goods_t *item;
};


/// Creates a new tree
///
/// \returns: empty tree
tree_t *tree_new()        // UNTESTED
{
  tree_t *empty_tree = calloc(1, sizeof(tree_t));
  empty_tree->item=NULL;
  return empty_tree;
}


/*
/// This function is used in tree_delete() to allow the trees which are
/// the sole owners of their data to free the data on delete.
typedef void(*tree_action)(K key, T elem);
*/



/// Remove a tree along with all T elements.
///
/// \param tree the tree
/// \param cleanup a function that takes a key and element as
///        argument, to be used to free memory. If this param is
///        NULL, no cleanup of keys or elements will happen.

void tree_delete(tree_t *tree, tree_action cleanup)  // UNTESTED
{
  while(tree!=NULL)
    {
      tree_delete(tree->left, (tree_action) tree_delete);
      tree_delete(tree->right,(tree_action) tree_delete);
      free(tree);
    }  
}


/// Get the size of the tree
///
/// \returns: the number of nodes in the tree

int tree_size(tree_t *tree)  // UNTESTED
{
  int size = 0;
  if (tree==NULL)
    {
      return size;
    }
  else
    {
      ++size;
      return size + tree_size(tree->left) + tree_size(tree->right);
    }
}



int tree_depth(tree_t *tree) // UNTESTED
{
  int depth = 0;
  if (tree==NULL)
    {
      return depth;
    }
  else
    {
      ++depth;
      if (tree_depth(tree->left)>tree_depth(tree->right))
	{
	  return tree_depth(tree->left);
	}
      else
	{
	  return tree_depth(tree->right);
	}
    }
}


/// Checks whether a key is used in a tree
///
/// \param tree pointer to the tree
/// \param key the key to check for inclusion in the tree
/// \returns: true if key is a key in the tree
bool tree_has_key(tree_t *tree, K key) // UNTESTED
{
  char *name = get_name(tree->item);                       
  if (strcmp(name, key) == 0)
    {
      return true;
    }
  else
    {
      return tree_has_key(tree->left, key) || tree_has_key(tree->right, key);
    }
}


// UNTESTED
bool lexi_comp(char *first, char *second)  //true if first string orders first lexigographically
{
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
  printf("Both strings are equal, this function is not supposed to be called in this case\n");
  return false;
}


/// Insert element into the tree. Returns false if the key is already used.
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended
/// \param elem the element
/// \returns: true if successful, else false
bool tree_insert(tree_t *tree, K key, T elem) // UNTESTED
{
  char *node_key = get_name(tree->item);       
  if (tree_has_key(tree,key))            // if item already exists:
    {
      if (strcmp(node_key, key)==0)      // if strings are equal:
	{
	  
	                                 // add 1 to amount of item (find it first)
	}
      else
	{
	  if (lexi_comp(node_key, key))  // if node_key orders first lexicographically:
	    {
	                                 // call tree_insert with tree->right
	    }
	  else
	    {
	                                  // call tree_insert with tree-> left
	    }
	}
      // dont forget to add element too.
      return false;
    }
  else
    {
  
    }
}


/// Returns the element for a given key in tree.
/// (The implementation may assume that the key exists, or reserve
/// e.g. NULL as a value to indicate failure)
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \returns: the element associated wity key key
/*T tree_get(tree_t *tree, K key)      // assumes that the key exists
{
  char *node_key = get_name(tree->item);
  if (strcmp(node_key, key)==0)       // if strings are equal:
    {
      return *(tree->item);           // return current nodes item 
    }
  else
    {
      if (lexi_comp(node_key, key))   // if node_key orders first lexicographically:
	{
          tree_get(tree->right, key); // call tree_insert with tree->right
	}
      else
	{
	  tree_get(tree->left, key);  // call tree_insert with tree-> left
	}
    }
}
*/

T tree_remove(tree_t *tree, K key)
{
  return 0;
}


K *tree_keys(tree_t *tree)
{
  return 0;
}


T *tree_elements(tree_t *tree)
{
  return 0;
}


void tree_apply(tree_t *tree, enum tree_order order, tree_action2 fun, void *data)
{
}
