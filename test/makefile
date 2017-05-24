
CC=gcc
CPPFLAGS= -I../inc -I/usr/include/fastdfs -I/usr/include/fastcommon  -I/usr/local/include/hiredis/
CFLAGS=-Wall 
LIBS= -lfdfsclient -lfastcommon -lhiredis -lfcgi -lm

select_redis_file = ./select_redis_file

data_cgi = ./data_cgi


target=$(select_redis_file)


ALL:$(target)


#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#data_cgi程序
$(data_cgi):./data_cgi.o ./make_log.o util_cgi.o redis_op.o  select_redis_file.o cJSON.o
	$(CC) $^ -o $@ $(LIBS)

#clean指令

clean:
	-rm -rf ./*.o $(target) ./*.o

distclean:
	-rm -rf ./*.o $(target) ./*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
