#include <ctype.h>
#include <stdio.h>

#include "../utils/hash.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"

/*  Takes in a word and normalizes it by making all letters lowercase and
 * getting rid of non-alphabetic characters */
void normalizeWord(char* word) {
  int count = 0;
  while (word[count] != 0) {
    if (isalpha(word[count])) {       // if character is alphabetic
      char c = tolower(word[count]);  // make the character lowercase
      word[count] = c;
      count++;
    } else {  // delete non-alphabetic characters
      word[count] = 0;
    }
  }
  if (count < 3) {
    word = "";
    // printf("word should be null. word is %s\n", word);
  }
  // printf("count = %d\n",count);
}

int main(void) {
  webpage_t* top = pageload(1, "../pages");
  hash_t* index = hopen(20);
  printf("%s\n", webpage_getURL(top));
  char* word;
  int pos = 0;
  while ((pos = webpage_getNextWord(top, pos, &word)) > 0) {
    normalizeWord(word);
    if (word != NULL) {
      printf("%s\n", word);
    }
    free(word);
  }

  webpage_delete(top);

  return 0;
}
