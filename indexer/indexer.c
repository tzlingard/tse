#include <stdio.h>

#include "../utils/pageio.h"
#include "../utils/webpage.h"
int main(void) {
  webpage_t* top = pageload(1, "../pages");
	printf("%s\n",webpage_getURL(top));
	char* word = (char*)malloc(sizeof(char*));
	int pos = 0;
	while((pos = webpage_getNextWord(top,pos, &word))>0){
		printf("%s ", word);
		//		printf("pos = %d\n",pos);
		free(word);		
		}

  webpage_delete(top);
  return 0;
}
