#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "queue.h"

typedef struct node {
  struct node *next;
  void* element;
} node_t;

typedef struct queue_t {
  node_t* front;
  node_t* back;
} queueStruct_t;

static node_t *make_node(node_t *next, void *element) {
  node_t *new;
  if (!(new = (node_t*)malloc(sizeof(node_t)))) {
    printf("Malloc failure\n");
    return NULL;
  }
  new->next = next;
  new->element = element;
  return new;
}

/*create a queue function*/
queue_t* qopen(void){
  /*define a pointer for this queue*/
  queueStruct_t* head;
  /*if there is a problem with allocating memory*/
  if(!(head = (queueStruct_t*)malloc(sizeof(queueStruct_t)))){
    /*throw an error*/
    printf("[Error: malloc failed allocating person]\n");
    return NULL;
  }
  return (queue_t*)head;
}


/* deallocate a queue, frees everything in it */
void qclose(queue_t *qp){
	if (qp == NULL){
		return;
	}
	node_t* f = ((queueStruct_t*)qp)->front;
	node_t* n = NULL;
	while(f!=NULL){
		n = f->next;
		free(f);
		f=n;
	}
	free(qp);
}


/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t qput(queue_t *qp, void *elementp) {
	if (qp == NULL || elementp == NULL){
		return 1;
	}
  //make node type
  node_t *qn = make_node(NULL, elementp);
  if (qn == NULL) {
    return 1; //return 1 if malloc fails
  }
  //case 1: empty queue
  if (((queueStruct_t*)qp)->front == NULL){
    ((queueStruct_t*)qp)->front = qn;
    ((queueStruct_t*)qp)->back = qn;
    return 0;
  }
  //case 2: nonempty queue
  else {
    ((queueStruct_t*)qp)->back->next = qn;
    ((queueStruct_t*)qp)->back = qn;
    return 0;
  }
  return 1;
}

/* get the first element from queue, removing it from the queue */
void* qget(queue_t *qp) {
	if (qp == NULL){
		return NULL;
	}
  //case 1: empty queue
  if (((queueStruct_t*)qp)->front == NULL) {
    return NULL;
  }
  //case 2: nonempty queue
  else {
    node_t* fp =((queueStruct_t*)qp)->front;
    ((queueStruct_t*)qp)->front = ((queueStruct_t*)qp)->front->next;
		void* elementp = (void*)(fp->element);
		free(fp);
		return elementp;
  }
}

/*apply a function to all the elements of the queue*/
void qapply(queue_t *qp, void (*fn)(void* elementp)){
	if (qp == NULL || fn == NULL){
		return;
	}
	node_t* c;
  for(c=((queueStruct_t*)qp)->front; c!=NULL; c=c->next){
    (*fn)((queueStruct_t*)(c->element));
  }
}


/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp){
 	if ((qp == NULL || searchfn == NULL) || skeyp == NULL) {
		return NULL;
	}
	node_t* n;
	for (n=((queueStruct_t*)qp)->front; n!=NULL; n=n->next) {
		if (searchfn(n->element, skeyp)) {
				return n->element;
			}
	}
	return NULL;
}


/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* qremove(queue_t *qp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp) {
	if ((qp == NULL || searchfn == NULL) || skeyp == NULL) {
		return NULL;
	}
	node_t* c =  ((queueStruct_t*)qp)->front;
	if (c==NULL){
		return NULL;
	}
	//case 1: element found at start of queue
	if ((*searchfn)(c->element, skeyp)) { //if the boolean is satisfied with the given key

		void* elementp = c->element;
		((queueStruct_t*)qp)->front = ((queueStruct_t*)qp)->front->next;
		free(c);
		return elementp;
	}
	//case 2: element found in middle/end of queue
	node_t *f = c; //following pointer 
  for(c=((queueStruct_t*)qp)->front->next; c != NULL; c=c->next) { //iterate through the queue //TODO: qp is a queue, not a queueStruct: no front?
    if ((*searchfn)(c->element, skeyp)) { //if the boolean is satisfied with the given key

			void* elementp = c->element;
			f->next = c->next;
			if(((queueStruct_t*)qp)->back == c) {
				((queueStruct_t*)qp)->back = f;
			}
			free(c);
			return elementp;
		}
		f=c;
  }
  return NULL;
}


/* concatenatenates elements of q2 into q1
 * q2 is dealocated, closed, and unusable upon completion 
 */

void qconcat (queue_t *q1p, queue_t *q2p) {
  if ((queueStruct_t*)q1p != NULL && (queueStruct_t*)q2p != NULL) {
    while (((queueStruct_t*)q2p)->front != NULL) {
      qput((queueStruct_t*)q1p, (node_t*)qget((queueStruct_t*)q2p)); //removes first element from q2p and puts it at the end of q1p, fails if put is non-zero
		}
		qclose(q2p);
  }
}

/*
 *void qconcat (queue_t *q1p, queue_t *q2p) {
 * if ((queueStruct_t*)q1p != NULL && (queueStruct_t*)q2p != NULL) {
 *   while ((queueStruct_t*)q2p != NULL) {
 *     if(qput((queueStruct_t*)q1p, (node_t*)qget((queueStruct_t*)q2p)) != 0) {//removes first element from q2p and puts it at the end of q1p, fails if put *is non-zero
 *	printf("Concatenation failed.\n");
 *	return;
 *      }
 *   }
 * }
 *}
 */
