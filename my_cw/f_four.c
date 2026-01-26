#include <stdio.h>
#include <ctype.h>

int low_in_sent(char* sent) {
   int lowercase = 0;
   for(int j = 0; sent[j] != '\0'; j++) {
         if(islower(sent[j]) != 0)
            lowercase++;
   }
   return lowercase;
}


void bubble_sort_lowercase(char** text, int sentence_count) {

   for(int i = 0; i < sentence_count - 1; i++) {
      for(int k = 0; k < sentence_count - i - 1; k++) {
         if (text[k] == NULL || text[k+1] == NULL) {
            continue;
         }

         int lowercase1 = low_in_sent(text[k]);
         int lowercase2 = low_in_sent(text[k+1]);

         if(lowercase1 < lowercase2) {
            char *temp = text[k];
            text[k] = text[k+1];
            text[k+1] = temp;
         }
      }
   }
   for(int i = 0; i < sentence_count; i++) {
      if(text[i] != NULL) {
         printf("%s\n", text[i]);
      }
   }
}