#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "redis_op.h"
#include "make_log.h"

int main()
{
	int ret = 0;
	
		//���� redis ���Ӿ��
	redisContext* conn;
	char value[1024] = {0};
	
	//����redis tcp���ݿ�
	conn = rop_connectdb_nopwd("127.0.0.1","6379");
	
	if(NULL == conn)
	{
		 LOG(ROP_MODULE, ROP_PROC, "conn db error");
		 ret = -1;
		 goto END;
	}
	
	
	
	
	
	
	
		if(Conn !=NULL)
		{
			//�Ͽ�����
    	redisFree(Conn);
		}
	
	
END:
	return ret;
}