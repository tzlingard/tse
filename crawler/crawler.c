#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../utils/hash.h"
#include "../utils/lhash.h"
#include "../utils/lqueue.h"
#include "../utils/pageio.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"

lqueue_t *internals;
lhashtable_t *table;

typedef struct {
  char seedurl[80];
  char pagedir[80];
  int maxDepth;
} pageData_t;

void printURL(void *page) {
  if (page == NULL) {
    return;
  }
  printf("%s\n", webpage_getURL((webpage_t *)page));
}

bool urlcheck(void *elementp, const void *searchkeyp) {
  char *p = (char *)elementp;
  char *url = (char *)searchkeyp;
  return (!strcmp(p, url));
}

void *crawlPages(void *p) {
  pageData_t *data = (pageData_t *)p;
  webpage_t *page = webpage_new(data->seedurl, 0, NULL);
  lqput(internals, page);
  webpage_t *new;
  int id = 0;
  char *pageCopy = (char *)calloc(strlen(data->seedurl) + 1, sizeof(char));
  strcpy(pageCopy, data->seedurl);
  lhput(table, pageCopy, pageCopy, strlen(pageCopy));
  int depth = 0;
  webpage_t *qp1;
  while ((qp1 = (webpage_t *)lqget(internals)) != NULL) {
    // TODO: ensure the program doesn't terminate before all pages are saved
    // (queue can be empty but not all pages retrieved yet)
    depth = webpage_getDepth(qp1);
    if (webpage_fetch(qp1)) {
      pagesave(qp1, ++id, data->pagedir);
      if (depth < data->maxDepth) {
        int i = 0;
        char *result = NULL;
        while ((i = webpage_getNextURL(qp1, i, &result)) > 0) {
          if (lhsearch(table, urlcheck, result, strlen(result)) == NULL &&
              IsInternalURL(result)) {  // if the given URL isn't already in the
                                        // hash table
            printf("depth = %d\n", depth);
            new = webpage_new(result, depth + 1,
                              NULL);  // create a new webpage for the given URL
            lqput(internals, new);
            printf("Added to queue: %s\n", result);
            lhput(table, result, result, strlen(result));
          } else {  // if URL is already in hash table
            free(result);
          }
          printf("\n");
        }
      }
    }
    webpage_delete(qp1);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  const char *usage =
      "usage: crawler <seedurl> <pagedir> <maxdepth> <numThreads>";
  char seedurl[80], pagedir[80];
  int maxdepth, numThreads, i;
  if (argc != 5) {  // ensure 4 arguments after crawler
    printf("%s\n", usage);
    return -1;
  }
  sscanf(argv[1], "%s", seedurl);  // scan args into their respective variables
  sscanf(argv[2], "%s", pagedir);
  sscanf(argv[3], "%d", &maxdepth);
  sscanf(argv[4], "%d", &numThreads);
  if (access(pagedir, R_OK) != 0) {  // fail if page directory cannot be read
    printf("Page directory is not valid.\n%s\n", usage);
    return -1;
  }
  if (maxdepth < 0) {  // fail if max depth is negative
    printf("Enter a non-negative max depth.\n%s\n", usage);
    return -1;
  }
  if (numThreads < 0) {  // fail if max depth is negative
    printf("Enter a non-negative number of threads.\n%s\n", usage);
    return -1;
  }
  pthread_t threads[numThreads];
  pageData_t *p = (pageData_t *)malloc(sizeof(pageData_t));
  strcpy(p->seedurl, seedurl);
  strcpy(p->pagedir, pagedir);
  p->maxDepth = maxdepth;
  internals = lqopen();
  table = lhopen(80);
  for (i = 0; i < numThreads; i++) {
    if (pthread_create(&threads[i], NULL, crawlPages, (void *)p) != 0) {
      // TODO: move crawler code into a function crawlPages and decide what arg
      // to pass to it (queue of pages and hashtable of URLs are both shared)
      printf("Thread %d failed to be created.\n", i + 1);
      exit(EXIT_FAILURE);
    } else {
      printf("Thread %d created\n", i + 1);
    }
  }
  for (i = 0; i < numThreads; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      printf("Thread %d failed to join.\n", i);
      exit(EXIT_FAILURE);
    } else {
      printf("Thread %d finished\n", i + 1);
    }
  }
  free(p);
  lhclose(table);
  printf("closed table\n");
  lqclose(internals);
  printf("closed queue\n");
  return 0;
}