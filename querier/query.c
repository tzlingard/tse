#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "../utils/hash.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"
#include "../utils/pageio.h"
#include "../utils/indexio.h"


typedef struct{ //structure to hold data for every page in index
	int rank;
	int id;
	int occurences[34];
} query_docs_t;




typedef struct{
	char* word;
	queue_t* freq;
}word_t;


typedef struct{
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

bool isValid(char* c){
	char* curr = c;
	while((int)(*curr)!=10){ //10 is line feed
		if(!isalpha(*curr)){
			if( ! (((int)(*curr))==9 || ((int)(*curr))==32)){
				return false;
			}
		}
		curr+=1;
	}
	return true;
}

//true if the given query_docs_t has given id


bool findID(void* doc, const void* id){
	return ((docs_t*)doc)->id  == *((int*)id);
}



bool findWord(void* indexWord, const void* word){
	return strcmp( ((word_t*)indexWord)->word, (char*)word)==0;
}

bool stringCompare(void* word1, const void* word2){
	return strcmp((char*)word1, (char*)word2)==0;
}


void printWord(void* word){
	printf("%s ", ((char*)word));
}


void printRank(void* doc){
	query_docs_t* d = (query_docs_t*)doc;
	webpage_t* p = pageload(d->id, "../pages");
	printf("rank: %d doc: %d URL: %s\n", d->rank, d->id, webpage_getURL(p));
	webpage_delete(p);
}



int main(int argc, char *argv[]) {
	char* input;
	char* currchar;
	char  word[20];
	bool cont = true;

	hashtable_t* index = indexload("../indexer", "index.txt");
	while(true) {
		input = (char*)malloc(sizeof(char*)*101);
		printf("> ");
		fgets(input, 100, stdin);

		//break if the input is CTRL D
		if((int)(*input)==0){
			free(input);
			printf("\n");
			break;
		}
		
		//skip the loop if the input has non numeric characters
		cont = true;		
		if(!isValid(input)){
			printf("[invalid query]\n");
			cont = false;
		}
		
		currchar = input; 
		queue_t* words = qopen(); 

		while(cont) {
			//skip past spaces and tabs, if any
			while( ((int)(*currchar))==9 || ((int)(*currchar))==32) {	
				currchar+=sizeof(char);
			}
			//print and save current word
			if(sscanf(currchar, "%s", word)==1) {
				currchar+=strlen(word);
				normalizeWord(word);
				if(strcmp(word, "and") != 0 && strcmp(word, "or") != 0 && strlen(word) >= 3) {
					//skip the reserved words "and" and "or" and words less than 3 letters long
					char* w = malloc(sizeof(char*)*30);
					strcpy(w, word);
					qput(words, w); //add the query word to the queue
				}
			}
			else {
				cont=false;
			}
		}



		queue_t* docs = qopen();



		//add documents for first word (if word exists) to docs queue


		char* currWord;
		if((currWord = (char*)(qget(words)))!=NULL){
			word_t* indexWord = hsearch(index, findWord, currWord, sizeof(currWord));
			queue_t* temp = qopen();
			docs_t* doc;
			while((doc  = (docs_t*)qget(indexWord->freq))!=NULL){
				if(doc->freq > 0){
					query_docs_t* d = malloc(sizeof(query_docs_t));
					d->rank = doc->freq;
					qput(docs, d);
				}
				qput(temp, doc);
			}
			indexWord->freq = temp;
		}

		//scan through rest of words
		//remove from queue of "surviving" docs and only re-add if document also has current word


		while((currWord = (char*)(qget(words)))!=NULL){
			word_t* indexWord = hsearch(index, findWord, currWord, sizeof(currWord));
			query_docs_t* d;
			queue_t* temp_docs = qopen();
			while( (d = (query_docs_t*)(qget(docs))) != NULL){
				int freq = (((docs_t*)qsearch(indexWord->freq, findID, &(d->id))))->freq;
				if(freq > 0){
					if(freq<d->rank){
						d->rank = freq;
					}
					qput(temp_docs, d);
				}
			}
			docs = temp_docs;
		}
		
		qapply(docs, printRank);
		

		
		qclose(docs);
			qclose(words);		
		free(input);
	}
	return 0;
	
}