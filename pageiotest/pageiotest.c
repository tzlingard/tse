#include <stdio.h>

#include "pageio.h"
#include "webpage.h"

int main(void) {
	//	FILE *fp = fopen("../pages/1", "r");
  webpage_t* originalPage = pageload(1, "../pages");
	printf("Page Save: %d\n", pagesave(originalPage, 1, "."));
	webpage_t* savedPage = pageload(1, ".");
	
	char* originalURL = webpage_getURL(originalPage);
	int originalDepth = webpage_getDepth(originalPage);
	char* originalHTML = webpage_getHTML(originalPage);
	int originalHTMLlen = webpage_getHTMLlen(originalPage);
	char* savedURL = webpage_getURL(savedPage);
	int savedDepth = webpage_getDepth(savedPage);
	char* savedHTML = webpage_getHTML(savedPage);
	int savedHTMLlen = webpage_getHTMLlen(savedPage);
	printf("Difference in URLs: %d\n", strcmp(originalURL, savedURL));
	printf("Difference in Depths: %d\n", (originalDepth-savedDepth));
	printf("Difference in HTMLs: %d\n", strcmp(originalHTML, savedHTML));
	printf("Difference in HTML Lenghts: %d\n", (originalHTMLlen-savedHTMLlen));
	


  webpage_delete(originalPage);
  webpage_delete(savedPage);
  return 0;
}
