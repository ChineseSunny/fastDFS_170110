

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

//#include "fdfs_client.h"
//#include "logger.h"

#define FILE_ID_LEN (128)

int myupload_byexec(char *filename,char *file_id);

int fdfs_upload_by_filename(const char *filename, char *file_id);