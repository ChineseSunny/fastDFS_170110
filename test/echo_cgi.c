#include <stdlib.h>
#include <unistd.h>

#include "fcgi_stdio.h"
#include "fcgi_config.h"

extern char **environ;


static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}

int main (int argc, char *argv[])
{
    char **initialEnv = environ;//将该进程本身原有的环境变量 做一个备份
    int count = 0;

    while (FCGI_Accept() >= 0) {

        //当前请求的post数据的长度
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI echo</title>"
                "<h1>FastCGI echo</h1>\n"
                "Request number %d,  Process ID: %d<p>\n", ++count, getpid());

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

            printf("Standard input:<br>\n<pre>\n");
            for (i = 0; i < len; i++) {
                //读一个字节
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //回显一个字节
                putchar(ch);
            }
            printf("\n</pre><p>\n");
        }

        PrintEnv("Request environment", environ);
        PrintEnv("Initial environment", initialEnv);
    } /* while */

    return 0;
}
