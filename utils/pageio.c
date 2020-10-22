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
#include <stdio.h>                                                                                                     
#include "../utils/webpage.h"                                                                                           
#include "../utils/queue.h"                                                                                            
#include "../utils/hash.h"                                                                                              
#include <string.h>                                                                                                     
#include <stdbool.h>                                                                                             
#include <stdint.h>                                                                                                    
#include <unistd.h>

int32_t pagesave(webpage_t* pagep, int id, char* dirname) {                             
  char dest[80];                                                                        
  sprintf(dest, "%s/%d", dirname, id);                                                  
  if (access(dest, F_OK)) { //if the file doesn't exist yet                             
    FILE* fp;                                                                           
    fp = fopen(dest, "w");                                                              
    fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep), webpage_getDepth(pagep), webpa\
ge_getHTMLlen(pagep), webpage_getHTML(pagep));                                          
    fclose(fp);                                                                         
    printf("Saved: %s\n",webpage_getURL(pagep));                                        
    return 0;                                                                           
  }                                                                                     
  else {                                                                                
    return -1;                                                                          
  }                                                                                     
}

/*                                                                                                                      
 * pageload -- loads the numbered filename <id> in direcory <dirnm>                                                  
 * into a new webpage
 *                                                                                                            
 * returns: non-NULL for success; NULL otherwise                                                               
 */                                                                                                                     
webpage_t *pageload(int id, char *dirnm){
	char *file; /*set up file pointer*/                                                                                                        
	int depth=0;/* our new page will have a depth of 0*/
	sprintf(file, "%s/%d", dirnm, id);/*pass the file from the directory*/ 

	if (access(dest, R_OK)) { /*if the file is readable*/
	 FILE *fp;
	 FILE *html;
	 fp=fopen(file,"r");/*set up reading of file*/
	 int ch = fgetc(fp); /*get one character at a time, move pointer to next character*/

	 while (ch != EOF){/*continue to read file until the end*/
		 html= fopen(ch,"a");/*pass one char at a time to the html*/
	 }
	 
	 webpage_t newpage=webpage_new(dest,depth,html); /*create the new webpage with the file info*/
	 return 0; /*return non-Null for success*/
 }
	return NULL; /*return Null for failure*/
}
