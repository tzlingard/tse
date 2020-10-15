#include <stdio.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include <string.h>

int main(void) {
    char *html;
    queue_t* internals= qopen();
    webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    if(webpage_fetch(page)) {
        html = webpage_getHTML(page);
        char* result;
        int i = 0;
        while ((i = webpage_getNextURL(page, i, &result)) > 0) {
            if(IsInternalURL(result)) {
                qput(internals, result);
                printf("Internal: %s\n",result);
            }
            else {
                printf("External: %s\n",result);
            }
            free(result);
        }
    }
    else {
        exit(EXIT_FAILURE);
    }
    int i;
    int styleCount = 0;
    for(i=1; i<sizeof(internals); ++i){
        webpage_t* internalPage = (webpage_t*)qget(internals); /*pop URL from queue, and store it*/  
        printf("URL: %s\n", webpage_getURL(internalPage)); /*print out URL*/
        
        char *html= webpage_getHTML(internalPage); /*get the html*/
        if (strcmp(html, "CodingStyle.html")){ /*if the html is CodingStyle.html*/
            styleCount=styleCount+1; /*add to our counter*/ 
        }
        qclose(internals);
        printf("There are %d CodingStyle.html entries\n", styleCount);
        
    }
    webpage_delete(page);
    exit(EXIT_SUCCESS);
    return 0;
}
