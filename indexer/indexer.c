#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../utils/hash.h"
#include "../utils/indexio.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"
int sum = 0;
int finId = 0;
int iteration = 0;
char dirname[30];

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

void closeWord(void* word) {free(((word_t*)word)->word); }

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
  iteration++;
  printf("%d\n", iteration);
  word_t* w = (word_t*)word;
  webpage_t* page;
  int id = 1;
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
    } else {
      id = finId + 1;  // break loop
    }
  }
}

void deleteDocQueue(void* word) {
  word_t* w = (word_t*)word;
  qclose(w->freq);
}

int main(int argc, char* argv[]) {
  char indexnm[30];
  int words = 0;
  sscanf(argv[1], "%s", dirname);
  printf("dirname = %s\n", dirname);
  if (access(dirname, R_OK) != 0) {  // if directory cannot be read
    exit(EXIT_FAILURE);
  }
  sscanf(argv[2], "%s", indexnm);
  // printf("%d\n", finId);
  hashtable_t* index = hopen(20);
  int id = 1;
  char loc[35];
  sprintf(loc, "%s/%d", dirname, id);
  while (access(loc, R_OK) == 0) {  // while the id exists in the directory
    webpage_t* top = pageload(id, dirname);
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
        if ((w = (word_t*)hsearch(index, wordUsed, word, strlen(word))) == NULL) {
                                // if word has already been used and placed in the index
                                //      (w->freq)++;
                                // printf("Word %s now has frequency %d\n", w->word, w->freq);
                                // if the word hasn't been used, add it with frequency of 1
          printf("%s\n", word);
				  w = (word_t*)malloc(sizeof(word_t));
          w->word = (char*)malloc(strlen(word) * sizeof(char) + 1);
          strcpy(w->word, word);
                                // w->freq = 1;
                                // printf("%s - %d\n", w->word, w->freq);
          hput(index, w, w->word, strlen(w->word));
<<<<<<< HEAD
                                // happly(index, printWord);
        }else{
					 printf("%removed extra: s\n", word);
					hremove(index, w, w->word, strlen(w->word));   
				}
				
=======
          words++;
          // happly(index, printWord);
        }
>>>>>>> ea2d7661f1362f1c79777f14b7766c8eaf509be7
      }
      free(word);
    }
    webpage_delete(top);
    id++;
    memset(loc, 0, 35);                  // clear the string
    sprintf(loc, "%s/%d", dirname, id);  // set the new id location
  }
  finId = id - 1;
  printf("Num words = %d\n", words);
  printf("finId = %d\n", finId);
  printf("Making doc queue...\n");
  happly(index, makeDocQueue);
  happly(index, sumWords);
  printf("Sum = %d\n", sum);
  indexsave(index, ".", indexnm);
  happly(index, deleteDocQueue);
  happly(index, closeWord);
  hclose(index);
  return 0;
}
