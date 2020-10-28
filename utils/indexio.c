#include <stdio.h>
#include <unistd.h>

#include "hash.h"
#include "queue.h"
#include "webpage.h"

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

/* Save an index to a file*/
void indexsave(hashtable_t* htp, char* dirname, char* indexnm) {}

/* Load an index from a file*/
hashtable_t* indexload(char* dirname, char* indexnm) {
  char loc[80];
  sprintf(loc, "%s/%s", dirname, indexnm);
  if (access(loc, R_OK) == 0) {
    FILE* fp;
    fp = fopen(loc, 'r');
    char* line;
    hashtable_t* index;
    while (fscanf(fp, "%s", line) == 1) {
      char* word;
      int docId, count;
      sscanf(line, "%s", word);
      while (scanf(line, "%d %d", docId, count) == 1) {
        word_t* w = (word_t*)malloc(sizeof(word_t));
        w->word = (char*)malloc(strlen(word) * sizeof(char) + 1);
        strcpy(w->word, word);
        hput(index, w, w->word, strlen(w->word));
      }
      if (feof(fp)) break;
    }
    happly(index, makeDocQueue);
    return (index);
  } else
    return NULL;
}