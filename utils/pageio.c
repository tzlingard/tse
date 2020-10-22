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
#include <string.h>
#include <unistd.h>

#include "../utils/hash.h"
#include "../utils/queue.h"
#include "../utils/webpage.h"

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
  if (access(file, R_OK)) {          /*if the file is readable*/
    FILE *fp;
    char url[80];
    char html[80];
    int depth;
    int htmlLen;
    fp = fopen(file, "r"); /*set up reading of file*/
    fscanf(fp, "%s", url);
    fscanf(fp, "%d", &depth);
    fscanf(fp, "%d", &htmlLen);
    /*get one character at a time, move pointer to next character*/
    int ch;
    do { /*continue to read file until the end*/
      ch = fgetc(fp);
      sprintf(html, "%c", (char)ch); /*pass one char at a time to the html*/
    } while (ch != EOF);
    webpage_t *newpage = webpage_new(url, depth, html);
    /*create the new webpage with the file info*/
    return newpage; /*return non-Null for success*/
  }
  return NULL; /*return Null for failure*/
}
