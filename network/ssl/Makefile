# Usage: make MODE=1       单向认证
#        make MODE=2       双向认证

all: clean ssl

CC = gcc
CFLAGS = -DMODE=$(MODE)
OBJS = ssl_server.o ssl_client.o

ssl: $(OBJS)
	$(CC) -o ssl_server ssl_server.o -lssl -lcrypto
	$(CC) -o ssl_client ssl_client.o -lssl -lcrypto

clean:
	rm -rf *.o ssl_client ssl_server

# 下一行也可以写成.c.o，不过这种是老式风格，无法添加其他依赖文件
%.o: %.c
	$(CC) $(CFLAGS) -c $<
