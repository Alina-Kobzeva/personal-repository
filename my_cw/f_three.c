#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void remove_same_length(char** text, int sentence_count) {
   for(int i = 0; i < sentence_count; i++) {
      if(text[i] == NULL)
         continue;

      char* str = strdup(text[i]);
      if(!str)
         continue;

      size_t len = strlen(str);
      if(len > 0 && str[len-1] == '.')
         str[len-1] = '\0';

      int result = 1;
      int first_len = -1;
      int word_count = 0;
      
      char *word = strtok(str, " ");

      while(word != NULL) {
         int word_len = strlen(word);
         if(word_len > 0) {
            word_count++;
         }

         if(first_len == -1) {
            first_len = word_len;
         }

         else if(first_len != word_len) {
            result = 0;
            break;
         }

         word = strtok(NULL, " ");
      }

      free(str);

      if(result && word_count > 1) {
         free(text[i]);
         text[i] = NULL;
      }  
   }
   for(int i = 0; i < sentence_count; i++) {
      if(text[i] != NULL) {
         printf("%s\n", text[i]);
      }
   }
}
