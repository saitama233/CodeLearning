openssl.sh为生成秘钥及证书的脚本
执行脚本后
ca的certs文件夹为程序所用的根证书，服务器证书和客户端证书
ca的private文件夹为私钥及证书请求文件
<br>
<br>
### 单向认证
(1)make SMODE=0 CMODE=0
此时服务器证书没有通过验证也能成功握手
(2)make SMODE=0 CMODE=1
此时服务器证书只有通过验证才能成功握手
<br>
### 双向验证
(有六种情况,只说两种)
(1)make SMODE=1 CMODE=1
此时请求客户端证书,但若是客户端不发送也可成功握手
(2)make SMODE=1 CMODE=3
此时客户端必须发送证书并成功验证才能成功握手
