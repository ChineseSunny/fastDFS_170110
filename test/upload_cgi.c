#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "upload_file_read.h"

#include "make_log.h"
#include "util_cgi.h"


#include "fcgi_stdio.h"
#include "fcgi_config.h"


extern char **environ;

#define FCGI "fcgi"
#define UPLOAD "upload"

#define FDFS "FDFS"
#define PROC "proc"

int main (int argc, char *argv[])
{
	int i = 0,ret = 0, ch;
  char *buf = NULL;
  char *p = NULL ; char * begin = NULL; char * end = NULL;
	
	char *file_id = NULL;
	char *q = NULL; char *k = NULL;
	//分割线
	char bound[256] = { 0 };
	
	char filename[256] = { 0 };
	
	char content[256] = { 0 }; 
	
	char type[256] = { 0 };
	
		//接收、建立连接
    while (FCGI_Accept() >= 0) 
    {
        //当前请求的post数据的长度
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        printf("Content-type: text/html\r\n"
                "\r\n");

        if (contentLength != NULL) {
            //有post数据
            len = strtol(contentLength, NULL, 10);  //当前请求的post数据的长度
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input.<p>\n");
        }
        else {
            //在处理post数据
            
						//开辟存放post数据的内存
            buf = malloc(len);
            if (buf == NULL) {
                LOG(FCGI, UPLOAD, "malloc buf error");
                break;
            }
            
            memset(buf, 0, len);
           	//指针 p 指向 post 首部，临时指针
            begin = p = buf;

            printf("Standard input:<br>\n<pre>\n");
          
          	//按 - 字节 - 读取 post 数据
            for (i = 0; i < len; i++) {
                //读一个字节
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //回显一个字节
                //putchar(ch);
                *p = ch;
                p++;		
            }
            
            end = p;		// end/p 指向数据 尾部
            printf("\n</pre><p>\n");
						
						//将buf 存到本地，进行分析
            FILE *post_fp = fopen("./post_data.txt", "w");
            fwrite(buf, len , 1, post_fp);
            fclose(post_fp);
            
            
            //处理web服务器发来的数据
            //p指向第一行尾部
						p = strstr(begin,"\r\n");
					
						if(NULL ==p)
						{
							LOG(FCGI,UPLOAD, "wrong no boundary\n");
              goto END;
						}
						//得到分割线
						strncpy(bound,begin,p-begin);
						bound[p-begin] = '\0';
						LOG(FCGI,UPLOAD, "bound : %s\n",bound);
						
						//已经处理了p - begin 长度，post剩余长度
						
						len -= (p - begin);
						
						//p 指向第二行
						p += 2 ;
						len -= (p - begin);	
					
						q = begin = p;
						
						p = strstr(begin,"\r\n");
						if(NULL ==p)
						{
							LOG(FCGI,UPLOAD, "wrong no content\n");
              goto END;
						}
						
						strncpy(content,begin,p-begin);
						content[p-begin] = '\0';
						LOG(FCGI,UPLOAD, "content = [%s]\n",content);
						
						//截取filename
            q = strstr(begin,"filename=");
            int m = strlen("filename=");
            q += m;
            
            strncpy(filename,q+1,p-q-2);
            LOG(FCGI,UPLOAD, "filename : [%s]\n",filename);
           
            trim_space(filename);
            
            //第三行
            p +=2;
            
            //剩下的数据长度
            len -= (p - begin);
            
            begin = p;
            
            //Type
            p = strstr(begin,"\r\n");
          	q = strchr(begin,':');
          	strncpy(type,q+1,p-q);
            LOG(FCGI,UPLOAD, "type:%s\n",type);
            
            p += 4;  //空行--->正文
            begin = p;  
           
            //内容 memstr(char* full_data, int full_data_len, char* substr)
            p = memstr(begin, len, bound);
            if( NULL == p)
            {
            p = end - 2 ;
            }
            else
            {
            	p -= 2;
            }
            
            int fd = open(filename,O_WRONLY | O_CREAT,0664);
            
            if(fd < 0)
           	{
           		printf("open %s err\n",filename);
           	}
            
            ftruncate(fd,p - begin);
            write(fd,begin,p - begin);
            close(fd);
            
            //===============> 将该文件存入fastDFS中,并得到文件的file_id <============
						//redis数据库建表
						ret = upload_file_by_name(filename);
						
						if(ret)
						{
							LOG(FCGI,UPLOAD, "wrong no content\n");
              goto END;
						}
						//printf("file_id:%s\n",file_id);
			
			

            //将本地文件删除掉
            //unlink(filename);


            //================ > 得到文件所存放storage的host_name <================
            //fielid f 入redis
            
            
						
            	
            
            
            
            printf("%s 文件上传成功", filename);
            
END:
	memset( bound,0,256 );
	memset( filename,0,256 );
	memset( content,0,256 );
	
	if(NULL!=buf)
  {
     free(buf);
  }
        }

    } /* while */

    return 0;
}
