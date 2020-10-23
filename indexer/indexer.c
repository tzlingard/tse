#include <stdio.h>
#include <string.h>

#include "../utils/pageio.h"
#include "../utils/webpage.h"

void NormalizeWord(char* word) {
  int len;
  int i;
  if ((len = strlen(word)) < 3) {
    for (i = 0; i < len; i++) {
      if (word[i] < 91 && word[i] > 64) {  // if the letter is uppercase
        word[i] += 32;                     // convert to lowercase
      } else if (word[i] > 122 || word[i] < 65) {
        // if the letter is non-alphabetic
        word[i] = 0;
      }
    }
  } else {
    word = "";  // if the word isn't at least 3 letters, get rid of it
  }
}

int main(void) {
  webpage_t* top = pageload(1, "../pages");
  printf("%s\n", webpage_getURL(top));
  FILE* fp;
  fp = fopen("1out.txt", "w");
  char* word;
  int pos = 0;
  while ((pos = webpage_getNextWord(top, pos, &word)) > 0) {
    NormalizeWord(word);
    printf("%s ", word);
    fprintf(fp, "%s ", word);
    //		printf("pos = %d\n",pos);
    free(word);
  }
  fclose(fp);
  webpage_delete(top);
  return 0;
}
