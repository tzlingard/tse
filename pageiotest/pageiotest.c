#include <stdio.h>

#include "pageio.h"
#include "webpage.h"

int main(void) {
  webpage_t* page = pageload(2, "../pages");
  printf("URL: %s\n", webpage_getURL(page));
  printf("Depth: %d\n", webpage_getDepth(page));
  printf("HTML:\n%s\n", webpage_getHTML(page));
  webpage_delete(page);
  return 0;
}