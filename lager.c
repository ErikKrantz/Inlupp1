#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "tree.h"
#include "list.h"
#include "lager.h"
#include <time.h>
  
struct shelf_entry
{
  char *shelf;
  int amount;
};

struct goods
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};

struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  goods_t *merch;
  goods_t copy;
};

void print_menu()
{
  char *menu_choices = "[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukatalogen\n[A]vsluta\n";
  puts(menu_choices);
}

void get_shelf_aux(link_t *link)
{
  shelf_entry_t *shelf = (shelf_entry_t *) get_element(link);
  char *shelfname = shelf->shelf;
  link_t *nextlink = (link_t *) get_next(link);
  puts(shelfname);
  if (nextlink!=NULL)
    {
      get_shelf_aux((link_t *)get_next(link));
    }
}

void get_shelf(goods_t *item)
{
  link_t *firstlink = (link_t *) get_first(item->list);
  link_t *nextlink = (link_t *) get_next(firstlink);
  shelf_entry_t *shelf = (shelf_entry_t *) get_element(firstlink);
  char *currentshelf = shelf->shelf;
  printf("%s \n", currentshelf);
  if (nextlink!=NULL)
    {
      get_shelf_aux(nextlink);
    }
}

char *get_name(goods_t *item)
{
  return item->name;
}

void print_goods(goods_t *item)
{
  if (item==NULL)
    {
      puts("Din vara hittades inte.");
    }
  else
    {
      printf("Name:  %s\n", item->name);
      printf("Desc:  %s\n", item->desc);
      int priceore = item->price % 100;
      int pricekr = item->price/100;
      printf("Price: %d.%d SEK\n", pricekr, priceore);
      int amount = tally_amount(item->list);
      printf("Amount: %d\n", amount);
      get_shelf(item);
      puts("\n");
    }
}

int tally_amount(list_t *list)
{
  link_t *cursor = get_first(list); 
  int length = list_length(list);
  shelf_entry_t *element = (shelf_entry_t*)  get_element(cursor);
  int amount = element->amount;
  for (int i=0; i<length-1; i++)
    {
      cursor = get_next(cursor);
      shelf_entry_t *element = (shelf_entry_t*) get_element(cursor);
      int amount2 = element->amount;
      amount += amount2;
    }
  return amount;
}

bool shelf_occupied_aux(char *input, tree_node_t *tree)
{
  if (!get_value_node(tree))
    {
      return false;
    }
  goods_t *item = (goods_t *) get_value_node(tree);
  list_t *lista = (item->list);
  shelf_entry_t *shelf_entry = (shelf_entry_t *) list_first(lista);
  char *shelf = shelf_entry->shelf;
  if(!strcmp(input,shelf))
    {
      return true;
    }

  if (!get_right(tree))
    {
      if (!get_left(tree))
        {
          return false;
        }
      return shelf_occupied_aux(input, get_left(tree));
    }
  return shelf_occupied_aux(input, get_right(tree));
}

bool shelf_occupied(char *input, tree_root_t *tree)
{
  if (!get_value_root(tree))
    {
      return false;
    }
  
  goods_t *item = (goods_t *) get_value_root(tree);
  list_t *lista = (item->list);
  shelf_entry_t *shelf_entry = (shelf_entry_t *) list_first(lista);

  char *shelf = shelf_entry->shelf;
  
  // mellan denna kommentar och bp5.5 -> seg fault när man lägger till en andra vara.
  if(!strcmp(input, shelf)) // det är här man får seg fault, antagligen shelf som är NULL lr nåt sånt.
    {
      return true;
    }
  else
    {
      return shelf_occupied_aux(input, get_root(tree)); 
    }
}


void add_goods(tree_root_t *tree)
{
  goods_t *item = calloc(1, sizeof(goods_t));
  char *name = ask_question_string("Välj namn för varan\n");
  item->name = name;
  
  char *desc = ask_question_string("Välj beskrivning för din vara\n");
  item->desc = desc;

  int price = ask_question_int("Sätt ett pris\n");
  item->price = price;

  list_t *list = list_new();
  item->list = list;
  
  bool occupied = true;
  shelf_entry_t *shelf_elem = calloc(1, sizeof(shelf_entry_t));
  char *shelf;

  do
    {
      shelf = ask_question_string("Vilken hyllplats? T.ex. A25\n");
      occupied = shelf_occupied(shelf, tree);
    }while(occupied);
  
  shelf_elem->shelf = shelf;
  list_prepend(list, shelf_elem);

  int amount = ask_question_int("Hur många exemplar av varan vill du lägga till?\n");
  shelf_elem->amount = amount;

  if(tree_insert(tree, name, item))       
    {
    printf("Det lyckades!\n\n");
    }
  else
    {
      printf("Något gick fel, lägg till din vara igen.\n\n");
    }
}


void remove_goods()
{
}

void list_goods_aux(tree_root_t *tree, int index, int tree_siz)
{
  char *arr[20];
  for (int i=0; i<index+20; index++)
    {
      if (index==tree_siz+1)
        {
          return;
        }
      arr[i] = (char *) tree_keys(tree)[index];
      printf("%d. %s\n", index+1, arr[i]);
    }
  
  char *c = ask_question_string("Vill du lista fler varor? [Y]?\n");
  if (strcmp(c,"Y")==0 || strcmp(c,"y")==0)
    {
      list_goods_aux(tree, index, tree_siz);
    }
}

void list_goods(tree_root_t *tree)
{
  int tree_siz = tree_size(tree);
  char *arr[20];
  int i;
  for(i=0; i<20; i++)
    {
      arr[i] = (char *) tree_keys(tree)[i];
      printf("%d. %s\n", i+1, arr[i]);
      
      if(i+1==tree_siz)
        {
          break;
        }
    }
  char *c = ask_question_string("Vill du lista fler varor? [Y]?\n");
  if (strcmp(c,"Y")==0 || strcmp(c,"y")==0)
    {
      list_goods_aux(tree, i, tree_siz);
    }
}



void display_goods(tree_root_t *tree)
{
  char *item = ask_question_string("Vilken vara vill du visa?\n");
  print_goods(tree_get(tree,item));
}

goods_t copy_item(goods_t *item)
{
  goods_t copy;
  copy.name = item->name;
  copy.desc = item->desc;
  copy.price = item->price;
  copy.list = item->list;   // måste antagligen gå in i listans links och kopiera elementen.
  return copy;
}


void list_menu(goods_t *item, tree_root_t *tree, action_t undo) 
{
 top:
  puts("Ändra [N]amn");
  puts("Ändra [B]eskrivning");
  puts("Ändra [P]ris");
  puts("Ändra [M]ängd");
  puts("[L]ista varukatalog");
  puts("[A]vbryt\n");
  
  char *c = ask_question_string("");
  c[0] = toupper(c[0]);

  if(!strcmp(c,"N"))
    {
      char *new_name = ask_question_string("Välj nytt namn för varan\n");
      *(get_key_root(tree, item->name)) = new_name;
      item->name = new_name;
      print_goods(item);
      goto top;
    }
  
  if(!strcmp(c,"B"))
    {
      char *new_desc = ask_question_string("Välj ny beskrivning för varan\n");
      item->desc = new_desc;
      print_goods(item);
      goto top;
    }
  
  if(!strcmp(c,"P"))
    {
      int new_price = ask_question_int("Välj nytt pris för varan\n");
      item->price = new_price;
      print_goods(item);
      goto top;
    }

  if(!strcmp(c, "M"))
    {
      int new_amount = ask_question_int("Välj ett nytt varuantal\n");      
      shelf_entry_t *shelf_entry = (shelf_entry_t *) get_element(get_first(item->list));
      shelf_entry->amount = new_amount;
      print_goods(item);
      goto top;
    }
            
  if (!strcmp(c, "L"))
    {
      list_goods(tree);
      goto top;
    }
  
  if (!strcmp(c,"A"))
    {
      return;
    }
  else
    {
      puts("Fel inmatning, testa igen");
      goto top;
    }
}

void edit_goods(tree_root_t *tree, action_t undo)
{
  char *item_key = ask_question_string("Vilken vara vill du ändra?\n");    
  goods_t *item = tree_get(tree, item_key);
  while(!item)
    {
      item_key = ask_question_string("Hittade ej varan, försök igen\n");
      item = tree_get(tree, item_key);
    }
  print_goods(item);

  undo.type=3;
  undo.merch = item; 
  undo.copy = copy_item(item);
  
  list_menu(item, tree, undo);
  return;
}


void menu_choice(tree_root_t *tree, action_t undo) 
{
  print_menu();
  char *c = ask_question_string("Vad vill du göra idag?\n");
  c[0] = toupper(c[0]);
  
  if (!strcmp(c,"L"))
    {
      add_goods(tree);
      return;
    }

  if (!strcmp(c,"T"))
    {
      remove_goods();
      return;
    }

  if (!strcmp(c,"R"))
    {
      edit_goods(tree, undo);
      return;
    }

  if (!strcmp(c,"G"))
    {
      if(undo.type==0)
        {
          puts("Det finns ingenting att ångra");
          return;
        }
      
      if(undo.type==3) // Free:a gamla itemet?
        {
          undo.merch = &undo.copy;
        }
    }

  if (!strcmp(c,"H"))
    {
      list_goods(tree);
      return;
    }

  if (!strcmp(c,"A"))
    {
      puts("Du har valt att stänga av programmet");
      exit(0);
    }
  
  return;
}


int main()
{
  action_t undo = {.type = 0, .merch = NULL};
  tree_root_t *tree = tree_new();
  puts("\nVälkommen till lagerhantering 1.0");
  puts("=================================");
  while(true)
    {
      menu_choice(tree, undo);
    }
  
  return 0;
}
