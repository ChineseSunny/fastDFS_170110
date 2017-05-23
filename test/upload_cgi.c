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

        //��ǰ�����post���ݵĳ���
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n");

        if (contentLength != NULL) {
            //��post����
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        }
        else {
            //�ڴ���post����
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
                //��һ���ֽ�
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //����һ���ֽ�
                //putchar(ch);
                *p = ch;
                p++;
            }
            printf("\n</pre><p>\n");

            //��ʱbuf��Ӧ�ô��ȫ����post����
            FILE *fp = NULL;
            fp = fopen("post_data.txt", "w");

            fwrite(buf, 1, len,fp);
            fclose(fp);
        }

    } /* while */

    return 0;
}