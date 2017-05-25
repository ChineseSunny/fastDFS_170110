
#include "select_redis_file.h"


int select_file_to_cjson(int fromID,int count,char **out_p)
{
	
	int ret = 0;
	int file_num = 0;
	
	char *out = NULL;
	//创建 redis 连接句柄
	
	redisContext* conn = NULL;
	
	RVALUES file_id_arr = NULL;
	
	//创建 cjson  root object
	cJSON * root = cJSON_CreateObject();
	
	//创建 cjson array
	cJSON * arr = cJSON_CreateArray();
	
	//连接redis tcp数据库
	conn = rop_connectdb_nopwd("127.0.0.1", "6379");
	LOG(ROP_MODULE, ROP_PROC, "rop_connectdb_nopwd 127.0.0.1 suc");
	
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
	LOG(ROP_MODULE, ROP_PROC, "rop_list_len %d\n",count);
	
	//开辟空间
	file_id_arr = malloc(count * VALUES_ID_SIZE);
	
	LOG(ROP_MODULE, ROP_PROC, "malloc suc ");
	
	ret = rop_range_list(conn, "FILE_INFO_LIST", 0,count, file_id_arr, &file_num);
	
	if(ret)
	{
		 LOG(ROP_MODULE, ROP_PROC, "rop_range_list err");
		 ret = -1;
		 goto END;
	}
	LOG(ROP_MODULE, ROP_PROC, "rop_range_list len :%d  suc\n",file_num);

	int i = 0;
		
	for(i =0;i< count;++i)
	{
		//创建 cjson 小 object
		cJSON * item = cJSON_CreateObject();
		
		char name[VALUES_ID_SIZE] = { 0 };
		char user[VALUES_ID_SIZE] = { 0 };
		char time[VALUES_ID_SIZE] = { 0 };
		char pv[VALUES_ID_SIZE] = { 0 };
		
		char type[VALUES_ID_SIZE] = { 0 };
		char url[VALUES_ID_SIZE] = { 0 };
		char shar_status[VALUES_ID_SIZE] = { 0 };
	
		//printf("file_id : %s\n",file_id_arr[i]);
		
		LOG("file_id", "file_id", "file_id ： %s  suc\n",file_id_arr[i]);
		
		//添加 id 和 value 到
		cJSON_AddStringToObject(item,"id",file_id_arr[i]);
		LOG(CJSON, CJSON, "id ：%s suc\n",file_id_arr[i]);
		
		rop_hget_string(conn,FILEID_NAME_HASH, file_id_arr[i],name);
		//printf("name : %s\n",name);
		cJSON_AddStringToObject(item,"title_m",name);
		LOG(CJSON, CJSON, "title_m ：%s suc\n",file_id_arr[i]);
		
		rop_hget_string(conn,FILEID_USER_HASH, file_id_arr[i],user);
		//printf("user : %s\n",user);
		cJSON_AddStringToObject(item,"title_s",user);
		LOG(CJSON, CJSON, "title_s :%s suc\n",file_id_arr[i]);
		
		rop_hget_string(conn,FILEID_TIME_HASH, file_id_arr[i],time);
		//printf("time : %s\n",time);
		cJSON_AddStringToObject(item,"descrip",time);
		LOG(CJSON, CJSON, "descrip ： %s suc\n",file_id_arr[i]);
		
		
		rop_hget_string(conn,FILEID_PV_HASH, file_id_arr[i],pv);
		//printf("pv : %s\n",pv);
		cJSON_AddNumberToObject(item,"pv",atoi(pv));
		LOG(CJSON, CJSON, "pv ：%s suc\n",file_id_arr[i]);
		
		 //kind (预留字段)
     cJSON_AddNumberToObject(item, "kind", 0);

     //picurl_m(图片logourl地址)
     char pic_url[VALUES_ID_SIZE] = {0};
		//TYPE
		rop_hget_string(conn,FILEID_TYPE_HASH, file_id_arr[i],type);
		strcat(pic_url, "http://");
    strcat(pic_url, "192.168.202.136");
    strcat(pic_url, "/static/file_png/");
    strcat(pic_url, type);
    strcat(pic_url, ".png");
		
		cJSON_AddStringToObject(item,"picurl_m",pic_url);
		LOG(CJSON, CJSON, "picurl_m ： %s suc\n",file_id_arr[i]);
		
		
		rop_hget_string(conn,FILEID_URL_HASH, file_id_arr[i],url);
		//printf("pv : %s suc\n",pv);
		cJSON_AddStringToObject(item,"url",url);
		LOG(CJSON, CJSON, "url ： %s suc\n",file_id_arr[i]);
		
		rop_hget_string(conn,FILEID_SHARED_HASH, file_id_arr[i],shar_status);
		//printf("pv : %s\n",pv);
		cJSON_AddNumberToObject(item,"hot",atoi(shar_status));
		LOG(CJSON, CJSON, "hot :%s suc\n",file_id_arr[i]);
		
		
		//cJSON_AddItemToArray(cJSON *array, cJSON *item);
		//添加object ---> array
		cJSON_AddItemToArray(arr,item);
		
		LOG(CJSON, CJSON, "arr ：%s suc\n",file_id_arr[i]);
	}
	
	cJSON_AddItemToObject(root,"games",arr);
	LOG(CJSON, CJSON, "root ：%s suc\n",file_id_arr[i]);
	
	
	out = cJSON_Print(root);
	
	*out_p = out;

END:
	
	if (conn != NULL) {
        rop_disconnect(conn);
    }
    
  if(file_id_arr!=NULL)
  {
  	free(file_id_arr);
  	file_id_arr = NULL;
  }
	return ret;
}