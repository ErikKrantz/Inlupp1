#ifndef __lager_h__
#define __lager_h__

#include <stdbool.h>


typedef struct goods goods_t;

typedef struct shelf_entry shelf_entry_t;

typedef struct action action_t;

//shelf_entry_t copy_shelf(shelf_entry_t shelf);

char *get_name(goods_t *item);

#endif
