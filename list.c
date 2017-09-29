#include "list.h"
#include "utils.h"
#include "tree.h"
#include "lager.h"
#include <stdlib.h>

//en dynamisk lista av lagerplatser

struct list
{
  int amount;
  char *shelf; // ändra typ till något mer passande eller ha kontrollfunktion för varje shelf
};

//create/return empty list
list_t *list_new()
{
  return calloc(1, sizeof(list_t));
}

//L är en voic-pekare till en hyllplats?
void list_append(list_t *list, L elem)
{
  
}

void list_prepend(list_t *list, L elem)
{
}

bool list_insert(list_t *list, int index, L elem)
{
  
  return true; // STUB, CHANGE 
}

bool list_remove(list_t *list, int index, L *elem)
{
  return true; // STUB, CHANGE
}

L *list_get(list_t *list, int index)
{
  return 0;   // STUB, CHANGE
}

L *list_first(list_t *list)
{
  return 0;   // STUB, CHANGE
}

L *list_last(list_t *list)
{
  return 0;   // STUB, CHANGE
}

int list_length(list_t *list)
{
  return 0;   // STUB, CHANGE
}

void list_delete(list_t *list, list_action cleanup)
{
}

void list_apply(list_t *list, list_action2 fun, void *data)
{
}
