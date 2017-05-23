#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hiredis.h"

int main()
{
	redisContext *Conn = NULL;
	//��������
	Conn = redisConnect("127.0.0.1", 6379);
	if (Conn == NULL || Conn->err)
	{
    if (Conn) 
    {
    	printf("Error: %s\n", Conn->errstr);
		}
		else
		{
			printf("redisConnect err\n");
		}
		
	}
	
	//ʹ��reply
	redisReply* reply = redisCommand(Conn, "SET %s %s","test_string","helloHiredis");

	if(NULL == reply)
	{
		printf("redisCommand err\n");
	}
	
	printf("set %s success\n","test_string");
	
	freeReplyObject(reply);

	reply = redisCommand(Conn, "GET %s","test_string");
	
	if(NULL == reply)
	{
		printf("redisCommand err\n");
		if(Conn !=NULL)
		{
			//�Ͽ�����
    	redisFree(Conn);
		}
	}
	
	//����ֵ
	
  printf("value = %s,len = %d\n",reply->str,reply->len);
	freeReplyObject(reply);
	
	char *list_key = "my_list_test";
	
	int list_len = 0;

	reply = redisCommand(Conn, "lpush %s %s",list_key,"cppchina");
	if( NULL == reply)
	{
		 printf("lpush error\n");
		 redisFree(Conn);
	}
	
	//�б����
	list_len = reply->integer ;
	printf("lpush succ len = %d\n", list_len);
	freeReplyObject(reply);
	 
	
	return 0;	
}
