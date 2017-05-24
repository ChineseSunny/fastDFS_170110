
#include "upload_file.h"


int upload_file_by_name(char *filename)
{
	
	int ret = 0;
	
	time_t now ;
	char create_time[TIME_LEN] = {0};
	int get_num = 0;
	
	RVALUES values;
	
	//获取file_id
	char file_id[512] = { 0 };
	
	ret = myupload_byexec(filename,file_id);
	
	if(ret)
	{
		printf("fdfs_upload_by_filename err\n");
		ret = -1;
		return ret;
	}
	
	LOG(ROP_MODULE, ROP_PROC, "file_id:%s\n",file_id);
	
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
		ret = -1;
		goto END;
	}
	
	//ret = rop_hset_sting(redisContext *conn,char* key,char *field,char* value)
	
	//file_id 入 hash 表，hasn  key：FILEID_NAME_HASH
	ret = rop_hset_sting(conn,FILEID_NAME_HASH,file_id,filename);
		
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_NAME_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	//user
	ret = rop_hset_sting(conn,FILEID_USER_HASH,file_id,"sunny");
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_USER_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	//创建时间
	now = time(NULL);
	strftime(create_time, TIME_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));
	
	ret = rop_hset_sting(conn,FILEID_TIME_HASH,file_id,create_time);
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TIME_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	//点击量
	ret = rop_hset_sting(conn,FILEID_PV_HASH,file_id,"1");
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_PV_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
	//
	char suffix[30] = { 0 };
	ret =get_file_suffix((const char *)filename,suffix);
	
	//int rop_hset_sting(redisContext *conn, char *field,char* key, char* value);
	
	ret = rop_hset_sting(conn,FILEID_TYPE_HASH,file_id,suffix);
	LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "FILEID_TYPE_HASH:%sn",FILEID_TYPE_HASH,suffix);
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TYPE_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	
/*#define FILEID_URL_HASH     "FILEID_URL_HASH"
#define FILEID_SHARED_HASH     "FILEID_SHARED_HASH"*/

		//拼接file_url
	char file_url[256] = { 0 };
	strcat(file_url,"http://");
	strcat(file_url,"192.168.202.136");
	strcat(file_url,"/");
	strcat(file_url,file_id);

	ret = rop_hset_sting(conn,FILEID_URL_HASH,file_id,file_url);
	LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "FILEID_URL_HASH:%s\n",FILEID_URL_HASH,file_url);
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TYPE_HASH,file_id, file_url,conn->errstr);
		ret = -1;
		goto END;
	}
	

	
	ret = rop_hset_sting(conn,FILEID_SHARED_HASH,file_id,"0");
	LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "FILEID_SHARED_HASH:%sn",FILEID_SHARED_HASH,"0");
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TYPE_HASH,file_id, "0",conn->errstr);
		ret = -1;
		goto END;
	}
	
	
END:
	return ret;	
}
