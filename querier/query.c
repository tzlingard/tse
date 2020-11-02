#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utils/hash.h"
#include "../utils/queue.h"



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
		currchar = input;//
		int occurrences[34]; //33 is the max number of 3+ letter words in 100 chars
		int i = 0;
		int rank = 1000;
		FILE *fp;
		while(cont) {
			//skip past spaces and tabs, if any
			while( ((int)(*currchar))==9 || ((int)(*currchar))==32) {	
				currchar+=sizeof(char);
			}
			//print current word
			if(sscanf(currchar, "%s", word)==1) {
				currchar+=strlen(word);
				normalizeWord(word);
				if(strcmp(word, "and") != 0 && strcmp(word, "or") != 0 && strlen(word) >= 3) {
					//skip the reserved words "and" and "or" and words less than 3 letters long
					char indexWord[30];
					int occurrence = 0;
					int docID;
					bool found = false;
					fp = fopen("../indexer/index", "r"); //open the index for reading
					while (fscanf(fp, "%s %d %d", indexWord, &docID, &occurrence) != EOF) {
						// scan through the index file
						if (strcmp(indexWord, word) == 0) {
							//if the word is found in the index, print it and add it
							printf("%s:%d ", word, occurrence);
							occurrences[i] = occurrence;
							i++;
							found = true;
						}
					}
					if (!found) {
						//if the word isn't found in the index, print it and add it to the list
						printf("%s:0 ", word);
						occurrences[i] = 0;
						i++;
						rank = 0;
					}
					fclose(fp);
				}
			}
			else {
				cont=false;
			}
		}
		int j;
		for (j=0; j<i; j++) {
			//find the minimum number of occurrences and set this as the rank
			if (occurrences[j] < rank) {
				rank = occurrences[j];
			}
		}
		if (i > 0) {
			printf("-- %d\n", rank);
		}
		else if (isValid(input)){
			printf("[Invalid query]\n");
		}
		free(input);
	}
	return 0;
}
