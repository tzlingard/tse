#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>


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

void print_car(void *c) {
	car_t* car = (car_t*)c;
  if (car != NULL) {
    printf("Plate: %s\n",car->plate);
    printf("Price: %lf\n", car->price);
    printf("Year: %d\n", car->year);
  }
  else {
    printf("Nothing to print.\n");
  }
	printf("\n");
}

bool platecheck(void* c, const void* p){
	car_t *car = (car_t*)c;
	char *plate = (char*)p;
	return (!strcmp(car->plate, plate));
}

car_t *front = NULL;

int main(void) {
  
  car_t *c1 = make_car("123456", 9999.99, 2014);
  car_t *c2 = make_car("Darty1", 120000, 2019);
  car_t *c3 = make_car("SUNGOD", 80080.42, 2020);
  car_t *c4 = make_car("DARTXC", 1769.99, 1769);
	car_t *c5 = make_car("CS5050", 5050.50, 1950);
	car_t *c6 = make_car("ENGS50", 5050.50, 1950);



	
	printf("Test 1: Open a new queue\n");
	queue_t *q1 = qopen();
	printf("\n\n");
	
	printf("Test 2: Get and print from empty queue\n");
	car_t *cc1 = (car_t*)qget(q1);
	print_car(cc1);
	printf("\n\n");

	printf("Test 3 : Put to queue (Adds 4 cars)\n");
	qput(q1,(void*)c1);
	qput(q1,(void*)c2);
	qput(q1,(void*)c3);
	qput(q1,(void*)c4);
	qput(q1,(void*)c5);
	qput(q1,(void*)c6);
	printf("\n\n");

	printf("Test 4 : Apply print to nonempty queue\n");
	qapply(q1, print_car);
	printf("\n\n");
		
	printf("Test 5: Get and print from nonempty queue\n");
	cc1 = (car_t*)qget(q1);
	print_car(cc1);
	printf("\n\n");
 
	printf("Test 6: Search for plate 'SUNGOD' from nonempty queue\n");
	char* plate1 = "SUNGOD";
	car_t *find_car1 = qsearch(q1, platecheck, (void*)plate1);
 	print_car(find_car1);
	printf("\n\n");

	printf("Test 7: Search for plate 'THIS_SHOULD_FAIL' from nonempty queue\n");
	char* plate2 = "THIS_SHOULD_FAIL";
	car_t *find_car2 = qsearch(q1, platecheck, (void*)plate2);
	print_car(find_car2);
	printf("\n\n");

	char* plate3 = "Darty1";
	char* plate4 = "DARTXC";
	char* plate5 = "ENGS50";
	

	printf("Current queue:\n");
	qapply(q1, print_car);
	
	printf("Test 8: Remove plate from front of nonempty queue\n");
	printf("(Removing plate Darty1)\n");
	qremove(q1, platecheck, (void*)plate3);
	printf("Queue after removal:\n");
	qapply(q1, print_car);
	printf("\n\n");
 
	printf("Test 9: Remove plate from middle of nonempty queue\n");
	printf("(Removing plate DARTXC)\n");
	qremove(q1, platecheck, (void*)plate4);
	printf("Queue after removal:\n");
	qapply(q1, print_car);
	printf("\n\n");
 
	printf("Test 10: Remove plate from end of nonempty queue\n");
	printf("(Removing plate ENGS50)\n");
	qremove(q1, platecheck, (void*)plate5);
	printf("Queue after removal:\n");
	qapply(q1, print_car);
	printf("\n\n");
 
	printf("Test 11: Remove plate 'THIS SHOULD FAIL' from nonempty queue\n");
	qremove(q1, platecheck, (void*)plate2);
	printf("Queue after removal:\n");
	qapply(q1, print_car);
	printf("\n\n");

	printf("Test 12: Concat 2 queues\n");
	queue_t *q2 = qopen();
	qput(q2, c1);
	qput(q2, c6);
	printf("Queue 1:\n");
	qapply(q1, print_car);
	printf("Queue 2:\n");
	qapply(q2, print_car);
	printf("Queue 1 after concat:\n");
	qconcat(q1, q2);
	qapply(q1, print_car);
	printf("\n\n");

	printf("Test 13: Close queue\n");
	printf("\n\n");
	qclose(q1);
	
	printf("Tests complete\n");
  free(c1);
  free(c2);
  free(c3);
  free(c4);
	free(c5);
	free(c6);
  exit(EXIT_SUCCESS);
	
}
