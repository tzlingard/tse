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

	while(true){
		input = (char*)malloc(sizeof(char*)*101);

		printf("> ");
		fgets(input, 100, stdin);

		//break if the input is CTRL D
		if((int)(*input)==0){
			free(input);
			break;
		}
		
		//skip the loop if the input has non numeric characters
		cont = true;
		if(!isValid(input)){
			printf("[invalid query]");
			cont = false;
		}
		
		
		currchar=input;//
		while(cont){
			//skip past spaces and tabs, if any
			while( ((int)(*currchar))==9 || ((int)(*currchar))==32){	
				currchar+=sizeof(char);
			}
			//print current word
			if(sscanf(currchar, "%s", word)==1){
				currchar+=strlen(word);
				normalizeWord(word);
				printf("%s ", word);
			}
			else{
				cont=false;
			}
		}
		free(input);
		printf("\n");
	}
	return 0;
}
