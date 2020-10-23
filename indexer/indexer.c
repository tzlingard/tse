#include <stdio.h>
#include <ctype.h>
#include "../utils/pageio.h"
#include "../utils/webpage.h"


void normalizeWord(char* word){




	int count = 0;
	
	while(word[count] != 0){
		char c = tolower(word[count]);
		word[count] = c;
		count++;
	}
	if(count<3){
		word = NULL;
		printf("word should be null. word is %s\n",word);
	}
	//printf("count = %d\n",count);
}

int main(void) {
	//char* test = malloc(5);
	//strcpy(test, "TeSt");
	//normalizeWord(test);
	//printf("normalized word = %s\n",test);
	
  webpage_t* top = pageload(1, "../pages");
	printf("%s\n",webpage_getURL(top));
	char* word;
	int pos = 0;
	while((pos = webpage_getNextWord(top,pos, &word))>0){
		normalizeWord(word);
		if(word != NULL){
			printf("%s\n", word);
		}
		free(word);		
	}


  webpage_delete(top);
	
  return 0;
}
