# coding=utf-8
# 报错ConfigParser.NoSectionError: No section: 'messages'

from ConfigParser import ConfigParser

CONFIGFILE = r"D:\chenxin\chenxin\PythonPrograms\Python基础教程\19.4.2 代码清单19-1 简单的配置文件.txt"

# 创建ConfigParser对象
config = ConfigParser()
# 读取配置文件
config.read(CONFIGFILE.decode('utf-8'))

# 打印初始的问候语：
# 要查看的字段是‘messages’
print config.get("messages", "greeting")

# 使用配置文件的一个问题读取半径：
radius = input(config.get("messages", "question"))

print config.get("messages", "result_message") + ' ' + str(config.getfloat("numbers", "pi")*radius*radius)
