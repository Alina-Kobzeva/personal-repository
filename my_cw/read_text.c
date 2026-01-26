#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** read_text(int *sentence_count){
   int text_capacity = 10, sentence_capacity = 100, ch_count = 0;
   *sentence_count = 0;

   char **text = malloc(text_capacity * sizeof(char*));
   if(text == NULL) {
      printf("Error: Ошибка выделения памяти\n");
      return NULL;
   }

   char *new_sentence = malloc(sentence_capacity * sizeof(char));
   if(new_sentence == NULL) {
      free(text);
      printf("Error: Ошибка выделения памяти\n");
      return NULL;
   }

   int ch;
   int newlines = 0;

   while((ch = getchar()) != EOF) {
      if(ch == '\n') {
         newlines++;
         if(newlines == 2) {
            break;
         }
         continue;
      } else {
         newlines = 0;
      }

      if(ch_count >= sentence_capacity - 1) {
         sentence_capacity *= 2;
         char* temp_sentence = realloc(new_sentence, sentence_capacity * sizeof(char));
         if(temp_sentence == NULL) {
            free(new_sentence);
            free(text);
            printf("Error: Ошибка выделения памяти\n");
            return NULL;
         }
         new_sentence = temp_sentence;
      }

      if (ch == '.') {
         if (ch_count > 0) {
            new_sentence[ch_count++] = '.';
            new_sentence[ch_count] = '\0';

            if(*sentence_count >= text_capacity) {
               text_capacity *= 2;
               char** temp_text = realloc(text, text_capacity * sizeof(char*));
               if(temp_text == NULL) {
                  free(new_sentence);
                  free(text);
                  printf("Error: Ошибка выделения памяти\n");
                  return NULL;
               }
               text = temp_text;
            }

            text[*sentence_count] = malloc((ch_count + 1) * sizeof(char));
            if(text[*sentence_count] == NULL) {
               free(new_sentence);
               free(text);
               printf("Error: Ошибка выделения памяти\n");
               return NULL;
            }
            strcpy(text[*sentence_count], new_sentence);
            (*sentence_count)++;
            ch_count = 0;
         }
      } else {
         if((char)ch == ' ' && ch_count == 0) {
            continue;
         }
         new_sentence[ch_count++] = (char)ch;
      }
   }

   free(new_sentence);
   return text;
}