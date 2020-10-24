#include <ctype.h>
#include <stdio.h>

#include "../utils/hash.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"
#include "string.h"

typedef struct {  // structure to hold each word of a webpage and its frequency
  char* word;
  int freq;
} word_t;

/* returns true if the given word has already been used */
bool wordUsed(void* word, const void* key) {
  return (!strcmp((char*)word, (char*)key));
}

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
      word = "";
			break;
    }
  }
  if (count < 3) {
    word = "";
  }

}

int main(void) {
  webpage_t* top = pageload(1, "../pages");
  hashtable_t* index = hopen(20);
  printf("%s\n", webpage_getURL(top));
  char* word;
  int pos = 0;
  while ((pos = webpage_getNextWord(top, pos, &word)) >
         0) {             // loop over each word in the page
    normalizeWord(word);  // convert to alphabetic and lowercase
    if (word != NULL) {
      printf("%s\n", word);
      word_t* w;
      if ((w = (word_t*)hsearch(index, wordUsed, word, strlen(word))) != NULL) {
        // if word has already been used and placed in the index
        (w->freq)++;
      } else {  // if the word hasn't been used, add it with frequency of 1
        w->freq = 1;
        hput(index, (void*)w, w->word, strlen(w->word));
      }
    }
    free(word);
  }
  hclose(index);
  webpage_delete(top);
  return 0;
}
