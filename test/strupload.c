#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fcgi_stdio.h"
#include "fcgi_config.h"
#include "make_log.h"

extern char **environ;


#define MODULE "cgi"
#define PROC "upload"


char* memstr(char* full_data, int full_data_len, char* substr)  
{  
	int i = 0;
 	if (full_data == NULL || full_data_len <= 0 || substr == NULL) {  
        return NULL;  
    }  
  
    if (*substr == '\0') {  
        return NULL;  
    }  
  
    int sublen = strlen(substr);  
  
     
    char* cur = full_data;  
    int last_possible = full_data_len - sublen + 1;  
    for (i = 0; i < last_possible; i++) {  
        if (*cur == *substr) {  
            //assert(full_data_len - i >= sublen);  
            if (memcmp(cur, substr, sublen) == 0) {  
                //found  
                return cur;  
            }  
        }  
        cur++;  
    }  
  
    return NULL;  
}  

int main()
{
	int ret = 0;
	char line[128] = { 0 };
	char filename[128] = { 0 };
	
	
	FILE *fp = fopen("post_data.txt", "r");
	
	FILE *p1 = strstr(fp,"\r\n");
	FILE *p2 = p1;
	
	
	
	
	return ret;
}