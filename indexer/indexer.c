#include <stdio.h>

#include "../utils/pageio.h"
#include "../utils/webpage.h"
int main(void) {
  webpage_t* top = pageload(1, "../pages");
  char** word;
  webpage_getNextWord(top, 0, word);
  printf("%s", *word);
  free(word[0]);
  webpage_delete(top);
  return 0;
}