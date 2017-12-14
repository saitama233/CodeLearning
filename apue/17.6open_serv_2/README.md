gcc main_server.c control_client.c ../13.3daemonize.c loop_select.c handle_request.c buf_args.c cli_args.c ../17.4send_fd.c ../17.4send_err.c ../17.3serv_listen.c ../17.3serv_accept.c ../14.7readn_writen.c ../errorlog.c -o opend
<br>
gcc main_client.c csopen.c ../17.3cli_conn.c ../17.4recv_fd.c -o open
<br>
在客户端输入文件名的时候需要输入绝对路径，因为opend是守护进程形式，其中使用了chdir("/")，把服务进程的目录变成了根目录，不再是之前的目录了
