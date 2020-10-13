#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_car(car_t *car);
car_t *make_car(char plate[], double, int);

car_t *front = NULL;

int main(void) {
  int fails = 0;
  car_t *c1 = make_car("123456", 9999.99, 2014);
  car_t *c2 = make_car("Darty1", 120000, 2019);
  car_t *c3 = make_car("SUNGOD", 80080.42, 2020);
  car_t *c4 = make_car("DARTXC", 1769.99, 1769);

	


	//Test 1: lget() from an empty list
	printf("Test 1: Get from an empty list\n");

  if (lget() != NULL) {
    printf("Failed: get from an empty list\n");
    fails++;
  }
	printf("Test 1 complete\n\n");
	
	//Test 2: lapply to an empty list
  printf("Test 2: Apply to an empty list\n");
	lapply(print_car);

	printf("Test 2 complete\n\n");
	
	
	//Test 3: remove from an empty list
  printf("Test 3: Remove from empty list\n");
	if (lremove("DARTXC") != NULL) {
		printf("Failed: remove from an empty list");
	}
	printf("Test 3 complete\n\n");
	
	//Test 4: put to an empty list
	
	printf("Test 4: Put to an empty list\n");
  if (lput(c1) != 0) {
    printf("Failed: put to an empty list\n");
    fails++;
  }
	printf("Test 4 complete\n\n");
	
	printf("Test 5: Put to a nonempty list\n");
  if (lput(c2) != 0) {
    printf("Failed: put to a non-empty list\n");
    fails++;
  }

	printf("Test 5 complete\n\n");

	printf("Test 6: get from a nonempty list\n");
  if (lget() == NULL) {
    printf("Failed: get from a non-empty list\n");
    fails++;
  }
  printf("Test 6 complete\n\n");
	
  lput(c3);
  lput(c4);
  printf("Test 7: Apply to a non-empty list\n");
  lapply(print_car);
	printf("Test 7 complete\n\n");

	printf("Test 8: Remove from end of non-empty list\n");
	printf("Removing plate 123456\n");
	if (lremove("123456") != c1) {
    printf("Failed: Remove from a non-empty list at the end\n");
    fails++;
  }
	printf("New list:\n");
	lapply(print_car);
	printf("Test 8 complete\n\n");

	printf("Test 9: Remove from start of non-empty list\n");
	printf("Removing plate DARTXC\n");
  if (lremove("DARTXC") != c4) {
    printf("Failed: Remove from a non-empty list at the beginning\n");
    fails++;
  }
	printf("New list:\n");
	lapply(print_car);
	printf("Test 9 complete\n\n");

	printf("Test 10: Remove from middle of non-empty list\n");
	lput(c1);
  lput(c2);
	printf("List:\n");
	lapply(print_car);
	printf("Removing plate 123456\n");
  if (lremove("123456") != c1) {
    printf("Failed: Remove from a non-empty list somewhere in the middle\n");
    fails++;
  }
	printf("New list:\n");
	lapply(print_car);
	printf("Test 10 complete\n\n");
	
  printf("Test completed with %d errors.\n", fails);
  free(c1);
  free(c2);
  free(c3);
  free(c4);
  exit(EXIT_SUCCESS);
	
}

car_t *make_car(char plate[], double price, int year) {
  car_t *new;
  if (!(new = (car_t*)malloc(sizeof(car_t)))) {
    printf("Malloc failure\n");
    return NULL;
  }
  strcpy(new->plate, plate);
  new->price = price;
  new->year = year;
  new->next = NULL;
  return new;
}

void print_car(car_t *car) {
  if (car != NULL) {
    printf("Plate: %s\n",car->plate);
    printf("Price: %lf\n", car->price);
    printf("Year: %d\n", car->year);
  }
  else {
    printf("Nothing to print.\n");
  }
	
}
