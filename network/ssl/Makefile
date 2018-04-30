# 单向认证
# (1)make SMODE=0 CMODE=0
# 此时服务器证书没有通过验证也能成功握手
# (2)make SMODE=0 CMODE=1
# 此时服务器证书只有通过验证才能成功握手
# 双向验证
# (有六种情况,只说两种)
# (1)make SMODE=1 CMODE=1
# 此时请求客户端证书,但若是客户端不发送也可成功握手
# (2)make SMODE=1 CMODE=3
# 此时客户端必须发送证书并成功验证才能成功握手
all: clean ssl

CC = gcc
# CFLAGS = -Wall
CFLAGS += -DSERVER_SSL_VERIFY_NODE=1
CFLAGS += -DSERVER_SSL_VERIFY_PEER=2
# 下面的宏也可以设置为1,为1的话和PEER做或操作为3,也能区分开
CFLAGS += -DSERVER_SSL_VERIFY_FAIL_IF_NO_PEER_CERT=3
CFLAGS += -DCLIENT_SSL_VERIFY_NONE=1
CFLAGS += -DCLIENT_SSL_VERIFY_PEER=2
CFLAGS += -DSMODE=$(SMODE)
CFLAGS += -DCMODE=$(CMODE)
OBJS = ssl_server.o ssl_client.o

ssl: $(OBJS)
	$(CC) -o ssl_server ssl_server.o -lssl -lcrypto
	$(CC) -o ssl_client ssl_client.o -lssl -lcrypto

clean:
	rm -rf *.o ssl_client ssl_server

# 下一行也可以写成.c.o，不过这种是老式风格，无法添加其他依赖文件
%.o: %.c
	$(CC) $(CFLAGS) -c $<
