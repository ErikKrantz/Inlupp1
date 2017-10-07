#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "tree.h"
#include "list.h"
#include <time.h>

struct shelf_entry
{
  char *shelf
  int amount
};

typedef struct shelf_entry shelf_entry_t

struct goods
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};

typedef struct goods goods_t;

char *get_name(goods_t *item)
{
  return item->name;
}

void print_goods(goods_t *goods)
{
  printf("Name:  %s\n", goods->name);
  printf("Desc:  %s\n", goods->desc);
  int priceore = goods->price % 100;
  int pricekr = goods->price/100;
  printf("Price: %d.%d SEK\n", pricekr, priceore);
  int amount = tally_amount(goods->list);
  printf("Amount: %d\n", amount);
  // print shelves too....
}


/*
test för list_new/list_append som ej är klart.
item_instances_t item = {};
print_goods(list_append(list_new(), item)); 
 */


void add_goods(tree_root_t *tree)
{
  goods_t *item = calloc(1, sizeof(goods_t));
  char *name = ask_question_string("Välj namn för vara\n");
  item->name = name;

  char *desc = ask_question_string("Välj beskrivning för din vara\n");
  item->desc = desc;

  int price = ask_question_int("Sätt ett pris\n");
  item->price = price;
  
  list_t *list = list_new();
  item->list = list;
  
  printf("Break 1\n");
  
  shelf_entry_t *shelf_elem = calloc(1, sizeof(shelf_entry_t));
  // link_t *next = NULL;                          // I om att listan är tom så är next == NULL
  // link_t *new_link = calloc(1, sizeof(link_t)); // lager.c vet nog inte sizeof link_t             Alternativ 1 (troligtvis fel)
  // link_t *new_link = link_new(shelf_elem, next) // Här behöver lager.c inte veta sizeof link_t    Alternativ 2 (troligtvis rätt)
  // new_link->elem = shelf_elem;
  // new_link->next = next; 
  
  printf("Break 2\n");
  
  list_prepend(list, shelf_elem);                  // sätter in elementet i början av listan
  
  printf("Break 3\n");
    
  char *shelf = ask_question_string("Välj hyllplats\n");
  shelf_elem->shelf = shelf
  
  printf("Break 4\n");
  
  int amount = ask_question_int("Hur många exemplar av varan vill du lägga till?\n");
  shelf_elem->amount = amount;
  
  printf("Break 5\n");

  if(tree_insert(tree, name, item))
    {
    printf("Det lyckades!\n");
    }
  else
    {
      printf("Något gick fel, lägg till din vara igen.\n");
    }
}
}

void remove_goods()
{
}

void edit_goods()
{
}

void list_goods()
{
}

// Vad är poängen?? Gör väl samma som print_goods Vad för argument?
void display_goods(goods_t *goods)
{

}

void undo_action()
{
}

void exit_program()
{
  printf("Du har kallat på exit_program\n");
  // return;
}

void menu_choice(char c, char *menu)
{
  if (toupper(c)=='L')
    {
      tree_root_t emptytree = NULL;
      add_goods(emptytree);
    }

  if (toupper(c)=='T')
    {
      remove_goods();
    }

  if (toupper(c)=='R')
    {
      edit_goods();
    }

  if (toupper(c)=='G')
    {
      undo_action();
    }

  if (toupper(c)=='H')
    {
      list_goods();
    }

  if (toupper(c)=='A')
    {
      printf("Du har kommit till if-satsen i menu_choice\n");
      exit_program();
    }
  else
    {
      c = ask_question_char(menu);                    // printar frågan en gång för mkt
      menu_choice(c, menu);
    }
}


int main()
{
  puts("[L]ägga till en vara");
  puts("[T]a bort en vara");
  puts("[R]edigera en vara");
  puts("Ån[g]ra senaste ändringen");
  puts("Ändra [h]ela varukatalogen");
  puts("[A]vsluta");

  shelf_entry_t shelf1;
  shelf1->shelf="A25";
  shelf1->amount=2;

  shelf_entry shelf2;
  shelf2->shelf="B25";
  shelf2->amount=3;

  link_t *link2 = link_new(shelf2, NULL);
  link_t *link1 = link_new(shelf1, link2);

  list_t list;
  list->first = link1;
  list->last = link2;

  goods_t *item;
  item->name="Tvål";
  item->desc="Rengör";
  item->price=2990;
  item->list=list;

  print_goods(item);
  return 0;
}


/*
  char *firststring = "ABC";
  char *secondstring = "ABCD";
  if (lexi_comp(firststring, secondstring))
    {
      printf("True\n");
    }
  else
    {
      printf("False\n");
    }
*/

/*
  char *menu = "Vad vill du göra?\n";
  char menu_c = ask_question_char(menu);
  menu_choice(menu_c, menu);
*/
