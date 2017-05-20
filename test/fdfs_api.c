#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include "fdfs_api.h"


int myupload_byexec(char *filename,char *file_id)
{
	int pipefd[2];
	
	int ret = pipe(pipefd);
	
	if(-1 == ret)
	{
		perror("pipe error");
		exit(0);
	}
	
	pid_t pid = fork();
	
	if(-1 == pid)
	{
		
		perror("fork error");
		exit(0);
	}
	if( 0 == pid)
	{
		//写，关闭读
		close(pipefd[0]);
		
		dup2(pipefd[1],STDOUT_FILENO);
		
		execlp("fdfs_upload_file","upload","/etc/fdfs/client.conf",filename,NULL);
	}
	
	if(pid > 0 )
	{
		sleep(0.3);
		//从管道读数据，关闭写
		close(pipefd[1]);
		
		//char buf[1024]={0};
		
		wait(NULL);
		
		int len = read(pipefd[0],file_id,1024);
		file_id[len - 1] = '\0';
		
		//printf("%s",file_id);
		
		close(pipefd[0]);
		
	}
	
}

int fdfs_upload_by_filename(const char *filename, char *file_id)
{
	const char *local_filename;
	
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;

	log_init();
	g_log_context.log_level = LOG_ERR;
	
	ignore_signal_pipe();

  //加载一些配置文件
	if ((result=fdfs_client_init("/etc/fdfs/client.conf")) != 0)
	{
		return result;
	}

    //通过配置问价拿到内容 得到一个tracker链接的句柄
	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

	local_filename = filename;
	*group_name = '\0';

    //通过询问tracker句柄 得到一个可用的storage句柄
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		return result;
	}

    //根据得到的storage句柄 上传本地local_filename
	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
        //上传succ
		printf("%s\n", file_id);
	}
	else
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

    //断开链接 回收资源
	tracker_disconnect_server_ex(pTrackerServer, true);
    
	fdfs_client_destroy();

	return result;
}