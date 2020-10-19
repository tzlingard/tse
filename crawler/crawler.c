#include <stdio.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include "../utils/hash.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

bool checkURLs(void* url1, const void* url2) {
	if (strcmp((char*)url1, (char*) url2)) {
		return false;
	}
	else return true;
}

void printURL(void *page){
	if(page==NULL){return;}
	printf("%s\n",webpage_getURL((webpage_t*)page));
}


bool urlcheck(void* elementp, const void* searchkeyp){
	webpage_t* p = (webpage_t*)elementp;
	char* url = (char*)searchkeyp;
	return (!strcmp(webpage_getURL(p),url));
}

int32_t pagesave(webpage_t* pagep, int id, char* dirname) {
	char dest[80];
	sprintf(dest, "%s/%d", dirname, id);
	printf("dirname = %s, dest = %s\n", dirname, dest);
	if (access(dest, F_OK)) { //if the file doesn't exist yet
		FILE* fp;
		fp = fopen(dest, "w+");
		fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep));
		fclose(fp);
		return 0;
	}
	else {
		return -1;
	}
}

int main(void) {
	queue_t* internals= qopen();
	hashtable_t* table = hopen(20); //not sure if 20 is a good number to use but i dont think it really matters
	webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	pagesave(page, 1, "../pages");
	webpage_t *new;
	if(webpage_fetch(page)) {
		//		char* html = webpage_getHTML(page);
		char* result = NULL;
		int i = 0;
		hput(table, (void*)page, (webpage_getURL(page)), strlen(webpage_getURL(page)));
		while ((i = webpage_getNextURL(page, i, &result)) > 0) {
			if(hsearch(table, urlcheck, result, sizeof(result))==NULL){
				if(IsInternalURL(result)) {
					new = webpage_new(result, 1, NULL); //create a new webpage for the given URL
					qput(internals, new);
					//printf("Internal: %s\n",result);
					hput(table, (void*)new, (webpage_getURL(new)), sizeof(webpage_getURL(new)));
				}
				else {
					//printf("External: %s\n",result);
				}
				
			}
			free(result);
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
	hclose(table);
	
	webpage_delete(page);	
	qclose(internals);
	exit(EXIT_SUCCESS);
	return 0;
	
}

