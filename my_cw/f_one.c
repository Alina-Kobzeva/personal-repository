#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void convert_seconds_to_time(char** text, int sentence_count) {

   for(int i = 0; i < sentence_count; i++) {
      for(int j = 0; text[i][j] != '\0'; j++) {
         if(isdigit(text[i][j])) {
            int k = j;
            while(isdigit(text[i][k])) {
               k++;
            }

            if(strncmp(&text[i][k], "sec", 3) == 0) {
               char num_str[20] = {0};
               strncpy(num_str, &text[i][j], k-j);
               num_str[k-j] = '\0';
               int num = atoi(num_str);
               j = k + 2;
               printf("%02d:%02d:%02d\n", num / 3600, (num % 3600) / 60, num % 60);
            }
         }
      }
   }
}