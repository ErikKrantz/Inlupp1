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

void print_menu()
{
  char *menu_choices = "[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukatalogen\n[A]vsluta\n";
  printf("%s", menu_choices);
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

bool shelf_occupied_aux(char *shelf, tree_node_t *tree)
{
  if (tree==NULL)
    {
      return false;
    }

  char *value = (char *) get_element((link_t *)get_value_node(tree));
  if (strcmp(shelf, value))
    {
      return true;
    }
  return shelf_occupied_aux(shelf, get_right(tree)) && shelf_occupied_aux(shelf, get_left(tree));  
}

bool shelf_occupied(char *shelf, tree_root_t *tree)
{
  if (tree==NULL)
    {
      return false;
    }

  char *value = (char *) get_element((link_t *)get_value_root(tree));
  if(strcmp(shelf, value))
    {
      return true;
    }
  else
    {
      shelf_occupied_aux(shelf, get_root(tree)); 
      
    }
  return false;
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
  
  //bool occupied = true;
  //shelf_entry_t *elem = (shelf_entry_t *) get_element(get_first(item->list));
  shelf_entry_t *shelf_elem = calloc(1, sizeof(shelf_entry_t));
  //char *shelf = "";

  list_prepend(list, shelf_elem);

  /*
  do
    {
      shelf = ask_question_string("Vilken hyllplats? T.ex. A25\n");     // Ger seg fault atm, måste göra koll för shelf på något sätt
      occupied = shelf_occupied(shelf, tree);
    }while(occupied);
  shelf_elem->shelf = shelf;
  */

  char *shelf = ask_question_string("Vilken hyllplats? T.ex. A25\n");
  shelf_elem->shelf = shelf;
  
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

void undo_action()
{
}


void list_menu(goods_t *item, tree_root_t *tree) 
{
  puts("Ändra [N]amn");
  puts("Ändra [B]eskrivning");
  puts("Ändra [P]ris");
  puts("[L]ista varukatalog");
  puts("[A]vbryt");
  puts("");
  
  char *c = ask_question_string("");
  char temp = c[0];
  c[0] = toupper(temp);
  
  if (!strcmp(c, "L"))
    {
      list_goods(tree);
      list_menu(item, tree);
    }
  
  if (!strcmp(c,"A"))
    {
      return;
    }
  
  if(!strcmp(c,"N"))
    {
      char *new_name = ask_question_string("Välj nytt namn för varan\n");
      *(get_key_root(tree, item->name)) = new_name;
      item->name = new_name;
      print_goods(item);
      list_menu(item, tree);
    }
  
  if(strcmp(c,"B")==0)
    {
      char *new_desc = ask_question_string("Välj ny beskrivning för varan\n");
      item->desc = new_desc;
      print_goods(item);
      list_menu(item, tree);
    }
  
  if(strcmp(c,"P")==0)
    {
      int new_price = ask_question_int("Välj nytt pris för varan\n");
      item->price = new_price;
      print_goods(item);
      list_menu(item, tree);
    }
    
  puts("Fel inmatning, testa igen");
  list_menu(item, tree);
}

void edit_goods(tree_root_t *tree)
{
  char *item_key = ask_question_string("Vilken vara vill du ändra?\n");    
  goods_t *item = tree_get(tree, item_key);
  while(!item)
    {
      item_key = ask_question_string("Hittade ej varan, försök igen\n");
      item = tree_get(tree, item_key);
    }
  print_goods(item);

  list_menu(item, tree);
  return;
}


void menu_choice(tree_root_t *tree) 
{
  print_menu();
  char *c = ask_question_string("Vad vill du göra idag?\n");
  
  if (strcmp(c,"L")==0 || strcmp(c,"l")==0)
    {
      add_goods(tree);
      return;
    }

  if (strcmp(c,"T")==0 || strcmp(c,"t")==0)
    {
      remove_goods();
      return;
    }

  if (strcmp(c,"R")==0 || strcmp(c,"r")==0)
    {
      edit_goods(tree);
      return;
    }

  if (strcmp(c,"G")==0 || strcmp(c,"g")==0)
    {
      undo_action();
      return;
    }

  if (strcmp(c,"H")==0 || strcmp(c,"h")==0)
    {
      list_goods(tree);
      return;
    }

  if (strcmp(c,"A")==0 || strcmp(c,"a")==0)
    {
      puts("Du har valt att stänga av programmet");
      exit(0);
    }
  
  return;
}


int main()
{
  tree_root_t *tree = tree_new();
  menu_choice(tree);

  while(true)
    {
      menu_choice(tree);
    }
  
  return 0;
}
