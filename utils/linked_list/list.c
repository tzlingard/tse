#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "list.h"

static car_t *front = NULL;

/* put(): place a car at the beginning of the list
 * returns 0 if successful; nonzero otherwise 
 */
int32_t lput(car_t *cp) {
  cp->next = front;
  front = cp;
  return 0;
}

/* get(): remove and return the first car in the list;
 * return NULL if the list is empty
 */
car_t *lget() {
  if (front == NULL) {
		return NULL; //returns NULL if list is empty
  }
  else {
    car_t *first = front;
    front = front->next;
    return first;
  }
}

/* apply a function to every car in the list */
void lapply(void (*fn)(car_t *cp)) {
  car_t *c;
	
  for (c=front; c!=NULL; c=c->next) {
    (*fn)(c);
  }
}

/* remove(): find, remove, and return any car with
 * the designated plate; return NULL if not present
 */
car_t *lremove(char *platep){
  if ( front == NULL){
		return NULL;
	}	
	car_t *p = front;
	if (strcmp(p->plate, platep) == 0) {
		front = p->next;
		return p;
	}
	car_t *f = p;
	p = p->next;
	while (p != NULL){
		if (strcmp(p->plate, platep) == 0) {
				f->next = p->next;
				return p;
		}
		f = p;
		p = p->next;
	}
	return NULL;
}


/*
Old remove function
*/

/*
car_t *lremove(char *platep) {
  car_t *c;
  car_t *r = NULL;
  if (!strcmp(front->plate, platep)) {
    r = lget();
  }
  else {
    for (c=front; c != NULL; c=c->next) {
      if (!strcmp(c->next->plate, platep)) {
	c->next = c->next->next;
	r = c->next;
      }
    }
  }
  return r;
}
*/

