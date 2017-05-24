#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "redis_op.h"

#include "fdfs_api.h"

#include "make_log.h"

#define ROP_MODULE "store"
#define ROP_PROC "file"

#define FILE_INFO_LIST      "FILE_INFO_LIST"
#define FILEID_NAME_HASH    "FILEID_NAME_HASH"
#define FILEID_USER_HASH    "FILEID_USER_HASH"
#define FILEID_TIME_HASH    "FILEID_TIME_HASH"

#define FILEID_PV_HASH    	"FILEID_PV_HASH"

#define FILEID_TYPE_HASH    "FILEID_TYPE_HASH"
#define FILEID_URL_HASH     "FILEID_URL_HASH"
#define FILEID_SHARED_HASH     "FILEID_SHARED_HASH"


#define TIME_LEN (128)

/* -------------------------------------------*/
/**
 * @brief  根据一个文件路径 入库一个文件 
 *
 * @param filename 文件的路径
 *
 * @returns   
 *      succ 0, -1 fail
 */
/* -------------------------------------------*/

int upload_file_by_name(char *filename);