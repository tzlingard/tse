#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lqueue.h"
#include "../lhash.h"

#define MAXREG 10
/* the representation of a car */
typedef struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
} car_t;

car_t *make_car(char plate[], double price, int year) {
  car_t *new;
  if (!(new = (car_t *)malloc(sizeof(car_t)))) {
    printf("Malloc failure\n");
    return NULL;
  }
  strcpy(new->plate, plate);
  new->price = price;
  new->year = year;
  new->next = NULL;
  return new;
}

void print_car(void *c) {
  car_t *car = (car_t *)c;
  if (car != NULL) {
    printf("Plate: %s\n", car->plate);
    printf("Price: %lf\n", car->price);
    printf("Year: %d\n", car->year);
  } else {
    printf("Nothing to print.\n");
  }
  printf("\n");
}

bool platecheck(void *c, const void *p) {
  car_t *car = (car_t *)c;
  char *plate = (char *)p;
  return (!strcmp(car->plate, plate));
}

void *htestA(void *h) {
  lhashtable_t *h1 = h;
  car_t *c1 = make_car("123123", 9999.99, 2014);
  car_t *c2 = make_car("ABCDEF", 120000, 2019);

  printf("Test 2A : Put to hash table (Adds cars 123123 and ABCDEF)\n");
  lhput(h1, (void *)c1, (char*)(c1->plate), sizeof((char*)(c1->plate)));
  lhput(h1, (void *)c2, (char*)(c2->plate), sizeof((char*)(c2->plate)));
  printf("\n\n");

  printf("Test 3A : Apply print to nonempty hashtable\n");
  lhapply(h1, print_car);
  printf("\n\n");

  //  printf("Test 4A: Get and print from nonempty queue\n");
  // car_t* cc1 = (car_t *)lqget(q1);
  // print_car(cc1);
  // printf("\n\n");

  printf("Test 4A: Search for plate 'ABCDEF' from nonempty hash table\n");
  char *plate1 = "ABCDEF";
  car_t *find_car1 = lhsearch(h1, platecheck, (void *)plate1, sizeof((void*)plate1));
  print_car(find_car1);
  printf("\n\n");

  printf("Test 5A: Search for plate 'THIS_SHOULD_FAIL' from nonempty hash table\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lhsearch(h1, platecheck, (void *)plate2, sizeof((void*)plate2));
  print_car(find_car2);
  printf("\n\n");

  printf("Current hash table:\n");
  lhapply(h1, print_car);

  char *plate3 = "123123";
  printf("Test 6A: Remove plate from  nonempty hash table\n");
  printf("(Removing plates 123123 and ABCDEF)\n");
  free(lhremove(h1, platecheck, (void *)plate1, sizeof((void*)plate1)));
  free(lhremove(h1, platecheck, (void *)plate3, sizeof((void*)plate3)));
  printf("Hash table after removal:\n");
  lhapply(h1, print_car);
  printf("\n\n");

  printf("Tests A  complete\n");
  return NULL;
}

void *htestB(void *h) {
  lhashtable_t *h1 = h;
  car_t *c1 = make_car("456456", 9999.99, 2014);
  car_t *c2 = make_car("GHIJKL", 120000, 2019);

  printf("Test 2B : Put to hash table (Adds cars 456456 and GHIJKL)\n");
	lhput(h1, (void *)c1, (char*)(c1->plate), sizeof((char*)(c1->plate)));
	lhput(h1, (void *)c2, (char*)(c2->plate), sizeof((char*)(c2->plate)));

  printf("\n\n");

  printf("Test 3B : Apply print to nonempty hash table\n");
  lhapply(h1, print_car);
  printf("\n\n");

  //  printf("Test 4B: Get and print from nonempty queue\n");
  // car_t* cc1 = (car_t *)lqget(q1);
  // print_car(cc1);
  // printf("\n\n");

  printf("Test 4B: Search for plate 'GHIJKL' from nonempty hash table\n");
  char *plate1 = "GHIJKL";
  car_t *find_car1 = lhsearch(h1, platecheck, (void *)plate1, sizeof((void*)plate1));
  print_car(find_car1);
  printf("\n\n");

  printf("Test 5B: Search for plate 'THIS_SHOULD_FAIL' from nonempty hash table\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lhsearch(h1, platecheck, (void *)plate2, sizeof((void*)plate2));
  print_car(find_car2);
  printf("\n\n");

  printf("Current hash table:\n");
  lhapply(h1, print_car);

  char *plate3 = "456456";
  printf("Test 6B: Remove plate from  nonempty hash table\n");
  printf("(Removing plates 456456 and GHIJKL)\n");
  free(lhremove(h1, platecheck, (void *)plate1, sizeof((void*)plate1)));
  free(lhremove(h1, platecheck, (void *)plate3, sizeof((void*)plate3)));
  printf("Hash table after removal:\n");
  lhapply(h1, print_car);
  printf("\n\n");

  printf("Tests B complete\n");

  return NULL;
}

void *htestC(void *h) {
  lhashtable_t *h1 = h;
  car_t *c1 = make_car("789789", 9999.99, 2014);
  car_t *c2 = make_car("MNOPQR", 120000, 2019);

  printf("Test 2C : Put to hash table (Adds cars 789789 and MNOPQR)\n");
	lhput(h1, (void *)c1, (char*)(c1->plate), sizeof((char*)(c1->plate)));
	lhput(h1, (void *)c2, (char*)(c2->plate), sizeof((char*)(c2->plate)));
  printf("\n\n");

  printf("Test 3C : Apply print to nonempty hash table\n");
  lhapply(h1, print_car);
  printf("\n\n");

  // printf("Test 4C: Get and print from nonempty queue\n");
  // car_t* cc1 = (car_t *)lqget(q1);
  // print_car(cc1);
  // printf("\n\n");

  printf("Test 4C: Search for plate 'MNOPQR' from nonempty hash table/n");
  char *plate1 = "MNOPQR";
  car_t *find_car1 = lhsearch(h1, platecheck, (void *)plate1, sizeof((void*)plate1));
  print_car(find_car1);
  printf("\n\n");

  printf("Test 5C: Search for plate 'THIS_SHOULD_FAIL' from nonempty hash table\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lhsearch(h1, platecheck, (void *)plate2, sizeof((void*)plate2));
  print_car(find_car2);
  printf("\n\n");

  printf("Current hash table:\n");
  lhapply(h1, print_car);

  char *plate3 = "789789";
  printf("Test 6C: Remove plate from  nonempty hash table\n");
  printf("(Removing plates 789789 and MNOPQR)\n");
  free(lhremove(h1, platecheck, (void *)plate1, sizeof((void*)plate1)));
  free(lhremove(h1, platecheck, (void *)plate3, sizeof((void*)plate3)));
  printf("Hash table after removal:\n");
  lhapply(h1, print_car);
  printf("\n\n");

  printf("Tests C complete\n");
  return NULL;
}

int main(void) {
  printf("Test 1: Open a new hasahtable\n");
  lhashtable_t *h1 = lhopen(10);
  printf("\n\n");
  pthread_t tid1, tid2, tid3;
  if (pthread_create(&tid1, NULL, htestA, h1) != 0) {
    printf("Thread 1 failed\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&tid2, NULL, htestB, h1) != 0) {
    printf("Thread 2 failed\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&tid3, NULL, htestC, h1) != 0) {
    printf("Thread 3 failed\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid1, NULL) != 0) {
    printf("Thread 1 failed\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid2, NULL) != 0) {
    printf("Thread 2 failed\n");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid3, NULL) != 0) {
    printf("Thread 3 failed\n");
    exit(EXIT_FAILURE);
  }

  sleep(2);
	printf("final hash table: (should be empty)\n");
  lhapply(h1, print_car);
  lhclose(h1);

  exit(EXIT_SUCCESS);
}
