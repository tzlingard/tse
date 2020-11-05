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

bool isValid(char* c, char* cprev, int* wordcount) {
  char* curr = c;
  char* prev = cprev;
	int*  cnt= wordcount;
      //special cases:                                                                                                                      
      // OR is the first word: invalid query --  dealt with here                                                                           
      // OR is the last word: invalid query                                                                                                 
      // number is included: invalid query-- dealt with here                                                                                                
      // OR & AND follow each other: invalid query-- dealt with here                                                                                        
      //two consecutive ORs or ANDs: invalid query-- dealt with here
		while ((int)(*curr) != 10) {  // 10 is line feed
			if(isdigit(*curr){
					return false;
			}	
			else if (!isalpha(*curr)) {
				if (!(((int)(*curr)) == 9 || ((int)(*curr)) == 32)) {
					return false;
        }
			}
			else if( strcmp(*curr, "and")== 0  || (strcmp(*curr, "or")==0  ){
				if( strcmp(*prev, "and")== 0  || (strcmp(*prev, "or")==0 || cnt==0 ){
								return false;
				}
			}
	 
    curr += 1;
		}
			return true;		
}

// true if the given docs_t has given id
bool findID(void* doc, const void* id) {
  return ((docs_t*)doc)->id == *((int*)id);
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
  webpage_t* p = pageload(d->id, "../pages");
  printf("rank: %d doc: %d URL: %s\n", d->rank, d->id, webpage_getURL(p));
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
    printf("currWord = %s\n", currWord);
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
    } else {
      wordNotFound = true;
    }
    free(currWord);
  }
  // scan through rest of words
  // remove from queue of "surviving" docs and only re-add if document also
  // has current word
  while ((currWord = (char*)(qget(words))) != NULL && wordNotFound == false) {
    printf("currWord = %s\n", currWord);
    word_t* indexWord = hsearch(index, findWord, currWord, sizeof(currWord));
    queue_t* temp_docs = qopen();
    query_docs_t* d;
    if (indexWord != NULL) {
      printf("indexWord found\n");
      while ((d = (query_docs_t*)(qget(docs))) != NULL) {
        int freq =
            (((docs_t*)qsearch(indexWord->freq, findID, &(d->id))))->freq;
        printf("freq = %d\n", freq);
        if (freq > 0) {
          if (freq < d->rank) {
            d->rank = freq;
          }
          query_docs_t* qD;
          if ((qD = ((query_docs_t*)qsearch(docs, findID, &(d->id)))) != NULL) {
            d->rank = d->rank + qD->rank;
            // add the rank to the rank already in the final queue for that doc
          }
          qput(temp_docs, d);
        } else {
          free(d);
        }
      }
      /* query_docs_t* qDocs;
       while ((qDocs = qget(temp_docs)) != NULL) {
         qDocs->rank = (((docs_t*)qsearch(temp_docs, findID, &(d->id))))->freq;
         // set the rank in the final queue to the pre-or rank
         qput(docs, qDocs);
       }*/
    } else {
      if (!strcmp(currWord, "or")) {  // if the word is or, add the ranks
        query_docs_t* qDocs;
        while ((qDocs = qget(temp_docs)) != NULL) {
          qDocs->rank = (((docs_t*)qsearch(temp_docs, findID, &(d->id))))->freq;
          // set the rank in the final queue to the pre-or rank
          qput(docs, qDocs);
        }
      } else {
        wordNotFound = true;
        free(currWord);
      }
    }
  }
  if (wordNotFound == true) {
    printf("Word not found\n");
    qclose(docs);
    docs = qopen();
  }
  return docs;
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

int main(int argc, char* argv[]) {
  char input[100];
  char* currchar;
	char* pastchar= NULL;
	char word[20];
  bool cont = true;
  int wordcount= 0;
  index = indexload("../indexer", "index");
  printf("> ");
  while (fgets(input, 100, stdin) != NULL) {
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
    cont = true;
    if (!isValid(input,*pastchar,wordcount)) {
      printf("[invalid query]\n");
      cont = false;
    }
    pastchar = input;
    currchar = input;
    queue_t* tempwords = qopen(); //module 6 step 4- for complex queries we need a temp query to seperate phrases seperated by "or."  
		queue_t* finalwords =qopen();
    while (cont) {
      // skip past spaces and tabs, if any
      while (((int)(*currchar)) == 9 || ((int)(*currchar)) == 32) {
        currchar += sizeof(char);
				wordcount+=1;
      }
			


			
      // print and save current word
			/* if (sscanf(currchar, "%s", word) == 1) {
        currchar += strlen(word);
        normalizeWord(word);
        if (strcmp(word, "and") != 0) {
					
          if (strlen(word) >= 3 || strcmp(word, "or") == 0) {
            // skip the reserved words "and" and words less than 3 (except
            // "or") letters long
            char* w = malloc(sizeof(char*) * 30);
            strcpy(w, word);
            qput(words, w);  // add the query word to the queue
          }
        }
				
      } else {
        cont = false;
				}*/
    }

    queue_t* docs = getDocs(words);
    sortDocs(docs);
    qapply(docs, printRank);

    qclose(docs);
    qclose(words);
    // free(input);
    printf("> ");
  }
  closeIndex(index);
  return 0;
}
