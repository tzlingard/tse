#include <stdio.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include <string.h>

int main(void) {
	char *html = NULL;
	queue_t* internals= qopen();
	webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if(webpage_fetch(page)) {
		html = webpage_getHTML(page);
		char* result = NULL;
		int i = 0;
		while ((i = webpage_getNextURL(page, i, &result)) > 0) {
			if(IsInternalURL(result)) {
				qput(internals, result);
				printf("Internal: %s\n",result);
			}
			else {
				printf("External: %s\n",result);
			}
			//free(result);
			// ^can't free yet or stylecount check wont work
		}
	}
	else {
		exit(EXIT_FAILURE);
	}
	
	
	//Ensure that there are 2 stylecounts
	int i;
	int styleCount = 0;
	char* styleURL = "https://thayer.github.io/engs50/Resources/CodingStyle.html";
	for(i=1; i<sizeof(internals); ++i){
		char* currURL = (char*)(qget(internals)); /*pop URL from queue, and store it*/  
		//	printf("URL: %s\n", currURL);
    
		
		if (strcmp(currURL, styleURL)==0){ /*if the html is CodingStyle.html*/
			styleCount=styleCount+1; /*add to our counter*/ 
		}
	}
	while(qget(internals)!=NULL){
	}
	
	qclose(internals);
	printf("There are %d CodingStyle.html entries\n", styleCount);
  
  
	webpage_delete(page);
	exit(EXIT_SUCCESS);
	return 0;
}
