
#include "select_redis_file.h"


int select_file_to_cjson(int fromID,int count,char **out_p)
{
	
	int ret = 0;
	int file_num = 0;
	
	char *out = NULL;
	//���� redis ���Ӿ��
	
	redisContext* conn = NULL;
	
	RVALUES file_id_arr = NULL;
	
	//���� cjson  root object
	cJSON * root = cJSON_CreateObject();
	
	//���� cjson array
	cJSON * arr = cJSON_CreateArray();
	
	//����redis tcp���ݿ�
	conn = rop_connectdb_nopwd("127.0.0.1", "6379");
	
	if(NULL == conn)
	{
		LOG(ROP_MODULE, ROP_PROC, "conn db error");
		ret = -1;
		goto END;
	}
	
	//��ȡ������
	count = rop_list_len(conn,"FILE_INFO_LIST");
	
	if(count < 0)
	{
		 LOG(ROP_MODULE, ROP_PROC, "rop_list_len err");
		 ret = -1;
		 goto END;
	}
	
	file_id_arr = malloc(count * VALUES_ID_SIZE);
	
	
	ret = rop_range_list(conn, "FILE_INFO_LIST", 0,count, file_id_arr, &file_num);
	
	if(ret)
	{
		 LOG(ROP_MODULE, ROP_PROC, "rop_range_list err");
		 ret = -1;
		 goto END;
	}

	int i = 0;
		
	for(i =0;i< count;++i)
	{
		//���� cjson С object
		cJSON * item = cJSON_CreateObject();
		
		char name[VALUES_ID_SIZE] = { 0 };
		char user[VALUES_ID_SIZE] = { 0 };
		char time[VALUES_ID_SIZE] = { 0 };
		char pv[VALUES_ID_SIZE] = { 0 };
		
		char type[VALUES_ID_SIZE] = { 0 };
		char url[VALUES_ID_SIZE] = { 0 };
		char shar_status[VALUES_ID_SIZE] = { 0 };
	
		//printf("file_id : %s\n",file_id_arr[i]);
		//��� id �� value ��
		cJSON_AddStringToObject(item,"id",file_id_arr[i]);
		
		
		rop_hget_string(conn,FILEID_NAME_HASH, file_id_arr[i],name);
		//printf("name : %s\n",name);
		cJSON_AddStringToObject(item,"title_m",name);
		
		
		rop_hget_string(conn,FILEID_USER_HASH, file_id_arr[i],user);
		//printf("user : %s\n",user);
		cJSON_AddStringToObject(item,"title_s",user);
		
		rop_hget_string(conn,FILEID_TIME_HASH, file_id_arr[i],time);
		//printf("time : %s\n",time);
		cJSON_AddStringToObject(item,"descrip",time);
		
		rop_hget_string(conn,FILEID_PV_HASH, file_id_arr[i],pv);
		//printf("pv : %s\n",pv);
		cJSON_AddNumberToObject(item,"pv",atoi(pv));
		
		 //kind (Ԥ���ֶ�)
     cJSON_AddNumberToObject(item, "kind", 0);

     //picurl_m(ͼƬlogourl��ַ)
     char pic_url[VALUES_ID_SIZE] = {0};
		//TYPE
		rop_hget_string(conn,FILEID_TYPE_HASH, file_id_arr[i],type);
		strcat(pic_url, "http://");
    strcat(pic_url, "192.168.202.136");
    strcat(pic_url, "/static/file_png/");
    strcat(pic_url, type);
    strcat(pic_url, ".png");
		
		cJSON_AddStringToObject(item,"picurl_m",pic_url);
		
		
		rop_hget_string(conn,FILEID_URL_HASH, file_id_arr[i],url);
		//printf("pv : %s\n",pv);
		cJSON_AddStringToObject(item,"url",url);
		
		
		rop_hget_string(conn,FILEID_SHARED_HASH, file_id_arr[i],shar_status);
		//printf("pv : %s\n",pv);
		cJSON_AddNumberToObject(item,"hot",atoi(shar_status));
		
		
		
		//cJSON_AddItemToArray(cJSON *array, cJSON *item);
		//���object ---> array
		cJSON_AddItemToArray(arr,item);
	}
	
	cJSON_AddItemToObject(root,"games",arr);
	
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