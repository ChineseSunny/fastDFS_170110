#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "redis_op.h"
#include "make_log.h"

#define MODULE "test"
#define PROC    "select_files"

#define FILE_INFO_LIST      "FILE_INFO_LIST"
#define FILEID_NAME_HASH    "FILEID_NAME_HASH"
#define FILEID_USER_HASH    "FILEID_USER_HASH"
#define FILEID_TIME_HASH    "FILEID_TIME_HASH"
#define FILEID_PV_HASH      "FILEID_PV_HASH"

int main(int argc, char *argv[])
{
    int retn = 0;
    int count = 0;
    int i = 0;
    RVALUES file_id_array = NULL;

    redisContext *conn = NULL;


    conn = rop_connectdb_nopwd("127.0.0.1", "6379");
    
    if (conn == NULL) {
        LOG(MODULE, PROC, "conn db error");
        retn = -1;
        goto END;
    }
		
    count = rop_list_len(conn, FILE_INFO_LIST);
    
  
    
    if (count < 0) {
        printf("list len error\n");
        retn = -1;
        goto END;
    }
	
	
    file_id_array = malloc(count *VALUES_ID_SIZE);
    

    retn = rop_range_list(conn, FILE_INFO_LIST, 0, count, file_id_array, &count);
    if (retn == -1) {
        printf("list range error\n");
        goto END;
    }

    for (i = 0;i < count ; i++) {
        char name[VALUES_ID_SIZE] ={0};
        char user[VALUES_ID_SIZE] ={0};
        char time[VALUES_ID_SIZE] ={0};
        char pv[VALUES_ID_SIZE] ={0};
				
				//printf("********\n");
				
        LOG(MODULE, PROC, "====================");
        LOG(MODULE, PROC, "file_id = [%s]", file_id_array[i]);

        rop_hget_string(conn, FILEID_NAME_HASH, file_id_array[i], name);
        //LOG(MODULE, PROC, "name = [%s]", name);
        printf("name %s\n",name);

				rop_hget_string(conn, FILEID_USER_HASH, file_id_array[i], user);
        //LOG(MODULE, PROC, "user = [%s]", user);
				printf("user %s\n",user);

        rop_hget_string(conn, FILEID_TIME_HASH, file_id_array[i], time);
       // LOG(MODULE, PROC, "time = [%s]", time);
				printf("time %s\n",time);
				
        

        rop_hget_string(conn, FILEID_PV_HASH, file_id_array[i], pv);
        
        printf("pv %s\n",pv);
        //LOG(MODULE, PROC, "pv = [%s]", pv);
    }


END:
    if (conn != NULL) {
        rop_disconnect(conn);
    }
    if (file_id_array != NULL) {
        free(file_id_array);
    }

	return retn;
}
