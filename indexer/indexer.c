#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../utils/hash.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"
int sum = 0;
int finId = 0;

typedef struct {  // structure to hold each word of a webpage and its frequency
  char* word;
  queue_t* freq;
  // queue_t documents;
  // TODO: change freq to a queue filled with struct: (documents and
  // frequencies)
} word_t;

typedef struct {
  int id;
  int freq;
} docs_t;

/* returns true if the given word has already been used */
bool wordUsed(void* word, const void* key) {
  return (!strcmp(((word_t*)word)->word, (const char*)key));
}

// Needs to be fixed since freq is now a queue
// void printWord(void* word) {
//  printf("%s - %d\n", ((word_t*)word)->word, ((word_t*)word)->freq);
//}

void closeWord(void* word) { free(((word_t*)word)->word); }

// returns true if two pages have the same id
bool pageCompare(void* doc, const void* id) {
  return ((docs_t*)doc)->id - *((int*)id) == 0;
}

// returns the number of times a word is used in the document with the given ID
void sumWords(void* word) {
  int id = 1;  // using document 1
  docs_t* doc;
  while (id <= finId) {
    if ((doc = (docs_t*)qsearch(((word_t*)word)->freq, pageCompare,
                                (void*)&id)) != NULL) {
      printf("Word %s has frequency %d\n", ((word_t*)word)->word, doc->freq);
      sum += doc->freq;
    }
    id++;
  }
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
      *word = '\0';
      break;
    }
  }
  if (count < 3) {
    *word = '\0';
    // printf("count was less than 3, word should be empty. Word = %s\n", word);
  }
}

// returns the number of times a target word is found in the html of a given
// page
int wordCount(webpage_t* page, char* target) {
  int pos = 0;
  char* word;
  int count = 0;
  while ((pos = webpage_getNextWord(page, pos, &word)) > 0) {
    // loop over each word in the page
    normalizeWord(word);  // convert to alphabetic and lowercase
    if (strlen(word) > 0) {
      if (strcmp(word, target) == 0) {  // if the current word matches the
                                        // target
        count++;
      }
    }
    free(word);
  }
  return count;
}

// creates a queue of type doc_t for a given word_t
void makeDocQueue(void* word) {
  word_t* w = (word_t*)word;
  webpage_t* page;
  int id = 1;
  char dirname[10];
  strcpy(dirname, "../pages");
  w->freq = qopen();
  while (id <= finId) {
    if ((page = pageload(id, dirname)) != NULL) {
      int freq = wordCount(page, w->word);
      docs_t* d = malloc(sizeof(docs_t*));
      d->id = id;
      d->freq = freq;
      qput(w->freq, d);
      id++;
      webpage_delete(page);
    }
  }
}

void deleteDocQueue(void* word) {
  word_t* w = (word_t*)word;
  qclose(w->freq);
}

int main(int argc, char* argv[]) {
  sscanf(argv[1], "%d", &finId);
  printf("%d\n", finId);
  hashtable_t* index = hopen(20);
  int id = 1;
  while (id <= finId) {
    webpage_t* top = pageload(id, "../pages");
    // printf("%s\n", webpage_getURL(top));
    char* word;
    int pos = 0;
    //  word_t* w;
    while ((pos = webpage_getNextWord(top, pos, &word)) > 0) {
      // loop over each word in the page
      normalizeWord(word);  // convert to alphabetic and lowercase
      if (strlen(word) > 0) {
        // printf("%s\n", word);
        word_t* w;
        if ((w = (word_t*)hsearch(index, wordUsed, word, strlen(word))) !=
            NULL) {
          // if word has already been used and placed in the index
          //      (w->freq)++;
          // printf("Word %s now has frequency %d\n", w->word, w->freq);
        } else {  // if the word hasn't been used, add it with frequency of 1
          printf("%s\n", word);
          w = (word_t*)malloc(sizeof(word_t));
          w->word = (char*)malloc(strlen(word) * sizeof(char) + 1);
          strcpy(w->word, word);
          // w->freq = 1;
          // printf("%s - %d\n", w->word, w->freq);
          hput(index, w, w->word, strlen(w->word));
          // happly(index, printWord);
        }
      }
      free(word);
    }
    webpage_delete(top);
    id++;
  }
  printf("Making doc queue...\n");
  happly(index, makeDocQueue);
  happly(index, sumWords);
  printf("Sum = %d\n", sum);
  happly(index, deleteDocQueue);
  happly(index, closeWord);
  hclose(index);
  return 0;
}
