#include "lhash.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "hash.h"

pthread_mutex_t m;

lhashtable_t* hopen(uint32_t hsize) {
  pthread_mutex_init(&m, NULL);
  pthread_mutex_lock(&m);
  lhashtable_t* h = (lhashtable_t*)hopen(hsize);
  pthread_mutex_unlock(&m);
  return (h);
}

void lhclose(lhashtable_t* htp) {
  pthread_mutex_lock(&m);
  hclose((hashtable_t*)htp);
  pthread_mutex_unlock(&m);
  pthread_mutex_destroy(&m);
}

int32_t lhput(lhashtable_t* htp, void* ep, const char* key, int keylen) {
  pthread_mutex_lock(&m);
  int32_t v = hput((hashtable_t*)htp, ep, key, keylen);
  pthread_mutex_unlock(&m);
  return (v);
}

void lhapply(lhashtable_t* htp, void (*fn)(void* ep)) {
  pthread_mutex_lock(&m);
  happly((hashtable_t*)htp, fn);
  pthread_mutex_unlock(&m);
}

void* lhsearch(lhashtable_t* htp,
               bool (*searchfn)(void* elementp, const void* searchkeyp),
               const char* key, int32_t keylen) {
  pthread_mutex_lock(&m);
  void* returnp = hsearch((hashtable_t*)htp, searchfn, key, keylen);
  pthread_mutex_unlock(&m);
  return (returnp);
}

void* lhremove(lhashtable_t* htp,
               bool (*searchfn)(void* elementp, const void* searchkeyp),
               const char* key, int32_t keylen) {
  pthread_mutex_lock(&m);
  void* v = hremove((hashtable_t*)htp, searchfn, key, keylen);
  pthread_mutex_unlock(&m);
  return (v);
}