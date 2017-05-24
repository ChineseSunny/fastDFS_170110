#include "upload_file_read.h"

int upload_file_read()
{
	
	int ret = 0;
	int count = 0;
	
	//创建 redis 连接句柄
	
	redisContext* conn = NULL;
	
	RVALUES file_id_arr = NULL;
	
	
	//连接redis tcp数据库
	conn = rop_connectdb_nopwd("127.0.0.1", "6379");
	
	if(NULL == conn)
	{
	
		LOG(ROP_MODULE, ROP_PROC, "conn db error");
		ret = -1;
		goto END;
	}
	
	//获取链表长度
	count = rop_list_len(conn,"FILE_INFO_LIST");
	
	if(count < 0)
	{
		 LOG(ROP_MODULE, ROP_PROC, "rop_list_len err");
		 ret = -1;
		 goto END;
	}
	
	file_id_arr = malloc(count * VALUES_ID_SIZE);
	
	//遍历链表
	ret = rop_range_list(conn, "FILE_INFO_LIST", 0,count, file_id_arr, &count);
	
	if(ret)
	{
		 LOG(ROP_MODULE, ROP_PROC, "rop_range_list err");
		 ret = -1;
		 goto END;
	}

	int i = 0;
		
	for(i =0;i< count;++i)
	{
		//printf("=========333======");
		
		char name[VALUES_ID_SIZE] = { 0 };
		char user[VALUES_ID_SIZE] = { 0 };
		char time[VALUES_ID_SIZE] = { 0 };
		char pv[VALUES_ID_SIZE] = { 0 };
		
		//printf("file_id : %s\n",file_id_arr[i]);
		LOG(ROP_MODULE, ROP_PROC, "file_id : %s\n",file_id_arr[i]);
		
		rop_hget_string(conn,FILEID_NAME_HASH, file_id_arr[i],name);
		//printf("name : %s\n",name);
		LOG(ROP_MODULE, ROP_PROC, "name : %s\n",name);
		
		rop_hget_string(conn,FILEID_USER_HASH, file_id_arr[i],user);
		//printf("user : %s\n",user);
		LOG(ROP_MODULE, ROP_PROC, "user : %s\n",user);
		
		rop_hget_string(conn,FILEID_TIME_HASH, file_id_arr[i],time);
		//printf("time : %s\n",time);
		LOG(ROP_MODULE, ROP_PROC,"time : %s\n",time);
		
		rop_hget_string(conn,FILEID_PV_HASH, file_id_arr[i],pv);
		//printf("pv : %s\n",pv);
		LOG(ROP_MODULE, ROP_PROC,"pv : %s\n",pv);

		
	}



END:

	if (conn != NULL) {
        rop_disconnect(conn);
    }

	return ret;
}