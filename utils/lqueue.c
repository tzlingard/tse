#include "lqueue.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "queue.h"

pthread_mutex_t m;

lqueue_t* lqopen() {
  pthread_mutex_init(&m, NULL);
  pthread_mutex_lock(&m);
  queue_t* q = qopen();
  pthread_mutex_unlock(&m);
  return ((lqueue_t*)q);
}

void lqclose(lqueue_t* qp) {
  pthread_mutex_lock(&m);
  qclose((queue_t*)qp);
  pthread_mutex_unlock(&m);
  pthread_mutex_destroy(&m);
}

int32_t lqput(lqueue_t* qp, void* elementp) {
  pthread_mutex_lock(&m);
  int32_t v = qput((queue_t*)qp, elementp);
  pthread_mutex_unlock(&m);
  return (v);
}

void* lqget(lqueue_t* qp) {
  pthread_mutex_lock(&m);
  void* elementp = qget(qp);
  pthread_mutex_unlock(&m);
  return (elementp);
}

void lqapply(lqueue_t* qp, void (*fn)(void* elementp)) {
  pthread_mutex_lock(&m);
  qapply(qp, fn);
  pthread_mutex_unlock(&m);
}

void* lqsearch(lqueue_t* qp, bool (*searchfn)(void* elementp, const void* keyp),
               const void* skeyp) {
  pthread_mutex_lock(&m);
  void* returnp = qsearch(qp, searchfn, skeyp);
  pthread_mutex_unlock(&m);
  return (returnp);
}

void* lqremove(lqueue_t* qp, bool (*searchfn)(void* elementp, const void* keyp),
               const void* skeyp) {
  pthread_mutex_lock(&m);
  void* v = qremove(qp, searchfn, skeyp);
  pthread_mutex_unlock(&m);
  return (v);
}