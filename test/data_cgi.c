#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "util_cgi.h"
#include "make_log.h"

#include "fcgi_stdio.h"
#include "fcgi_config.h"

#define FCGI        "fcgi"
#define FCGI_DATA   "data"

#define URI_CMD_LEN (128)

int main ()
{

    while (FCGI_Accept() >= 0) {
        char cmd[URI_CMD_LEN] = {0};
        char fromId[URI_CMD_LEN] = {0};
        char count[URI_CMD_LEN] = {0};
//        char user[URI_CMD_LEN] = {0};

        printf("Content-type: text/html\r\n"
                "\r\n");

        // cmd=newFile&fromId=0&count=8&user=
        char *query_string = getenv("QUERY_STRING");
        LOG(FCGI, FCGI_DATA, "query_string = [%s]", getenv("QUERY_STRING"));
				
				//int query_parse_key_value(const char *query, const char *key, char *value, int *value_len_p);

        query_parse_key_value(query_string, "cmd", cmd, NULL);
		
        if (strcmp(cmd, "newFile") == 0) {
            //页面展示查询的一个业务
            query_parse_key_value(query_string, "fromId", fromId, NULL);
            query_parse_key_value(query_string, "count", count, NULL);
            //user
            //query_parse_key_value(query_string, "user", user, NULL);

            //根据fromid 和count 查询FILE_USER_LIST

            //再根据得到的每个fileid  获取每个文件的属性

            //根据每个属性 封装成一个json 字符串
            //将json字符串打印给前端

            //test
            char *json_str = malloc(4096);

            FILE*fp = fopen("json_test_data.json", "r");
            fread(json_str, 4096,1, fp);

            fclose(fp);


            //将jsonstr 打印给前端
            printf("%s", json_str);

        } 


        
        

    } /* while */

    return 0;
}
