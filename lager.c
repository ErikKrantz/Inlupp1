#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "tree.h"
#include "list.h"
#include <time.h>

struct goods
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};

typedef struct goods goods_t;

void print_goods(goods_t *goods)
{
  printf("Name:  %s\n", goods->name);
  printf("Desc:  %s\n", goods->desc);
  int priceore = goods->price % 100;
  int pricekr = goods->price/100;
  printf("Price: %d.%d SEK\n", pricekr, priceore);
  //printf("Shelf: %s\n", goods->list.shelf);        // Kan finnas på flera hyllor nu, tänk om.
}


/*
test för list_new/list_append som ej är klart.
item_instances_t item = {};
print_goods(list_append(list_new(), item)); 
 */


void add_goods()
{
 
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
}

void menu_choice(char c, char *menu)
{
  if (toupper(c)=='L')
    {
      add_goods();
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

  /*
  char *menu = "Vad vill du göra?\n";
  char menu_c = ask_question_char(menu);
  menu_choice(menu_c, menu);
  */
  
  return 0;
}
