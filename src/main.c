#include <unistd.h>
#include <pthread.h>

#include "fdfs_api.h"
#include "make_log.h"

#define FDFS_MAIN "main"
#define FDFS_MAIN_PROC "fdfs_api"

int main(int argc,char *argv[])
{
	int ret = 0;
	char file_id[128] = { 0 };
	
	ret = fdfs_upload_by_filename("a.c",file_id);
	if(ret)
	{
		printf("myupload_byexec err\n");
		return ret;
	}
	
	//LOG(FDFS_MAIN,FDFS_MAIN_PROC,"%s",file_id);
	printf("file_id = [%s]\n",file_id);
	
	return 0;
}
