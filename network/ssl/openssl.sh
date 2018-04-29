#!/bin/sh

# 创建根证书及测试证书脚本

# 复制配置文件到本目录，并修改其中的dir变量为./ca
cp /etc/ssl/openssl.cnf .
sed -i -e '42d' -e '43i dir = ./ca' openssl.cnf

# 创建需要的目录
mkdir -p ca/certs
cd ca
mkdir newcerts
mkdir private
mkdir crl
touch index.txt
echo 01 > serial

# 生成根证书
### 生成根证书私钥
echo "openssl genrsa -aes256 -out private/cakey.pem 4096:"
openssl genrsa -aes256 -out private/cakey.pem 4096
### 生成根证书签发文件
echo "openssl req -new -key private/cakey.pem -out private/ca.csr -subj \"/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=CA\":"
openssl req -new -key private/cakey.pem -out private/ca.csr -subj "/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=CA"
### 自签发根证书
echo "openssl x509 -req -days 365 -sha256 -extensions v3_ca -signkey private/cakey.pem -in private/ca.csr -out certs/ca.cer:"
openssl x509 -req -days 365 -sha256 -extensions v3_ca -signkey private/cakey.pem -in private/ca.csr -out certs/ca.cer

# 用根证书签发server端证书
### 生成服务端私钥
echo "openssl genrsa -aes256 -out private/server-key.pem 4096:"
openssl genrsa -aes256 -out private/server-key.pem 4096
### 生成服务器证书请求文件
echo "openssl req -new -key private/server-key.pem -out private/server.csr -subj \"/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=SERVER\":"
openssl req -new -key private/server-key.pem -out private/server.csr -subj "/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=SERVER"
### 用根证书签发服务端证书
echo "openssl x509 -req -days 365 -sha256 -extensions v3_req -CA certs/ca.cer -CAkey private/cakey.pem -CAserial ca.srl -CAcreateserial -in private/server.csr -out certs/server.cer:"
openssl x509 -req -days 365 -sha256 -extensions v3_req -CA certs/ca.cer -CAkey private/cakey.pem -CAserial ca.srl -CAcreateserial -in private/server.csr -out certs/server.cer

# 用根证书签发client端证书
### 生成客户端私钥
echo "openssl genrsa -aes256 -out private/client-key.pem 4096:"
openssl genrsa -aes256 -out private/client-key.pem 4096
### 生成客户端证书请求文件
echo "openssl req -new -key private/client-key.pem -out private/client.csr -subj \"/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=CLIENT\":"
openssl req -new -key private/client-key.pem -out private/client.csr -subj "/C=JP/ST=QY/L=SAI/O=WM/OU=RIP/CN=CLIENT"
### 用根证书签发客户端证书
echo "openssl x509 -req -days 365 -sha256 -extensions v3_req -CA certs/ca.cer -CAkey private/cakey.pem -CAserial ca.srl -in private/client.csr -out certs/client.cer:"
openssl x509 -req -days 365 -sha256 -extensions v3_req -CA certs/ca.cer -CAkey private/cakey.pem -CAserial ca.srl -in private/client.csr -out certs/client.cer
