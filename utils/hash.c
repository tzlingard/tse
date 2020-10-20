/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "queue.h"
#include <stdint.h>                                                         
#include <stdbool.h>

#define get16bits(d) (*((const uint16_t *) (d)))

/*typedef void hashtable_t;  representation of a hashtable hidden */ 
typedef struct hashtable_t {
  int size;
  queue_t** table;
} hashStruct_t;

/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */

static uint32_t SuperFastHash (const char *data, int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

hashtable_t *hopen(uint32_t size) {
  hashStruct_t* ht;
  if(!(ht = (hashStruct_t*)malloc(sizeof(hashStruct_t)))){
    /*throw an error*/
    printf("[Error: malloc failed allocating hash table]\n");
    return NULL;
  }
  ht->table = (queue_t**)malloc(size*sizeof(queue_t*));
  uint32_t i;
  for (i=0;i<size;i++) {
    ht->table[i] = qopen();
  }
  ht->size = size;
  return (hashtable_t*)ht;
}
                                                                                                   
/* hclose -- closes a hash table */                                               
void hclose(hashtable_t *htp) {
	if(htp==NULL){
		return;
	}
  int i;
  for(i=0;i<(*(hashStruct_t*)htp).size;i++) {
		qclose(((hashStruct_t*)htp)->table[i]);
		//		free(((hashStruct_t*)htp)->table[i]);
  }
	free(((hashStruct_t*)htp)->table);
  free(htp);
}                                                  
                                                                                  
/* hput -- puts an entry into a hash table under designated key                   
 * returns 0 for success; non-zero otherwise                                      
 */                                                                               
int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen) {
	if((hashStruct_t*)htp == NULL || ep == NULL){
		return 1;
	}
	if(key == NULL){
		return 1;
	}
	if(((hashStruct_t*)htp)->table == NULL){
		return 1;
	}
  uint32_t loc = SuperFastHash(key, keylen, ((hashStruct_t*)htp)->size);
	queue_t* q = (queue_t*)(((hashStruct_t*)htp)->table[loc]); 
	return qput(q, ep);
}       
                                                                                  
/* happly -- applies a function to every entry in hash table */                   
void happly(hashtable_t *htp, void (*fn)(void* ep)) {
	if(htp==NULL || fn==NULL){
		return;
	}
	int i;
  for(i=0;i<(*(hashStruct_t*)htp).size;i++) {
    qapply((((hashStruct_t*)htp)->table[i]), fn);
  }
}                             
                                                                                  
/* hsearch -- searchs for an entry under a designated key using a                 
 * designated search fn -- returns a pointer to the entry or NULL if              
 * not found                                                                      
 */                                                                               
void *hsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen) {
 	if( (htp==NULL || searchfn==NULL)||key==NULL) {
		return NULL;
	}
	int i;
  void *e;
  for(i=0;i<(*(hashStruct_t*)htp).size;i++) {
    e = qsearch((((hashStruct_t*)htp)->table[i]), searchfn, key);
    if (e != NULL) return e;
  }
  return NULL;
}                                                         
                                                                                  
/* hremove -- removes and returns an entry under a designated key                 
 * using a designated search fn -- returns a pointer to the entry or              
 * NULL if not found                                                              
 */
void *hremove(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen) {
	if( (htp==NULL || searchfn==NULL)||key==NULL) {
		return NULL;
	}
	for(int i=0;i<(*(hashStruct_t*)htp).size;i++) {
		if(qsearch((((hashStruct_t*)htp)->table[i]), searchfn, key) != NULL){
			return qremove((((hashStruct_t*)htp)->table[i]), searchfn, key);
		}
	}
	return NULL;
}
