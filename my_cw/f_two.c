#include <stdio.h>
#include <string.h>

void remove_first_word(char** text, int sentence_count) {

   for(int i = 0; i < sentence_count; i++) {
      char *str = text[i];
      char *space = strchr(str, ' ');

      if(space != NULL) {
         memmove(str, space + 1, strlen(space + 1) + 1);
      } else {
         str[0] = '\0';
      }

      if(strlen(str) != 0)
         printf("%s\n", str);
   }
}