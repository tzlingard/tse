/* pageio.c ---
 *
 *
 * Author: Ariel E. Attias
 * Created: Thu Oct 22 11:35:28 2020 (-0400)
 * Version:
 *
 * Description: module implementing pageio.h
 *
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hash.h"
#include "queue.h"
#include "webpage.h"

int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
  char dest[80];
  sprintf(dest, "%s/%d", dirname, id);
  if (access(dest, F_OK)) {  // if the file doesn't exist yet
    FILE *fp;
    fp = fopen(dest, "w");
    fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep),
            webpage_getDepth(pagep), webpage_getHTMLlen(pagep),
            webpage_getHTML(pagep));
    fclose(fp);
    printf("Saved: %s\n", webpage_getURL(pagep));
    return 0;
  } else {
    return -1;
  }
}

/*
 * pageload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */
webpage_t *pageload(int id, char *dirnm) {
  char file[80];                     /*location of page to load*/
  sprintf(file, "%s/%d", dirnm, id); /*pass the file from the directory*/
  if (access(file, R_OK) == 0) {     /*if the file is readable*/
    FILE *fp;
    char url[80];
    int depth;
    int htmlLen;
    fp = fopen(file, "r"); /*set up reading of file*/
    fscanf(fp, "%s", url);
    fscanf(fp, "%d", &depth);
    fscanf(fp, "%d", &htmlLen);
    char *html = malloc((sizeof(char) * htmlLen) + 1);
    /*get one character at a time, move pointer to next character*/
    char ch;
    int i;
    for (i = 0; i < htmlLen; i++) { /*continue to read file until the end*/
      ch = fgetc(fp);
      if (ch == EOF) break;
      sprintf(html + i, "%c", ch); /*pass one char at a time to the html*/
    }
    webpage_t *newpage = webpage_new(url, depth, html);
    fclose(fp);
    /*create the new webpage with the file info*/
    return newpage; /*return non-Null for success*/
  }
  return NULL; /*return Null for failure*/
}