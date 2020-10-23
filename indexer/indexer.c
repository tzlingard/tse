#include <stdio.h>

#include "../utils/pageio.h"
#include "../utils/webpage.h"
int main(void) {
  webpage_t* top = pageload(1, "../pages");
  char* word = (char*)malloc(sizeof(char) * 20);
  while (webpage_getNextWord(top, 0, &word) != -1) {
    printf("%s", word);
  }
  free(word);
  webpage_delete(top);
  return 0;
}