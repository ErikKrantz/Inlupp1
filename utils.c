#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"


bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t make_float(char *str)
{
  return (answer_t) { .f = atof(str) };
}


bool check(char *str)
{
  return (strlen(str) > 0) && is_number(str); 
}


answer_t convert_to_int (char *str)
{
  return (answer_t) atoi(str);
}


answer_t ask_question(char *question, check_func check, convert_func convert)
{
  int buf_siz = 999;
  char buf[buf_siz];
    do
      {
	printf("%s",question);
	read_string(buf, buf_siz);
      }while (!check(buf));
    return convert(buf);
}


bool is_number(char *str)
{
  int i = 0;

  if (str[0]=='-')
    {
      i = 1;                         // om första char i str är - skippar vi kollen av det elementet i vår for-loop
      if (strlen(str)==1)            // om str bara innehåller - kommer i sättas till 1 och vi kommer inte ens gå in i for-loopen. ergo behövs edge case.
	{
	  return false;
	}
    }
  for (; i<(strlen(str));i++)         // for-loop där i stegrar igenom chars i str
    {
      if (!isdigit(str[i]))           // om i:e elem i str inte är en siffra ->
	{
	  return false;               // om i:e char ej är en siffra -> returnera false.
	}
    }
  return true;
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; // svaret som ett heltal
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).s;
}



char ask_question_char(char *question)
{
  printf("%s", question);
  return toupper(getchar());
}



/* Labb 3 uppvärmningsuppg 
void print(char *string)
{
  int i=0;
  while(string[i] != '\0')
    {
      putchar(string[i]);
      i++;
    }
}
*/

//^^ Omskriven med pekararitmetik (labb 4): (ej testad)
void print(char *string)
{
  while(*string != '\0' || *string != EOF)
    {
      putchar(*string);
      string++;
    }
}


void println(char *string)
{
  print(string);
  printf("\n");   
}
/* Labb 3 uppvärmingsuppg */

// används inte atm, MÅSTE CLEARA BUFFERN T
/*
void clear_input_buffer()
{
  int c;
  do
    {
      c = getchar();
    }
  while (c != '\n' || c != EOF);	
}
*/
/*
int string_length(char *str)
{
  char *end = str;
  while (*end != '\0') ++end;
  return end - str;
}
*/


int read_string(char *buf, int buf_siz)
{
  char c;
  int i = 0;
  for (; i<buf_siz; ++i)
    {
      c = getchar();
      // clear_input_buffer();
      if (c != '\n' && c != EOF)
	{
	  buf[i] = c;
	}
      else
	{
	  break;
	}
    }
  buf[i] = '\0';
  return i;
}


//clearar buffer nu? ^^^
/*
char *ask_question_string(char *question, char *buf, int buf_siz)
{
  int empty;
  do
    {
      printf("%s", question);
      empty = read_string(buf, buf_siz);
    } while (empty == 0);
  return buf;
}
*/

/*
int ask_question_int(char *question)
{

  int result = 0;
  int conversions = 0;
    do
      {
	printf("%s\n", question);
	conversions = scanf("%d", &result);
	int c;
	      do
		{	
	  c = getchar();
		}
	      while (c != '\n' && c != EOF);
	      putchar('\n');
      }
    while (conversions < 1);
    return result;
}
*/
