#include <stdio.h>
#include "../utils/webpage.h"

int main(void) {
    char *html;
    webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    if(webpage_fetch(page)) {
        html = webpage_getHTML(page);
        char* result;
        int i = 0;
        while ((i = webpage_getNextURL(page, i, &result)) > 0) {
            if(IsInternalURL(result)) {
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
    webpage_delete(page);
    exit(EXIT_SUCCESS);
    return 0;
}