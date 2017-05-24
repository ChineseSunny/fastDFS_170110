

// 根据feile_id 查询 redis 数据库信息  接口封装

#pragma once

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

#define ROP_MODULE "test"
#define ROP_PROC "redis_op"

#include "cJSON.h"

#define FILE_INFO_LIST      "FILE_INFO_LIST"
#define FILEID_NAME_HASH    "FILEID_NAME_HASH"
#define FILEID_USER_HASH    "FILEID_USER_HASH"
#define FILEID_TIME_HASH    "FILEID_TIME_HASH"

#define FILEID_PV_HASH    	"FILEID_PV_HASH"

#define FILEID_TYPE_HASH    "FILEID_TYPE_HASH"
#define FILEID_URL_HASH     "FILEID_URL_HASH"
#define FILEID_SHARED_HASH     "FILEID_SHARED_HASH"


int select_file_to_cjson(int fromID,int count,char **out_p);