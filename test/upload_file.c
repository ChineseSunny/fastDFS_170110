
#include "upload_file.h"


int upload_file_by_name(char *filename)
{
	
	int ret = 0;
	
	time_t now ;
	char create_time[TIME_LEN] = {0};
	int get_num = 0;
	
	RVALUES values;
	
	//��ȡfile_id
	char file_id[512] = { 0 };
	
	ret = myupload_byexec(filename,file_id);
	
	if(ret)
	{
		printf("fdfs_upload_by_filename err\n");
		ret = -1;
		return ret;
	}
	
	LOG(ROP_MODULE, ROP_PROC, "file_id:%s\n",file_id);
	
	//���� redis ���Ӿ��
	redisContext* conn;
	char value[1024] = {0};
	
	char *filekey = "FILE_INFO_LIST";
	
	//����redis tcp���ݿ�
	conn = rop_connectdb_nopwd("127.0.0.1","6379");
	
	if(NULL == conn)
	{
		 LOG(ROP_MODULE, ROP_PROC, "conn db error");
		 ret = -1;
		 goto END;
	}
	
	//��file_id �� redis���ݿ�����
	ret= rop_list_push(conn,filekey,file_id);
	
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",filekey);
		ret = -1;
		goto END;
	}
	
	//ret = rop_hset_sting(redisContext *conn,char* key,char *field,char* value)
	
	//file_id �� hash ��hasn  key��FILEID_NAME_HASH
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
	
	//����ʱ��
	now = time(NULL);
	strftime(create_time, TIME_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));
	
	ret = rop_hset_sting(conn,FILEID_TIME_HASH,file_id,create_time);
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TIME_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	//�����
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
	
	if(ret)
	{
		LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-][GMS_REDIS]hset:key:%s , fileid:%s,value: %s Error:%sn",FILEID_TYPE_HASH,file_id, value,conn->errstr);
		ret = -1;
		goto END;
	}
	

	
END:
	return ret;	
}

	/*rop_range_list �õ������е�����
	  @param values    �õ����е�value����
    @param get_num   �õ����value�ĸ���
    
	ret = rop_range_list(conn,filekey, 0, -1,values ,&get_num);
	if(ret)
	{
		LOG(ROP_MODULE,	ROP_PROC,"lpush %s err",filekey);
		goto END;
	}
	printf("********filekey:%s\n",filekey);
	printf("********values:%s\n",values);*/