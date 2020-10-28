#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "hash.h"
//#include "pageio.h"
//#include "webpage.h"
#include "indexio.h"


typedef struct {  // structure to hold each word of a webpage and its frequency
  char* word;
  queue_t* freq;
  // queue_t documents;
  // TODO: change freq to a queue filled with struct: (documents and
  // frequencies)
} word_t;

typedef struct {
  int id;
  int freq;
} docs_t;



int main(void) {
	hashtable_t* htp = hopen(20);

	//add first word "Word 1" with doc freqs 3 0 and 1
	word_t* w1 = malloc(sizeof(word_t*));
	w1->word = (char*)malloc(sizeof(char)*6);
	strcpy(w1->word, "Word1");
	w1->freq = qopen();
	docs_t* d11 = malloc(sizeof(docs_t*));
	d11->id = 1;
	d11->freq = 3;
	docs_t* d12 = malloc(sizeof(docs_t*));
	d12->id = 2;
	d12->freq = 0;
	docs_t* d13 = malloc(sizeof(docs_t*));
	d13->id = 3;
	d13->freq = 1;
	qput(w1->freq, d11);
	qput(w1->freq, d12);
	qput(w1->freq, d13);
	hput(htp, w1, w1->word, strlen(w1->word));

	//add second word "Word 2" with doc freqs 0 5 and 2
	word_t* w2 = malloc(sizeof(word_t*));
	w2->word = (char*)malloc(sizeof(char)*6);
	strcpy(w2->word, "Word2");
	w2->freq = qopen();
	docs_t* d21 = malloc(sizeof(docs_t*));
	d21->id = 1;
	d21->freq = 0;
	docs_t* d22 = malloc(sizeof(docs_t*));
	d22->id = 2;
	d22->freq = 5;
	docs_t* d23 = malloc(sizeof(docs_t*));
	d23->id = 3;
	d23->freq = 2;
	qput(w2->freq, d21);
	qput(w2->freq, d22);
	qput(w2->freq, d23);
	hput(htp, w2, w2->word, strlen(w2->word));

	//save the index to the file test_index
	indexsave(htp, ".", "test_index");
	  
	qclose(w1->freq);
	free(w1->word);
	qclose(w2->freq);
	free(w2->word);
	hclose(htp);
	
	/*
	char* originalURL = webpage_getURL(originalPage);
	int originalDepth = webpage_getDepth(originalPage);
	char* originalHTML = webpage_getHTML(originalPage);
	int originalHTMLlen = webpage_getHTMLlen(originalPage);
	char* savedURL = webpage_getURL(savedPage);
	int savedDepth = webpage_getDepth(savedPage);
	char* savedHTML = webpage_getHTML(savedPage);
	int savedHTMLlen = webpage_getHTMLlen(savedPage);
	printf("Difference in URLs: %d\n", strcmp(originalURL, savedURL));
	printf("Difference in Depths: %d\n", (originalDepth-savedDepth));
	printf("Difference in HTMLs: %d\n", strcmp(originalHTML, savedHTML));
	printf("Difference in HTML Lenghts: %d\n", (originalHTMLlen-savedHTMLlen));
	*/




  return 0;
}
