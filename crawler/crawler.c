#include <stdio.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include <string.h>



void printURL(void *page){
	if(page==NULL){return;}
	printf("%s\n",webpage_getURL((webpage_t*)page));
}


int main(void) {
	queue_t* internals= qopen();
	webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	webpage_t *new;

	if(webpage_fetch(page)) {
		//		char* html = webpage_getHTML(page);
		
		char* result = NULL;
		int i = 0;
		
		while ((i = webpage_getNextURL(page, i, &result)) > 0) {
			//if(strcmp(result, "https://thayer.github.io/engs50/")!=0){
				if(IsInternalURL(result)) {
					new = webpage_new(result, 1, NULL); //create a new webpage for the given URL
					printf("Putting %s into queue\n", result);
					qput(internals, new);
					printf("Internal: %s\n",result);
				}
				else {
					printf("External: %s\n",result);
				}
				//if(strcmp(result, "https://thayer.github.io/engs50/")!=0){
					free(result);
					//}
			//}
			printf("\n");
		}
	
	}
	else {
		exit(EXIT_FAILURE);
	}
	printf("\n");

	//print all urls in queue
	printf("printing queue\n");
	qapply(internals, printURL);

	/*
	printf("\n");
	//Ensure that there are 2 stylecounts
	int i;
	int styleCount = 0;
	char* styleURL = "https://thayer.github.io/engs50/Resources/CodingStyle.html";
	char* currURL;
	int len = sizeof(internals);
	for(i=0; i<len-1; i++){
		webpage_t* p = (webpage_t*)(qget(internals));
		currURL = webpage_getURL(p);
		printf("URL: %s\n", currURL);
		if (strcmp(currURL, styleURL)==0){ 
			styleCount=styleCount+1; 
		}
		webpage_delete(p);
	}
	//	printf("There are %d CodingStyle.html entries\n", styleCount);
	*/

	//free all memory stored in queue
	webpage_t* site = NULL;
	while((site = (webpage_t*)qget(internals))!=NULL){
		free(webpage_getURL(site));
		free(webpage_getHTML(site));
		free(site);
	}
	webpage_delete(page);	
	qclose(internals);

	exit(EXIT_SUCCESS);
	return 0;
}

