# coding=utf-8

import socket

s = socket.socket()

#host = socket.gethostname()          #单台电脑
host = "192.168.1.64"               #多台电脑
port = 1234

s.connect((host,port))
print s.recv(1024)