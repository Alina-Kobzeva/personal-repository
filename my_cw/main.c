#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "read_text.h"
#include "without_repeating.h"
#include "f_one.h"
#include "f_two.h"
#include "f_three.h"
#include "f_four.h"

void info() {

   printf("0 - The function removes duplicate and displays the text.\n");
   printf("1 - For sentences containing '<num>sec' patterns, the function converts the extracted seconds into 'hour:min:sec' format and outputs the result.\n");
   printf("2 - The function deletes the first word in each sentence.\n");
   printf("3 - If all the words in a sentence are equal, the function deletes that sentence.\n");
   printf("4 - The function sorts sentences in descending order based on the number of lowercase letters in each sentence.");
   printf("5 - The function displays information about the functions implemented by the program.\n");
} 


int main() {

   printf("Course work for option 4.11, created by Alina Kobzeva.\n");

   int num_of_func;
   if (scanf("%d", &num_of_func) != 1) {
      printf("Error: command input error.\n");
      return 0;
   }

   if(num_of_func == 5){
      info();
      return 0;
   }

   if (num_of_func < 0 || num_of_func > 4) {
      printf("Error: there is no such function\n");
      return 0;
   }

   int sentence_count = 0;

   char** all_text = read_text(&sentence_count);

   if(all_text == NULL) {
      printf("Error: Не удалось прочитать текст\n");
      return 0;
   }

   int new_count = without_repeating(all_text, sentence_count);
   sentence_count = new_count;

   switch(num_of_func) {

      case '0':
         for(int i = 0; i < sentence_count; i++) {
            if(all_text[i] != NULL) {
               printf("%s\n", all_text[i]);
            }
         }
         break;
         

      case 1:
         convert_seconds_to_time(all_text, sentence_count);
         break;


      case 2:
         remove_first_word(all_text, sentence_count);
         break;

      case 3:
         remove_same_length(all_text, sentence_count);
         break;


      case 4:
         bubble_sort_lowercase(all_text, sentence_count);
         break;
   }              

   for (int i = 0; i < sentence_count; i++) {
      if(all_text[i] != NULL) {
         free(all_text[i]);
      }
   }
   
   free(all_text);
   return 0;
}
