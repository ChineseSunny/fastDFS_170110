
CC=gcc
CPPFLAGS= -I../inc -I/usr/include/fastdfs -I/usr/include/fastcommon  -I/usr/local/include/hiredis/ 
CFLAGS=-Wall 
LIBS= -lfdfsclient -lfastcommon -lhiredis -lfcgi -lm  

#main=./main
g_hiredis =./hiredis_test
g_redisop = ./redis_op
g_fdfs_redis_savefile = ./fdfs_redis_savefile
select_redis_file = ./select_redis_file

#demo_cgi=./demo_cgi
#echo_cgi=./echo_cgi
upload_cgi=./upload_cgi
data_cgi = ./data_cgi
reg_cgi = ./reg_cgi
login_cgi = ./login_cgi


target=$(main) $(g_hiredis) $(test_redisop) $(stest_fdfs_redis_savefile) $(select_redis_files) $(demo_cgi) $(echo_cgi) $(upload_cgi) $(data_cgi) $(reg_cgi) $(login_cgi)


ALL:$(target)


#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#main程序
#$(main):./main.o ./fdfs_api.o make_log.o
#	$(CC) $^ -o $@ $(LIBS)


#g_hiredis程序
$(g_hiredis):./hiredis_test.o 
	$(CC) $^ -o $@ $(LIBS)

#g_redisop程序
$(g_redisop):./redis_op_test.o make_log.o redis_op.o
	$(CC) $^ -o $@ $(LIBS)

#g_fdfs_redis_savefile 程序
$(g_fdfs_redis_savefile):./fdfs_redis_savefile.o redis_op.o make_log.o fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)

#select_files程序
$(select_redis_files):./select_redis_files.o redis_op.o make_log.o
	$(CC) $^ -o $@ $(LIBS)

#demo_files程序
#$(demo_cgi):./demo_cgi.o
#	$(CC) $^ -o $@ $(LIBS)

#echo_cgi程序
#$(echo_cgi):./echo_cgi.o
#	$(CC) $^ -o $@ $(LIBS)

#upload_cgi程序
$(upload_cgi):./upload_cgi.o ./make_log.o util_cgi.o upload_file.o redis_op.o fdfs_api.o
	$(CC) $^ -o $@ $(LIBS)

#data_cgi程序
$(data_cgi):./data_cgi.o ./make_log.o util_cgi.o redis_op.o  select_redis_file.o cJSON.o
	$(CC) $^ -o $@ $(LIBS)

#reg_cgi程序
$(reg_cgi):./reg_cgi.o  make_log.o util_cgi.o cJSON.o redis_op.o
	$(CC) $^ -o $@ $(LIBS) $(CPPFLAGS)

#login_cgi程序
$(login_cgi):./login_cgi.o  make_log.o util_cgi.o  cJSON.o redis_op.o
	$(CC) $^ -o $@ $(LIBS)


#clean指令

clean:
	-rm -rf ./*.o $(target) 

distclean:
	-rm -rf ./*.o $(target) 

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
