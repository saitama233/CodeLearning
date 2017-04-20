# coding=utf-8
#读取类

# 现在只实现了从txt中读取，也可以设置从Word读取，用win32com模块
# 读到两个回车就分一部分
class readFromTxt:
    def __init__(self, filename):
        self.filename = filename

    def readfile(self):
        section = []
        f = open(self.filename)
        for line in f:
            if line != '\n':
                line = line.decode('gbk')
                section.append(line)
            else:
                yield section
                section = []
