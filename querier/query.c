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


typedef struct{ //structure to hold data for every page in index
	int rank;
	int id;
	//	int occurences[34];
} query_docs_t;






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

/*
bool getDoc(void* doc, const void* id){
	return ((query_docs_t*)doc)->id  == *((int*)id);
}
*/


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
		
		//make string of all relevant index lines
		char* index_of_query = (char*)malloc(sizeof(char*)*1000);
		char* w = index_of_query; //pointer for writing
		FILE *fp;
		fp = fopen("../indexer/index.txt", "r"); //open the index for reading
		char indexWord[30];
		bool found = false;
		int docID;
		int occurrence;
		while (fscanf(fp, "%s ", indexWord) == 1 ){
			//if the index work is in the query, found is true 
			if(qsearch(words, stringCompare, (void*)indexWord)!=NULL){
				found = true;
				strcpy(w, indexWord);
				w += strlen(indexWord);
				*w = ' ';
				w+=sizeof(char);
			}
			char* c;;
			while(fscanf(fp, "%c", c)==1 && *c != '\n'){
				if(found){
					sprintf(w, "%c", *c);
					w+=sizeof(char);
				}
			}
			found = false;	
		}

		fclose(fp);
		printf("%s\n", index_of_query);
			
		queue_t* docs = qopen();
		queue_t* temp_docs = docs;


		//add documents for first word (if word exists) to docs queue

		if (sscanf(index_of_query, "%s ", indexWord) == 1 ){
			printf("inside\n");
			printf("indexWord = %s\n",indexWord);
			while(sscanf(index_of_query, "%d %d", &docID, &occurrence) == 2) {
				printf("id: %d\n",docID);
				if(occurrence>1){
					printf("id : %d had occurrencee>1\n",docID);
					query_docs_t* d = malloc(sizeof(query_docs_t));
					d->rank = occurrence;
					qput(docs, d);
				}
			}
		}

		
		qapply(docs, printRank);
		printf("here\n");
		//scan through rest of words
		//remove from queue of "surviving" docs and only re-add if document also has current word
		
		while (sscanf(index_of_query, "%s ", indexWord) == 1 ){
			query_docs_t* d;
			while( (d = (query_docs_t*)(qget(docs))) != NULL){
				
				while(sscanf(index_of_query, "%d %d", &docID, &occurrence) == 2) {
					if(d->id == docID){
						if(occurrence>0){
							if(occurrence<d->rank ){
								d->rank = occurrence;
							}
							qput(temp_docs, d);
						}
					}
				}
			}
			docs = temp_docs;
		}

		
		qapply(docs, printRank);
		

		
		//vvvv bad step 3 code vvvv
		
		/*
		if(isValid(input)){
			//find number of docs in index
			//seearch through index
			//if word is in queue of words
			//
			
			int numDocs = 0;
			char indexWord[30];
			int occurrence = 0;
			int docID;
			FILE *fp;
			
			
			//open index first time just to find number of documents
			fp = fopen("../indexer/index.txt", "r"); //open the index for reading
			if(fscanf(fp, "%s ", indexWord) == 1){
				while(fscanf(fp, "%d %d", &docID, &occurrence) == 2){
					numDocs++;
				}
			}
			fclose(fp);
		 
			//make queue of documents, type query_docs_t*
			queue_t* docs = qopen();
			
			for(int i = 1; i<= numDocs; i++){
				query_docs_t* d = (query_docs_t*)malloc(sizeof(query_docs_t));
				d->id = i;
				d->rank = 1000; //start with big number
				qput(docs, d);
			}
			
			
			//open index 2nd time to start actual loop
			fp = fopen("../indexer/index.txt", "r"); //open the index for reading
			
			
			int i = 0;//total number of query words found in index 
			bool found = false;

			while (fscanf(fp, "%s ", indexWord) == 1 ){
				//if the index work is in the query, found is true 
				if(qsearch(words, stringCompare, (void*)indexWord)!=NULL){
					found = true;
					i++;
				}
				
				while(fscanf(fp, "%d %d", &docID, &occurrence) == 2) {
					if(found){
						query_docs_t* d = (query_docs_t*)(qsearch(docs, getDoc, (void*)(&docID)));
						d->occurences[i] = occurrence;
						if(d->rank > occurrence){
							d->rank = occurrence;
						}
					}
				}



				found = false;
			}
			fclose(fp);
			
			//remove all documents that contained rank of zero
			for(int i = 1; i <= numDocs; i++){
				query_docs_t* d = (query_docs_t*)(qsearch(docs, getDoc, (&i)));
				if(d->rank==0){
					qremove(docs, getDoc, (&i));
					free(d);
				}
			}
			
			
			//only print if query was found in index
			if(i>0){
				qapply(docs, printRank); //print information for all remaining docs	
			}
			else {
				printf("No matches found\n");
			}
			
			


			qclose(docs);
		}



		*/
		qclose(docs);
			qclose(words);		
		free(input);
		free(index_of_query);
	}
	return 0;
	
}
