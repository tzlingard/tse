#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../lqueue.h"

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

void *qtestA(void *q) {
  lqueue_t *q1 = q;
  car_t *c1 = make_car("123123", 9999.99, 2014);
  car_t *c2 = make_car("ABCDEF", 120000, 2019);

  printf("Test 2A : Put to queue (Adds cars 123123 and ABCDEF)\n");
  lqput(q1, (void *)c1);
  lqput(q1, (void *)c2);
  printf("\n\n");

  printf("Test 3A : Apply print to nonempty queue\n");
  lqapply(q1, print_car);
  printf("\n\n");

	//  printf("Test 4A: Get and print from nonempty queue\n");
	// car_t* cc1 = (car_t *)lqget(q1);
  //print_car(cc1);
  //printf("\n\n");

  printf("Test 5A: Search for plate 'ABCDEF' from nonempty queue\n");
  char *plate1 = "ABCDEF";
  car_t *find_car1 = lqsearch(q1, platecheck, (void *)plate1);
  print_car(find_car1);
  printf("\n\n");

  printf("Test 6A: Search for plate 'THIS_SHOULD_FAIL' from nonempty queue\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lqsearch(q1, platecheck, (void *)plate2);
  print_car(find_car2);
  printf("\n\n");


  printf("Current queue:\n");
  lqapply(q1, print_car);

	char *plate3 = "123123";
  printf("Test 7A: Remove plate from  nonempty queue\n");
  printf("(Removing plates 123123 and ABCDEF)\n");
  lqremove(q1, platecheck, (void *)plate1);
	lqremove(q1, platecheck, (void *)plate3);
  printf("Queue after removal:\n");
  lqapply(q1, print_car);
  printf("\n\n");
	
  printf("Tests A  complete\n");
	return NULL;
}


void *qtestB(void *q) {
  lqueue_t *q1 = q;
  car_t *c1 = make_car("456456", 9999.99, 2014);
  car_t *c2 = make_car("GHIJKL", 120000, 2019);

  printf("Test 2B : Put to queue (Adds cars 456456 and GHIJKL)\n");
  lqput(q1, (void *)c1);
  lqput(q1, (void *)c2);
  printf("\n\n");

  printf("Test 3B : Apply print to nonempty queue\n");
  lqapply(q1, print_car);
  printf("\n\n");

	//  printf("Test 4B: Get and print from nonempty queue\n");
  //car_t* cc1 = (car_t *)lqget(q1);
	// print_car(cc1);
  //printf("\n\n");

  printf("Test 5B: Search for plate 'GHIJKL' from nonempty queue\n");
  char *plate1 = "GHIJKL";
  car_t *find_car1 = lqsearch(q1, platecheck, (void *)plate1);
  print_car(find_car1);
  printf("\n\n");

  printf("Test 6B: Search for plate 'THIS_SHOULD_FAIL' from nonempty queue\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lqsearch(q1, platecheck, (void *)plate2);
  print_car(find_car2);
  printf("\n\n");


  printf("Current queue:\n");
  lqapply(q1, print_car);

	char *plate3 = "456456";
  printf("Test 7B: Remove plate from  nonempty queue\n");
  printf("(Removing plates 456456 and GHIJKL)\n");
  lqremove(q1, platecheck, (void *)plate1);
	lqremove(q1, platecheck, (void *)plate3);
  printf("Queue after removal:\n");
  lqapply(q1, print_car);
  printf("\n\n");

  printf("Tests B complete\n");

	return NULL;
}


void *qtestC(void *q) {
  lqueue_t *q1 = q;
  car_t *c1 = make_car("789789", 9999.99, 2014);
  car_t *c2 = make_car("MNOPQR", 120000, 2019);

  printf("Test 2C : Put to queue (Adds cars 789789 and MNOPQR)\n");
  lqput(q1, (void *)c1);
  lqput(q1, (void *)c2);
  printf("\n\n");

  printf("Test 3C : Apply print to nonempty queue\n");
  lqapply(q1, print_car);
  printf("\n\n");

	//printf("Test 4C: Get and print from nonempty queue\n");
  //car_t* cc1 = (car_t *)lqget(q1);
  //print_car(cc1);
  //printf("\n\n");

  printf("Test 5C: Search for plate 'MNOPQR' from nonempty queue\n");
  char *plate1 = "MNOPQR";
  car_t *find_car1 = lqsearch(q1, platecheck, (void *)plate1);
  print_car(find_car1);
  printf("\n\n");

  printf("Test 6C: Search for plate 'THIS_SHOULD_FAIL' from nonempty queue\n");
  char *plate2 = "THIS_SHOULD_FAIL";
  car_t *find_car2 = lqsearch(q1, platecheck, (void *)plate2);
  print_car(find_car2);
  printf("\n\n");


  printf("Current queue:\n");
  lqapply(q1, print_car);

	char *plate3 = "789789";
  printf("Test 7C: Remove plate from  nonempty queue\n");
  printf("(Removing plates 789789 and MNOPQR)\n");
  lqremove(q1, platecheck, (void *)plate1);
	lqremove(q1, platecheck, (void *)plate3);
  printf("Queue after removal:\n");
  lqapply(q1, print_car);
  printf("\n\n");


  printf("Tests C complete\n");
	return NULL;
}



int main(void) {
  printf("Test 1: Open a new queue\n");
  lqueue_t *q1 = lqopen();
  printf("\n\n");
  pthread_t tid1, tid2, tid3;
  if (pthread_create(&tid1, NULL, qtestA, q1)!=0) {
		printf("Thread 1 failed\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&tid2, NULL, qtestB, q1)!=0) {
		printf("Thread 2 failed\n");
    exit(EXIT_FAILURE);
	}
	if (pthread_create(&tid3, NULL, qtestC, q1)!=0) {
		printf("Thread 3 failed\n");
    exit(EXIT_FAILURE);
  }

	
	if (pthread_join(tid1, NULL)!=0) {
		printf("Thread 1 failed\n");
    exit(EXIT_FAILURE);
  }
	
	if (pthread_join(tid2, NULL)!=0) {
		printf("Thread 2 failed\n");
    exit(EXIT_FAILURE);
  }
	
	if (pthread_join(tid3, NULL)!=0) {
		printf("Thread 3 failed\n");
    exit(EXIT_FAILURE);
  }

	sleep(2);
	printf("final queue: (should be empty)\n");
	lqapply(q1, print_car);
	lqclose(q1);
	
  exit(EXIT_SUCCESS);
}
