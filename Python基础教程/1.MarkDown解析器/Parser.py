# coding=utf-8
from ReadMD import readFromTxt
from Rules import *
import sys

class Parser:
    def __init__(self):
        self.rules = []
        # 转换后的内容（所有）
        self.blocks = []

    def addRule(self, rule):
        self.rules.append(rule)

    def parse(self, filename):
        n = 0
        ftt = readFromTxt(filename)
        head = GetHead()
        self.blocks.append(head.gethead())
        for section in ftt.readfile():
            # 在这步需要判断下列表是否为空，否则会处理空列表
            if section:
                # print n
                section = "".join(section)
                # n += 1
                # 进行转换
                sec = ""
                for rule in self.rules:
                    if rule.condition(section):
                        sec = rule.replace(section)
                self.blocks.append(sec)
                # print sec
        # 输出
        for line in self.blocks:
            print line


# 这个类增加了扩展性，可以选择转换成html或者其他的什么格式
class BasicTextParser(Parser):
    def __init__(self):
        Parser.__init__(self)
        self.addRule(TitleRule())
        self.addRule(UnorderedList())
        self.addRule(OrderedList())
        self.addRule(Image())
        self.addRule(Code())

parser = BasicTextParser()
parser.parse("MarkDown.txt")

