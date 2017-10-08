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

typedef struct shelf_entry shelf_entry_t;

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
  int amount = tally_amount(goods->list);                 // Bör ändras, då amount alltid ska vara tilldelad - i.e vi ska inte behöva räkna ut vad det är
  /* Osäker på om detta stämmer
  shelf_entry_t *shelf_elem = list_first(item->list)
  int amount = shelf_elem->amount
  char *shelf = shelf_elem->shelf
  printf("Amount: %d \n", amount);
  printf("Shelf: %s \n", shelf);
  */
}

void print_menu()
{
  puts("[L]ägga till en vara");
  puts("[T]a bort en vara");
  puts("[R]edigera en vara");
  puts("Ån[g]ra senaste ändringen");
  puts("Visa [h]ela varukatalogen");
  puts("[A]vsluta");
}

void ask_mainmenu()                                      // Återanvänds ofta, så kan vara bekvämt att ha.
{
  print_menu();
  char c = ask_question_char("Vad vill du göra nu?\n");
  menu_choice(c,tree);
}

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
  link_t *next = NULL;                                  // I om att listan är tom så är next == NULL
  // link_t *new_link = calloc(1, sizeof(link_t));      // lager.c vet nog inte sizeof link_t             Alternativ 1 (troligtvis fel)
  link_t *new_link = link_new(shelf_elem, next);        // Här behöver lager.c inte veta sizeof link_t    Alternativ 2 (troligtvis rätt)
  list->new_link = new_link;                            // Osäker om jag tänker rätt här, men man bör ju connecta till listan på något sätt 
  new_link->shelf_elem = shelf_elem;
  new_link->next = next; 
  
  printf("Break 2\n");
  
  list_prepend(list, shelf_elem);                       // sätter in elementet i början av listan
  
  printf("Break 3\n");
    
  char *shelf = ask_question_string("Välj hyllplats\n");
  shelf_elem->shelf = shelf;
  
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
      add_goods(tree);
    }
  print_menu();
  char c = ask_question_char("Din vara är tillagd, vad vill du göra nu?\n");
  menu_choice(c,tree);
}

void remove_goods()
{
}

void edit_menu(char c, goods_t *item)
{
  if (toupper(c)=='N')
    {
      char *new_name = ask_question_string("Välj nytt namn för vara\n");
      item->name = new_name;
    }
  if(toupper(c)=='B')
    {
      char *new_desc = ask_question_string("Välj ny beskrivning för vara\n");
      item->desc = new_desc;
    }
  if(toupper(c)=='P')
    {
      int new_price = ask_question_int("Sätt nytt pris för vara\n");
      item->price = new_price;
    }
  if(toupper(c)=='H')
  {
    /* TO DO - edit: otestad
    char *new_shelf = ask_question_string("Välj ny hyllplats\n");
    current_list = item->list;
    first_elem = list_first(current_list);
    first_elem->shelf = new_shelf
    */
  }
  if(toupper(c)=='A')
  {
    /* TO DO - edit: otestad
    int new_amount = ask_question_int("Välj nytt antal\n");
    current_list = item->list;
    first_elem = list_first(current_list);
    first_elem->amount = new_amount;
    */
  }
  if(toupper(c)=='T')
  {
    print_menu();
    char c = ask_question_char("Vad vill du göra nu?\n");
    menu_choice(c,tree);
  }
  else
    {
      c = ask_question_char("Fel inmatning, testa igen\n");
      edit_menu(c, item);
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
  printf("Ändra [H]yllplats\n");
  printf("Ändra [A]ntal\n");
  printf("Hoppa [T]ill Huvudmenyn\n");
  char c = ask_question_char("");
  edit_menu(c, item);
  char a = ask_question_char("Skriv 'Y' om du vill ändra något mer, any key om inte.\n");
  if (toupper(a)=='Y')
    {
      edit_goods(tree);
    }
  print_menu();
  char c = ask_question_char("Vad vill du göra nu?\n");
  menu_choice(c,tree);
}

void display_goods_aux(tree_root_t *tree)
{
  char *item_name = ask_question_string("Din vara hittades inte i trädet, var vänlig testa igen\n");
  if (tree_has_key(tree, item_name) == true)
  {
    print_goods(item_name);
    print_menu();
    char c = ask_question_char("Vad vill du göra nu?\n");
    menu_choice(c,tree);
  }
  display_goods_aux(tree);
}
    

void display_goods(tree_root_t *tree)
{
  if (tree == NULL)
  {
    printf("Det finns inga varor att hämta\n");
    print_menu();
    c = ask_question_char("Var vänlig välj ett av ovanstående alternativ\n");                   
    menu_choice(c, tree);
  }
  char *item_name = ask_question_string("Skriv namnet på varan du vill se\n");
  if (tree_has_key(tree,item_name))
  {
    print_goods(item_name);
    print_menu();
    char c = ask_question_char("Vad vill du göra nu?\n");
    menu_choice(c,tree);
  }
  display_goods_aux(tree);
}

void list_goods(tree_root_t *tree)
{
  if (tree==NULL)
  {
    printf("Det finns inga varor i trädet\n");
    ask_mainmenu();
  }
  display_goods(tree) // Test för att se om vi kan hitta en vara
  
  /*
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

  char c = ask_question_char("Vill du lista fler varor? [Y]?\n");
  if (toupper(c) == 'Y')
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
    }
  char c = ask_question_char("Vill du lista fler varor? [Y]?\n");
  if (toupper(c) == 'Y')
    {
      list_goods_aux(tree, i, tree_siz);
    }
}
*/
  
}



void undo_action()
{
}

void exit_program()
{
  printf("Du har kallat på exit_program\n");
  // return;
}

void menu_choice(char c, tree_root_t *tree)
{ 
  if (toupper(c)=='L')
    {
      add_goods(tree);
    }

  if (toupper(c)=='T')
    {
      remove_goods();
    }

  if (toupper(c)=='R')
    {
      edit_goods(tree);
    }

  if (toupper(c)=='G')
    {
      undo_action();
    }

  if (toupper(c)=='H')
    {
      list_goods(tree);
    }

  if (toupper(c)=='A')
    {
      exit_program();
    }
  else
    {
      print_menu();
      c = ask_question_char("Var vänlig välj ett av ovanstående alternativ\n");                   
      menu_choice(c, tree);
    }
}


int main()
{
  print_menu();
  tree_root_t *new_tree = NULL;
  char c = ask_question_char("Vad vill du göra idag?\n");
  menu_choice(c, new_tree);
    
  return 0;
}
