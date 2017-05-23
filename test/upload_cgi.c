#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fcgi_stdio.h"
#include "fcgi_config.h"
#include "make_log.h"

extern char **environ;


#define MODULE "cgi"
#define PROC "upload"

int main (int argc, char *argv[])
{

    while (FCGI_Accept() >= 0) {

        //当前请求的post数据的长度
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n");

        if (contentLength != NULL) {
            //有post数据
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        }
        else {
            //在处理post数据
            int i, ch;
            char *buf = NULL;
            char *p ;

            buf = malloc(len);
            if (buf == NULL) {
                LOG(MODULE, PROC, "malloc buf error");
                break;
            }
            memset(buf, 0, len);
            p = buf;

            printf("Standard input:<br>\n<pre>\n");
            for (i = 0; i < len; i++) {
                //读一个字节
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //回显一个字节
                //putchar(ch);
                *p = ch;
                p++;
            }
            printf("\n</pre><p>\n");

            //此时buf就应该存放全部的post数据
            FILE *fp = NULL;
            fp = fopen("post_data.txt", "w");

            fwrite(buf, 1, len,fp);
            fclose(fp);
        }

    } /* while */

    return 0;
}