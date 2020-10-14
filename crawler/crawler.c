#include <stdio.h>
#include <webpage.h>
int main(void) {
    printf("hello\n");
		int INITDEPTH = 0;
		webpage_t page = *webpage_new("https://thayer.github.io/engs50/",INITDEPTH , NULL);

		if(webpage_fetch(page)){
			char *html = webpage_getHTML(page);
			printf("Found html: %s\n", html);

			int pos = 0;
			char *result;
			while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
				bool IsInternalURL(char *url);
				printf("Found url: %s\n", result);
				free(result);
			}
		}
		else{
			exit EXIT_FAILURE;
		}

		/*return 0;*/
}
