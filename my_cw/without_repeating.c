#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int without_repeating(char** text, int count) {
   int unique_count = 0;

   for(int i = 0; i < count; i++) {
      if(text[i] == NULL) continue;
      int is_duplicate = 0;

      for(int j = 0; j < unique_count; j++) {
         if(text[j] != NULL && strcasecmp(text[i], text[j]) == 0) {
            is_duplicate = 1;
            break;
         }
      }

      if(!is_duplicate) {
         if(i != unique_count) {
            text[unique_count] = text[i];
         }
         unique_count++;
      } else {
            free(text[i]);
            text[i] = NULL;
      }
   }

   return unique_count;
}