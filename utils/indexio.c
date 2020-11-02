#include "indexio.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hash.h"
#include "queue.h"
#include "webpage.h"

FILE* fp_global;

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

// print to fp the current doc id and the frequency
void printDocToFile(void* d) {
  docs_t* doc = (docs_t*)d;
  fprintf(fp_global, "%d %d ", doc->id, doc->freq);
}

// print to fp the word, docs and freqs,
void printWordToFile(void* w) {
  word_t* word = (word_t*)w;
  fprintf(fp_global, "%s ", word->word);
  qapply(word->freq, printDocToFile);
  fprintf(fp_global, "\n");
}

/* Save an index to a file*/
void indexsave(hashtable_t* htp, char* dirname, char* indexnm) {
  char dest[80];
  sprintf(dest, "%s/%s", dirname, indexnm);
  fp_global = fopen(dest, "w");
  happly(htp, printWordToFile);
  fclose(fp_global);
}
/*
hashtable_t* indexload(char* dirname, char* indexnm) {
        return NULL;
}
*/

// Load an index from a file
hashtable_t* indexload(char* dirname, char* indexnm) {
  printf("starting\n");
  char loc[80];
  sprintf(loc, "%s/%s", dirname, indexnm);
  if (access(loc, R_OK) == 0) {
    FILE* fp;
    fp = fopen(loc, "r");

    char nextWord[80];
    hashtable_t* index = hopen(100);

    while (fscanf(fp, "%s", nextWord) == 1) {
      word_t* w = (word_t*)malloc(sizeof(word_t*));
      w->word = (char*)malloc(strlen(nextWord) * sizeof(char) + 1);
      strcpy(w->word, nextWord);
      w->freq = qopen();
      int docId, count;

      while (fscanf(fp, "%d %d", &docId, &count) == 2) {
        docs_t* d = (docs_t*)malloc(sizeof(docs_t*));
        d->id = docId;
        d->freq = count;
        qput(w->freq, d);
      }

      hput(index, w, w->word, strlen(w->word));
      if (feof(fp)) break;
    }
    fclose(fp);
    return (index);
  } else
    return NULL;
}
