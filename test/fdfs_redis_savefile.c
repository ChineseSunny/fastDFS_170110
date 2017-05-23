
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "fdfs_api.h"

#include "redis_op.h"
#include "make_log.h"

#define ROP_MODULE "test"
#define ROP_PROC "redis_op"

#define FILE_USER_LIST      "FILE_USER_LIST"
#define FILEID_NAME_HASH    "FILEID_NAME_HASH"
#define FILEID_USER_HASH    "FILEID_USER_HASH"
#define FILEID_TIME_HASH    "FILEID_TIME_HASH"
#define FILEID_PV_HASH      "FILEID_PV_HASH"

#define TIME_LEN (128)

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("use:a.out filename");
	}
	
	int ret = 0;
	
	time_t now ;
	char create_time[TIME_LEN] = {0};
	int get_num = 0;
	
	RVALUES values = 1;
	
	//获取file_id
	char file_id[512] = { 0 };
	
	//ret = fdfs_upload_by_filename("a.c",file_id);
	
	ret = myupload_byexec(argv[1],file_id);
	
	if(ret)
	{
		printf("fdfs_upload_by_filename err\n");
		return ret;
	}
	
	printf("**=====file_id:%s\n",file_id);
	
	
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
	
	//将file_id 入 redis数据库链表
	ret= rop_list_push(conn,filekey,file_id);
	
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",filekey);
		goto END;
	}
	
	//ret = rop_hset_sting(redisContext *conn,char* key,char *field,char* value)
	
	//file_id 入 hash 表，hasn  key：FILEID_NAME_HASH
	ret = rop_hset_sting(conn,FILEID_NAME_HASH,file_id,argv[1]);
		
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_NAME_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	ret = rop_hset_sting(conn,FILEID_USER_HASH,file_id,"sunny");
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_USER_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	now = time(NULL);
	strftime(create_time, TIME_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));
	
	ret = rop_hset_sting(conn,FILEID_TIME_HASH,file_id,create_time);
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TIME_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	ret = rop_hset_sting(conn,FILEID_PV_HASH,file_id,"1");
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_PV_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	/*rop_range_list 得到链表中的数据
	  @param values    得到表中的value数据
    @param get_num   得到结果value的个数
    
	ret = rop_range_list(conn,filekey, 0, -1,values ,&get_num);
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",filekey);
		goto END;
	}
	printf("********filekey:%s\n",filekey);
	printf("********values:%s\n",values);*/
	
END:
	if (conn != NULL) {
        rop_disconnect(conn);
    }
	return ret;	
}

