#include "list.h"
#include "utils.h"
#include "tree.h"
#include <stdlib.h>
  
struct shelf_entry
{
  char *shelf;
  int amount;
};

struct link
{
  L element;
  link_t *next;
};

struct list
{
  link_t *first;
  link_t *last;
};

//create/return empty list
list_t *list_new()
{
  return calloc(1, sizeof(list_t));
}

link_t *link_new(L element, link_t *next)
{
  link_t *result = malloc(sizeof(link_t));
  if (result)
    {
      result->element = element;
      result->next = next;
    }
  return result;
}
		 
/// Inserts a new element at the end of the list
///
/// \param list pointer to the list
/// \param elem the element to be appended
void list_append(list_t *list, L element)
{
  /// check if the list is empty
  if (list->last == NULL)
    {
      ///Append on an empty list is the same as prepend
      list_prepend(list,element);
    }
  else
    {
      /// Insert element at the end of the list
      link_t *link = link_new(element, NULL);
      list->last->next = link;
      /// Update the last pointer to point to the new last element
      list->last = link;
    } 
}



/// Inserts a new element at the beginning of the list
///
/// \param list pointer to the list
/// \param elem the element to be prepended
void list_prepend(list_t *list, L element)
{
  link_t *oldfirst = list->first;
  list->first = link_new(element, list->first);
  list->first->next = oldfirst;
  // Få pekaren till första elementet
  // Skapa en ny länk: dess next ska vara pekaren till första elementet som vi nyss tog fram.
  // ändra list->first till en pekare till det nya elementet  
}

int list_length(list_t *list)
{
  int length = 0;
  while (list->first->next!=NULL)
    {
      list->first=list->first->next;
      ++length;
    }
  return length;
}

/// Inserts a new element at a given index.
///
/// Example:
///
/// list_t *l = list_new(); // l == []
/// list_insert(l, 0, e1);  // l == [e1]
/// list_insert(l, 0, e2);  // l == [e2, e1]
/// list_insert(l, 1, e3);  // l == [e2, e3, e1]
/// list_insert(l, 5, e4);  // l == [e2, e3, e1]
///
/// The last case fails (and returns false) because
/// size is 3, meaning 5 is not a valid index.
///
/// Note that insert at index 0 is the same as prepend
/// and insert index size is the same as append.
///
/// Negative indexes should be supported:
///
/// list_insert(l, -1, e4);       // l == [e2, e3, e1, e4]
///
/// A positive index can be calculated from a
/// negative like this: pos_i = size + 1 + neg_i.
///
/// \param list  pointer to the list
/// \param index the index for elem to be inserted at
/// \param elem  the element to be inserted
/// \returns true if succeeded, else false
bool list_insert(list_t *list, int index, L elem)
{
  int list_siz = list_length(list);

  if (index>=list_siz)                          // Kollar om indexet ryms i listan.
    {
      return false;
    }
  
  if (index<0)                                     // omvandlar negativa index till positiva index
    {
      index = list_siz + 1 + index;
    }

  link_t *currentelement = list->first;            // initiera nuvarande element
  for (int i=0; i<index; ++i)      
    {
      currentelement = currentelement->next;       // Traversar listan tills vi är framme där elem ska sättas in
    }

  link_t *oldnext = currentelement->next;
  link_t *new_elem = link_new(elem, calloc(1, sizeof(link_t)));
  currentelement->next = new_elem;
  currentelement->next->next = oldnext;

  return true;
  //currentelements-> next ska peka på elem och elems next ska peka på currentelements gamla next
}

/// Removes an element from a list.
///
/// Example: (assume l == [e2, e3, e1, e4])
///
/// L elem;
/// list_remove(l, 1, &elem);  // l = [e2, e1, e4], elem == e3
/// list_remove(l, -1, &elem); // l = [e2, e1], elem == e4
///
/// \param list  pointer to the list
/// \param index the index to be removed
/// \param elem a pointer to where the element can be stored
/// \returns true if succeeded, else false
bool list_remove(list_t *list, int index, L *elem)
{
  link_t *currentelement = list->first;
  for (int i=0; i<index; ++i)
    {
      currentelement = currentelement->next;
    }

  free(currentelement);
  return true;
}


/// Returns the element at a given index
/// \param list  pointer to the list
/// \param index the index to be returned
/// \returns a pointer to the element at index index
L *list_get(list_t *list, int index)
{
  int list_siz = list_length(list);
  
  if (index<0)                                     // omvandlar negativa index till positiva index
    {
      index = list_siz + 1 + index;
    }
 
  link_t *currentelement = list->first;
  for (int i=0; i<index; i++)
    {
      currentelement = currentelement->next;
    }
  return &currentelement;
}


/// A convenience for list_get(list, 0)
L *list_first(list_t *list)
{
  list_get(list, 0);
}


/// A convenience for list_get(list, -1)
L *list_last(list_t *list)
{
  list_get(list, -1);
}
