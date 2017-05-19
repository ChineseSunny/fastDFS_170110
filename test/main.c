#include <unistd.h>
#include <pthread.h>

#include "fdfs_api.h"

int main(int argc,char *argv[])
{
	int ret = 0;
	char file_id[128] = { 0 };
	
	ret = myupload_byexec("a.c",file_id);
	if(ret)
	{
		printf("myupload_byexec err\n");
		return ret;
	}
	printf("file_id = [%s]\n",file_id);
	
}
