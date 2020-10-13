#include <stdio.h>
#include "queue.h"
#include "hash.h"
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
	car_t *c6 = make_car("DARTXC", 5050.50, 1950);




	
	
	
	printf("Test 1: Open a new hashtable\n");
	hashtable_t *h1 = hopen(10);
	printf("\n\n");

	printf("Test 2: Apply to empty table\n");
	happly(h1, print_car);
	printf("\n\n");


	printf("Test 3 : Search from empty table\n");
	print_car(hsearch(h1, platecheck, "SUNGOD", sizeof("SUNGOD")));
	printf("\n\n");

	printf("Test 4 : Remove from empty table\n");
	hremove(h1, platecheck, "CS5050", sizeof("CS5O5O"));
	printf("\n\n");
	
	
	printf("Test 5 : Put to hash table (Adds 6 cars)\n");
	hput(h1,(void*)c1, (char*)(c1->plate), sizeof((char*)(c1->plate)));
	hput(h1,(void*)c2, (char*)(c2->plate), sizeof((char*)(c2->plate)));
	hput(h1,(void*)c3, (char*)(c3->plate), sizeof((char*)(c3->plate)));
	hput(h1,(void*)c4, (char*)(c4->plate), sizeof((char*)(c4->plate)));
	hput(h1,(void*)c5, (char*)(c5->plate), sizeof((char*)(c5->plate)));
	hput(h1,(void*)c6, (char*)(c6->plate), sizeof((char*)(c6->plate)));
	printf("\n\n");

	printf("Test 6 : Apply to hashtable (should print all cars in table) \n");
	happly(h1,print_car);
	printf("\n\n");

	printf("Test 7 : Search from table (should print car with 'SUNGOD' plate) \n");
	print_car(hsearch(h1, platecheck, "SUNGOD", sizeof("SUNGOD")));
	printf("\n\n");

	printf("Test 8 : Remove from hash (should remove car with 'CS5050' plate) \n");
	hremove(h1, platecheck, "CS5050", sizeof("CS5O5O"));
	printf("Table after removal:\n");
	happly(h1,print_car);
	printf("\n\n");

	printf("Test 9 : close hash \n");
	hclose(h1);
	printf("\n\n");
	printf("Tests complete\n");
  free(c1);
  free(c2);
  free(c3);
  free(c4);
	free(c5);
	free(c6);
  exit(EXIT_SUCCESS);
	
}
