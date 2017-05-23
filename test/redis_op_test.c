#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "redis_op.h"
#include "make_log.h"

#define ROP_MODULE "test"
#define ROP_PROC "redis_op"

int main()
{
	int ret = 0;
	
	//创建连接句柄
	redisContext* conn;
	char value[1024] = {0};
	
	//连接redis tcp数据库
	conn = rop_connectdb_nopwd("127.0.0.1","6379");
	
	if(NULL == conn)
	{
		 LOG(ROP_MODULE, ROP_PROC, "conn db error");
		 ret = -1;
		 goto END;
	}
	
	/*ret= rop_list_push(conn,key,"sunny");
	
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",key);
		goto END;
	}
	*/
	
	
	//set封装
	ret = rop_set_string(conn,"sun","1");
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"rop_set_string err");
		goto END;
	}
	printf("set sun suc\n");
	
	//get封装
	ret = rop_get_string(conn,"sun",value);
	
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"rop_get_string  %s err",value);
		goto END;
	}
	printf("get value suc:%s\n",value);
	
	memset(value,0,sizeof(value));
	
	//hset封装
	ret = rop_hset_sting(conn,"mfield","mkey","mval");
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"rop_hset_string err");
		goto END;
	}
	printf("hset mfield suc\n");
	
	
	//hget封装
	ret = rop_hget_string(conn,"mfield","mkey",value);
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"rop_hset_string err");
		goto END;
	}
	printf("hget value suc:%s\n",value);
	
		if(conn !=NULL)
		{
			//断开链接
    	redisFree(conn);
		}
		
END:
	//垃圾回收
	return ret;
}