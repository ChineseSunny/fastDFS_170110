
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "fdfs_api.h"

#include "fdfs_client.h"
#include "logger.h"

#include "redis_op.h"
#include "make_log.h"

#define ROP_MODULE "test"
#define ROP_PROC "redis_op"

int main()
{
	int ret = 0;
	
	//获取file_id
	char file_id[512] = { 0 };
	ret = fdfs_upload_by_filename("a.c",file_id);
	if(ret)
	{
		printf("fdfs_upload_by_filename err\n");
		return ret;
	}
	printf("file_id:%s\n",file_id);
	
	
	//创建 redis 连接句柄
	redisContext* conn;
	char value[1024] = {0};
	
	char *filekey = "FILE_INFO_LIST";
	//连接redis tcp数据库
	conn = rop_connectdb_nopwd("127.0.0.1","6379");
	
	if(NULL == conn)
	{
		 LOG(ROP_MODULE, ROP_PROC, "conn db error");
		 ret = -1;
		 goto END;
	}
	
	//将file_id 入 redis 数据库
	ret= rop_list_push(conn,filekey,"file_id");
	
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",filekey);
		goto END;
	}
	
	
END:
	return ret;	
}

