/* pageio.c --- 
 * 
 * 
 * Author: Ariel E. Attias
 * Created: Thu Oct 22 11:35:28 2020 (-0400)
 * Version: 
 * 
 * Description: 
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
