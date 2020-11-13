#pragma once
/*
 * hash.h -- A generic hash table implementation, allowing arbitrary
 * key structures.
 *
 */
#include <stdbool.h>
#include <stdint.h>

typedef void lhashtable_t; /* representation of a locked hashtable hidden */

/* lhopen -- opens a locked hash table with initial size hsize */
lhashtable_t *lhopen(uint32_t hsize);

/* lhclose -- closes a locked hash table */
void lhclose(lhashtable_t *htp);

/* lhput -- puts an entry into a locked hash table under designated key
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen);

/* lhapply -- applies a function to every entry in a locked hash table */
void lhapply(lhashtable_t *htp, void (*fn)(void *ep));

/* lhsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *lhsearch(lhashtable_t *htp,
               bool (*searchfn)(void *elementp, const void *searchkeyp),
               const char *key, int32_t keylen);

/* lhremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *lhremove(lhashtable_t *htp,
               bool (*searchfn)(void *elementp, const void *searchkeyp),
               const char *key, int32_t keylen);