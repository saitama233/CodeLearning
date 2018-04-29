openssl.sh为生成秘钥及证书的脚本
执行脚本后
ca的certs文件夹为程序所用的根证书，服务器证书和客户端证书
ca的private文件夹为私钥及证书请求文件

make MODE=1为单向验证
make MODE=2为双向验证
