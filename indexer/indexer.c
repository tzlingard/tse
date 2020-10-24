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
  return (!strcmp((char*)word, (const char*)key));
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
    } else {  // delete non-alphabetic words
      *word = 0;
      break;
    }
  }
  if (count < 3) {
    *word = 0;
    // printf("count was less than 3, word should be empty. Word = %s\n", word);
  }
}

int main(void) {
	/*
  printf("non alphabetic words test:\n\n");

  char* alpha = (char*)malloc(10);
  strcpy(alpha, "AbCdEf");
  printf("Normalized %s = ", alpha);
  normalizeWord(alpha);
  printf("%s\n", alpha);
  free(alpha);

  char* nonalpha = (char*)malloc(10);
  strcpy(nonalpha, "aBc123");
  printf("Normalized %s = ", nonalpha);
  normalizeWord(nonalpha);
  printf("%s\n\n", nonalpha);
  free(nonalpha);
	*/
  webpage_t* top = pageload(1, "../pages");
	  hashtable_t* index = hopen(20);
		//printf("%s\n", webpage_getURL(top));
  char* word;
  int pos = 0;
	//  word_t* w;
  while ((pos = webpage_getNextWord(top, pos, &word)) > 0) {
    // loop over each word in the page
    normalizeWord(word);  // convert to alphabetic and lowercase
    if (strlen(word) > 0) {
      printf("%s\n", word);
			/*
			if ((w = (word_t*)hsearch(index, wordUsed, word, strlen(word))) != NULL) {
        // if word has already been used and placed in the index
        (w->freq)++;
        printf("%s - %d\n", w->word, w->freq);
      } else {  // if the word hasn't been used, add it with frequency of 1
        printf("First appearance of word %s\n", word);
        w = (word_t*)malloc(sizeof(word_t));
        w->word = word;
        w->freq = 1;
        // printf("%s - %d\n", w->word, w->freq);
        hput(index, (void*)w, w->word, strlen(w->word));
			*/      
		}
			free(word);
	}

	
  hclose(index);
  webpage_delete(top);
  return 0;
}
