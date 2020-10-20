#include <stdio.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include "../utils/hash.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

/*bool checkURLs(void* url1, const void* url2) {
	if (strcmp((char*)url1, (char*) url2)) {
		return false;
	}
	else return true;
}*/

void printURL(void *page){
	if(page==NULL){return;}
	printf("%s\n",webpage_getURL((webpage_t*)page));
}


bool urlcheck(void* elementp, const void* searchkeyp){
	char* p = (char*)elementp;
	char* url = (char*)searchkeyp;
	return (!strcmp(p,url));
}

int32_t pagesave(webpage_t* pagep, int id, char* dirname) {
	char dest[80];
	sprintf(dest, "%s/%d", dirname, id);
	if (access(dest, F_OK)) { //if the file doesn't exist yet
		FILE* fp;
		fp = fopen(dest, "w");
		fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep));
		fclose(fp);
		printf("Saved: %s\n",webpage_getURL(pagep));
		return 0;
	}
	else {
		return -1;
	}
}

int main(int argc, char* argv[]) {
	const char* usage = "usage: crawler <seedurl> <pagedir> <maxdepth>";
	char seedurl[80], pagedir[80];
	int maxdepth;
	if (argc != 4) { //ensure 3 arguments after crawler
		printf("%s\n", usage);
		return -1;
	}
	sscanf(argv[1], "%s", seedurl); //scan the arguments into their respective variables
	sscanf(argv[2], "%s", pagedir);
	sscanf(argv[3], "%d", &maxdepth);
	if (access(pagedir, R_OK) != 0) { //fail if page directory cannot be read
		printf("Page directory is not valid.\n%s\n",usage);
		return -1;
	}
	if (maxdepth < 0) { //fail if max depth is negative
		printf("Enter a non-negative max depth.\n%s\n",usage);
		return -1;
	}
	queue_t* internals= qopen();
	hashtable_t* table = hopen(20); //not sure if 20 is a good number to use but i dont think it really matters
	webpage_t *page = webpage_new(seedurl, 0, NULL);
	qput(internals, page);
	webpage_t *new;
	if(webpage_fetch(page)) {
		int id = 1;
		pagesave(page, id, pagedir);
		id++;
		char* result = NULL;
		int i = 0;
		char* pageCopy = (char*)calloc(strlen(seedurl) + 1, sizeof(char));
		strcpy(pageCopy, webpage_getURL(page));
		hput(table, pageCopy, pageCopy, strlen(pageCopy));
		int depth = 1;
		printf("depth = %d, maxDepth = %d\n", depth, maxdepth);
		webpage_t* qp1;
		while ((qp1 = (webpage_t*)qget(internals))) {
			printf("qp1 = %s\n",webpage_getURL(qp1));
			depth = webpage_getDepth(qp1);
			while ((i = webpage_getNextURL(qp1, i, &result)) > 0 && depth < maxdepth) {
				if(hsearch(table, urlcheck, result, strlen(result))==NULL && IsInternalURL(result)) { //if the given URL isn't already in the hash table
					new = webpage_new(result, depth, NULL); //create a new webpage for the given URL
					if (webpage_fetch(new)) {
						pagesave(new, id, pagedir);
						id++;
					}
					qput(internals, new);
					hput(table, result, result, strlen(result));
				}
				else { //if URL is already in hash table
					free(result); 
				}
				printf("\n");
			}
		}
	}
	else {
		return -1;
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
	while((site = (webpage_t*)qget(internals))!=NULL) {
		webpage_delete(site);
	}
	hclose(table);
	qclose(internals);
	exit(EXIT_SUCCESS);
	return 0;
}

