#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cJSON.h"

#include "util_cgi.h"
#include "make_log.h"
#include "select_redis_file.h"
#include "redis_op.h"

//头文件里有重定向操作，放在hiredis之后
#include "fcgi_stdio.h"
#include "fcgi_config.h"

#define FCGI        "fcgi"
#define FCGI_REG		"reg"

#define URI_CMD_LEN (128)

int main ()
{
	int ret = 0;

	char *out_p = NULL;

	while (FCGI_Accept() >= 0) {
		/*char cmd[URI_CMD_LEN] = {0};
		  char fromId[URI_CMD_LEN] = {0};*/

		printf("Content-type: text/html\r\n"
				"\r\n");	

		char *query_string = getenv("QUERY_STRING");
		char user[128];
		char flower_name[128];
		char pwd[128];
		char tel[128];
		char email[128];
		char *out;
		char buffer[256] = {0};
		char user_id[10] = {0};

		LOG(FCGI, FCGI_REG, "query_string = [%s]\n", getenv("QUERY_STRING"));

		//入redis库 配置文件 ip 127.0.0.1
		redisContext *redis_conn = rop_connectdb_nopwd("127.0.0.1","6379");

		if (redis_conn == NULL) 
		{
			return -1;
		}

		LOG(FCGI, FCGI_REG, "rop_connectdb_nopwd suc\n");

		cJSON *root = cJSON_CreateObject(); 
		cJSON_AddStringToObject(root, "code", "000");
		out = cJSON_Print(root);

		LOG(FCGI, FCGI_REG, "out = [%s]", out);

		printf(out);
		LOG(FCGI, FCGI_REG,"printf suc\n");

		free(out);
		LOG(FCGI, FCGI_REG, "free suc\n");    

	} 

	/* while */

	return 0;
}
