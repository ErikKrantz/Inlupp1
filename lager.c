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

/*
test för list_new/list_append som ej är klart.
item_instances_t item = {};
print_goods(list_append(list_new(), item)); 
 */

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
  
  if(tree_insert(tree, name, item))       // Ger seg fault
    {
    printf("Det lyckades!\n");
    }
  else
    {
      printf("Något gick fel, lägg till din vara igen.\n");
    }
}


void remove_goods()
{
}

void list_menu(char *c, goods_t *item)
{
  if (strcmp(c,"N")==0 || strcmp(c,"n")==0)
    {
      char *new_name = ask_question_string("Välj nytt namn för vara\n");
      item->name = new_name;
    }
  if(strcmp(c,"B")==0 || strcmp(c,"b")==0)
    {
      char *new_desc = ask_question_string("Välj ny beskrivning för vara\n");
      item->desc = new_desc;
    }
  if(strcmp(c,"P")==0 || strcmp(c,"p")==0)
    {
      int new_price = ask_question_int("Sätt nytt pris för vara\n");
      item->price = new_price;
    }
  else
    {
      c = ask_question_string("Fel inmatning, testa igen\n");
      list_menu(c, item);
    }
}

void edit_goods(tree_root_t *tree)
{
  char *item_key = ask_question_string("Vilken vara vill du ändra?\n");
  goods_t *item = tree_get(tree, item_key);
  print_goods(item);

  printf("Ändra [N]amn\n");
  printf("Ändra [B]eskrivning\n");
  printf("Ändra [P]ris\n");
  char *c = ask_question_string("");
  list_menu(c, item);
  char *a = ask_question_string("Skriv 'Y' om du vill ändra något mer, any key om inte.\n");
  if (strcmp(a,"Y")==0 || strcmp(a,"y")==0)
    {
      edit_goods(tree);
    }
}

void list_goods_aux(tree_root_t *tree, int index, int tree_siz)
{
  //int temp_i = index;
  //int upperlimit = tree_size(tree) - index;

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

void menu_choice(char *c, tree_root_t *tree) //menu??
{
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
      //edit_goods();
      //return;
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
  else
    {
      c = ask_question_string("Ditt val hittades inte i menyn, testa igen.\n");            // printar frågan en gång för mkt
      menu_choice(c, tree);
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
  

  tree_root_t *tree = tree_new();
  char *ch = ask_question_string("Vad vill du göra idag?\n");
  menu_choice(ch, tree);
  list_goods(tree);
  print_goods(tree_get(tree, "tvål"));  // verkar funka som den ska.
  display_goods(tree);
  edit_goods(tree);
  
  /*
  shelf_entry_t *shelf1;
  shelf1->shelf="A25";
  shelf1->amount=2;

  shelf_entry_t *shelf2;
  shelf2->shelf="B25";
  shelf2->amount=3;

  
  link_t *link2 = link_new(shelf2, NULL);
  link_t *link1 = link_new(shelf1, link2);

  list_t *list;
  get_first(list) = link1;
  //list->first = link1;

  get_last(list) = link2;
  //list->last = link2;
  
  goods_t *item;
  item->name="Tvål";
  item->desc="Rengör";
  item->price=2990;
  item->list=list;

  print_goods(item);
  */
  
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
