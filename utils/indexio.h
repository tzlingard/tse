#pragma once

#include <stdio.h>
#include <unistd.h>

#include "hash.h"
#include "queue.h"
#include "webpage.h"

/* Load an index from a file */
hashtable_t* indexload(char* dirname, char* indexnm);

/* Save an index to a file*/
void indexsave(hashtable_t* htp, char* dirname, char* indexnm);