#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utils/hash.h"
#include "../utils/indexio.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"

hashtable_t* index;
char pagedir[100];
FILE* out;

typedef struct {  // structure to hold data for every page in index
  int rank;
  int id;
  int occurences[34];
} query_docs_t;

typedef struct {
  char* word;
  queue_t* freq;
} word_t;

typedef struct {
  int id;
  int freq;
} docs_t;

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
}

bool isValid(char* curr, char* prev, int wordcount) {
  // special cases:
  // OR/AND is the first word: invalid query --  dealt with here
  // OR/AND is the last word: invalid query
  // non-alphabetic characters: invalid query-- dealt with here
  // OR & AND follow each other: invalid query-- dealt with here
  // two consecutive ORs or ANDs: invalid query-- dealt with here
  char currWord[30];
  strcpy(currWord, curr);
  char* c = curr;
  while ((int)(*c) != '\0') {  // check all letters are alphabetic
    if (!isalpha(*c)) {
      return false;
    }
    c += 1;
  }
  if (strcmp(currWord, "and") == 0 || (strcmp(currWord, "or") == 0)) {
    if (strcmp(prev, "and") == 0 || strcmp(prev, "or") == 0 || wordcount == 0) {
      // returns false if two reserved words in a row or starting with a
      // reserved word
      return false;
    }
  }
  return true;
}

// true if the given docs_t has given id
bool findID(void* doc, const void* id) {
  return ((docs_t*)doc)->id == *((int*)id);
}

// true if the given docs_t has given id
bool findQueryID(void* doc, const void* id) {
  return ((query_docs_t*)doc)->id == *((int*)id);
}

// true if the given query_docs_t has given rank
bool findRank(void* doc, const void* rank) {
  return ((query_docs_t*)doc)->rank == *((int*)rank);
}

bool findWord(void* indexWord, const void* word) {
  return strcmp(((word_t*)indexWord)->word, (char*)word) == 0;
}

bool stringCompare(void* word1, const void* word2) {
  return strcmp((char*)word1, (char*)word2) == 0;
}

void printWord(void* word) { printf("%s ", ((char*)word)); }

void printRank(void* doc) {
  query_docs_t* d = (query_docs_t*)doc;
  webpage_t* p = pageload(d->id, pagedir);
  printf("rank: %d doc: %d URL: %s\n", d->rank, d->id, webpage_getURL(p));
  webpage_delete(p);
}

void printRankToFile(void* doc) {
  query_docs_t* d = (query_docs_t*)doc;
  webpage_t* p = pageload(d->id, pagedir);
  fprintf(out, "rank: %d doc: %d URL: %s\n", d->rank, d->id, webpage_getURL(p));
  webpage_delete(p);
}

void closeWordT(void* word) {
  free(((word_t*)word)->word);
  qclose(((word_t*)word)->freq);
}

// deallocate all memory for an index
void closeIndex(hashtable_t* htp) {
  happly(htp, closeWordT);
  hclose(htp);
}

// given a queue of words, returns a queue of type query_doc_t for the docs
// containing all of those words
queue_t* getDocs(queue_t* words) {
  bool wordNotFound = false;  // true when a word in query is not in index at
  // all
  queue_t* docs = qopen();

  // add documents for first word (if word exists) to docs queue

  char* currWord;
  if ((currWord = (char*)(qget(words))) != NULL) {
    word_t* indexWord = hsearch(index, findWord, currWord, sizeof(currWord));
    if (indexWord != NULL) {
      queue_t* temp = qopen();
      docs_t* doc;
      while ((doc = (docs_t*)qget(indexWord->freq)) != NULL) {
        if (doc->freq > 0) {
          query_docs_t* d = malloc(sizeof(query_docs_t));
          d->rank = doc->freq;
          d->id = doc->id;
          qput(docs, d);
        }
        qput(temp, doc);
      }

      qclose(indexWord->freq);
      indexWord->freq = temp;
    } else if (strcmp(currWord, "or") != 0) {
      wordNotFound = true;
    }
    free(currWord);
  }

  // scan through rest of words
  // remove from queue of "surviving" docs and only re-add if document also has
  // current word

  while ((currWord = (char*)(qget(words))) != NULL && wordNotFound == false) {
    word_t* indexWord = hsearch(index, findWord, currWord, sizeof(currWord));
    if (indexWord != NULL) {
      query_docs_t* d;
      queue_t* temp_docs = qopen();
      while ((d = (query_docs_t*)(qget(docs))) != NULL) {
        docs_t* popDoc;
        int freq = 0;
        if ((popDoc = (((docs_t*)qsearch(indexWord->freq, findID,
                                         &(d->id))))) != NULL) {
          freq = popDoc->freq;
        }
        if (freq > 0) {
          if (freq < d->rank) {
            d->rank = freq;
          }
          qput(temp_docs, d);
        } else {
          free(d);
        }
      }
      qclose(docs);
      docs = temp_docs;
    } else {
      wordNotFound = true;
    }
    free(currWord);
  }
  if (wordNotFound) free(currWord);

  if (wordNotFound == true) {
    qclose(docs);
    docs = qopen();
  }
  return docs;
}

void docsCombine(queue_t* finaldocs, queue_t* tempdocs) {
  query_docs_t* dtemp;
  // continuously remove from temp docs
  while ((dtemp = (query_docs_t*)qget(tempdocs)) != NULL) {
    // if the current doc is not in the final docs, just add it
    if (qsearch(finaldocs, findQueryID, &(dtemp->id)) == NULL) {
      qput(finaldocs, dtemp);
    }
    // if the current doc is in the final docs, add the rankings
    else {
      ((query_docs_t*)(qsearch(finaldocs, findQueryID, &(dtemp->id))))->rank +=
          dtemp->rank;
      free(dtemp);
    }
  }
}

// sort the given queue of type query_docs_t by decreasing rank
void sortDocs(queue_t* docs) {
  int i = 1;
  queue_t* temp = qopen();
  query_docs_t* d;
  void* emptyTester;
  while ((emptyTester = qget(docs)) != NULL) {
    qput(docs, emptyTester);
    while ((d = (query_docs_t*)qremove(docs, findRank, &i)) != NULL) {
      qput(temp, d);
    }
    i++;
  }
  while ((emptyTester = qget(temp)) != NULL) {
    qput(temp, emptyTester);
    while ((d = (query_docs_t*)qremove(temp, findRank, &i)) != NULL) {
      qput(docs, d);
    }
    i--;
  }
  qclose(temp);
}

void query(char* input, bool quiet) {
  // input = (char*)malloc(sizeof(char*)*101);
  /*
  //break if the input is CTRL D
  if((int)(*input)==0){
          //		free(input);
          printf("\n");
          break;
  }
  */
  // skip the loop if it does not fulfill the module 6 step 4 requirements
  char* currchar;
  char word[20] = "";
  char prevWord[20] = "";
  bool cont = true;
  bool valid = true;
  int wordcount = 0;
  char* w;
  // skip the loop if it does not fulfill the module 6 step 4 requirements
  cont = true;
  valid = true;
  wordcount = 0;
  currchar = input;
  queue_t* tempwords =
      qopen();  // module 6 step 4- for complex queries we need a temp query
                // to seperate phrases seperated by "or."
  queue_t* finaldocs = qopen();
  while (cont) {
    // skip past spaces and tabs, if any
    while (((int)(*currchar)) == 9 || ((int)(*currchar)) == 32) {
      currchar += sizeof(char);
    }
    // print and save current word
    if (sscanf(currchar, "%s", word) == 1) {
      if (!isValid(word, prevWord, wordcount)) {
        printf("[invalid query]\n");
        cont = false;
        valid = false;
      } else {
        currchar += strlen(word);
        normalizeWord(word);
        if (strcmp(word, "and") != 0) {
          if (strlen(word) >= 3 || strcmp(word, "or") != 0) {
            w = malloc(sizeof(char*) * 30);
            strcpy(w, word);
            qput(tempwords, w);  // add the query word to the tempqueue
          } else if (strcmp(word, "or") == 0) {  // if we find the word OR
            // make queue of docs for words in current queue
            //			printf("w = %s\n",w);
            queue_t* newdocs = getDocs(tempwords);
            //		printf("w = %s\n",w);
            docsCombine(finaldocs,
                        newdocs);  // combine latest rankings with old rankings
            qclose(newdocs);
          }
        }
      }
    } else {
      cont = false;
    }
    strcpy(prevWord, word);
    wordcount++;
  }
  if (!isValid("or", prevWord, wordcount) && valid) {
    // returns false if last word is a reserved word
    printf("[invalid query]\n");
    valid = false;
  }
  if (valid) {
    // add final queue of temp words
    // printf("w = %s\n",w);
    queue_t* newdocs = getDocs(tempwords);
    // printf("w = %s\n",w);
    docsCombine(finaldocs,
                newdocs);  // combine latest rankings with old rankings
    qclose(newdocs);

    sortDocs(finaldocs);
    if (quiet)
      qapply(finaldocs, printRankToFile);
    else
      qapply(finaldocs, printRank);
  }
  printf("> ");
  qclose(finaldocs);
  qclose(tempwords);
}

int main(int argc, char* argv[]) {
  const char* usage = "usage: query <pageDirectory> <indexFile> [-q]\n";
  if (argc < 3 || argc > 6) {
    // check that the acceptable number of arguments are inputted
    printf("%s", usage);
    return -1;
  }
  sscanf(argv[1], "%s", pagedir);
  if (access(pagedir, R_OK) != 0) {
    // check the entered directory is readable
    printf("The specified page directory does not exist.\n");
    return -1;
  }
  char indexnm[100];
  sscanf(argv[2], "%s", indexnm);
  if (access(indexnm, R_OK) != 0) {
    // check the entered index file is readable
    printf("The specified index file does not exist.\n");
    return -1;
  }
  char input[500];
  index = indexload(pagedir, indexnm);
  char flag[3];
  if (argc > 3) {
    sscanf(argv[3], "%s", flag);
    if (strcmp(flag, "-q") != 0) {
      printf("%s", usage);
    } else {
      if (argc != 6) {
        // check all the outputs are entered for quiet loading
        printf("%s", usage);
        closeIndex(index);
        return -1;
      }
      char filename[100];
      char outnm[100];
      sscanf(argv[4], "%s", filename);
      sscanf(argv[5], "%s", outnm);
      if (access(filename, R_OK) != 0) {
        // check the query file is readable
        printf("File %s does not exist.\n", filename);
        closeIndex(index);
        return -1;
      } else {
        // query from file
        FILE* fp;
        fp = fopen(filename, "r");
        out = fopen(outnm, "w");
        while (fgets(input, 100, fp) != NULL) {
          fprintf(out, "%s", input);
          query(input, true);
        }
        fclose(fp);
        fclose(out);
      }
    }
  } else {
    // query from input
    printf("> ");
    while (fgets(input, 100, stdin) != NULL) {
      query(input, false);
    }
  }
  closeIndex(index);
  return 0;
}