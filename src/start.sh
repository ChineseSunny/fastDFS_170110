#停止tracker
sudo fdfs_trackerd /etc/fdfs/tracker.conf stop

#停止storage
sudo fdfs_storaged /etc/fdfs/storage.conf stop

#启动tracker
sudo fdfs_trackerd /etc/fdfs/tracker.conf

sudo fdfs_storaged /etc/fdfs/storage.conf


#启动redis
redis-server ./conf/redis.conf

#杀死已经启动的后台CGI程序
kill -9 `ps aux | grep "demo_cgi` | grep -v grep | awk ’{print $2}‘`

#启动demo_cgi

spawn-dcgi -a 127.0.0.1 -p 8081 -f ./demo_cgi
