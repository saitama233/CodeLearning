# coding=utf-8
# 转换类：MarkDown->HTML
import re


class GetHead:
    def __init__(self):
        pass

    def gethead(self):
        sec = "<link href=\"http://apps.bdimg.com/libs/highlight.js/9.1.0/styles/default.min.css\" rel=\"stylesheet\">\n" +\
                "<script src=\"http://apps.bdimg.com/libs/highlight.js/9.1.0/highlight.min.js\"></script>\n" +\
                "<script>hljs.initHighlightingOnLoad();</script>\n"
        return sec


class Rule:
    pass


# 转换标题，以#开头的字符串
class TitleRule(Rule):
    def __init__(self):
        self.pattern = re.compile('(#+)\s*(.*?)\n')

    def condition(self, section):
        match = re.search(self.pattern, section)
        if match:
            return True
        else:
            return False

    def replace(self, section):
        match = re.findall(self.pattern, section)
        sec = ""
        match = match[0]
        if len(match[0]) == 1:
            sec = '<h1>' + match[1] + '</h1>\n'
        elif len(match[0]) == 2:
            sec = '<h2>' + match[1] + '</h2>\n'
        elif len(match[0]) == 3:
            sec = '<h3>' + match[1] + '</h3>\n'
        elif len(match[0]) == 4:
            sec = '<h4>' + match[1] + '</h4>\n'
        elif len(match[0]) == 5:
            sec = '<h5>' + match[1] + '</h5>\n'
        elif len(match[0]) == 6:
            sec = '<h6>' + match[1] + '</h6>\n'
        return sec


# 转换无序列表，以+、-、*加一个空格开头的字符串
class UnorderedList(Rule):
    def __init__(self):
        self.pattern = re.compile(r'(-)\s+(.*?)\n|(\+)\s+(.*?)\n|(\*)\s+(.*?)\n')

    def condition(self, section):
        match = re.findall(self.pattern, section)
        if match:
            return True
        else:
            return False

    def replace(self, section):
        match = re.findall(self.pattern, section)
        sec = ""
        for line in match:
            part = ''
            if line[0] != '':
                part = "<li>" + line[1] + "</li>\n"
            elif line[2] != '':
                part = "<li>" + line[3] + "</li>\n"
            elif line[4] != '':
                part = "<li>" + line[5] + "</li>\n"
            sec += part
        sec = "<ul>\n" + sec + "</ul>\n"
        return sec


# 转换有序列表，以num.空格开头
class OrderedList(Rule):
    def __init__(self):
        self.pattern = re.compile(r'(\d\.)\s+(.*?)\n')

    def condition(self, section):
        match = re.findall(self.pattern, section)
        if match:
            return True
        else:
            return False

    def replace(self, section):
        match = re.findall(self.pattern, section)
        sec = ""
        for line in match:
            part = ''
            part = "<li>" + line[1] + "</li>\n"
            sec += part
        sec = "<ol>\n" + sec + "</ol>\n"
        return sec


# 转换图片 ![]()格式
class Image(Rule):
    def __init__(self):
        self.pattern = re.compile(r'!\[(.*?)\]\((.*?)\)\n')

    def condition(self, section):
        match = re.findall(self.pattern, section)
        # print match
        if match:
            return True
        else:
            return False

    def replace(self, section):
        match = re.findall(self.pattern, section)
        sec = ""
        sec = "<img src=\"" + "".join(match[0][1]) + \
              "\"alt=\"" + "".join(match[0][0]) + "\" />\n"
        return sec


class Code(Rule):
    def __init__(self):
        self.pattern = re.compile(r"```\s*(.*?)\n(.*?)```\n", re.S)

    def condition(self, section):
        match = re.findall(self.pattern, section)
        if match:
            return True
        else:
            return False

    def replace(self, section):
        match = re.findall(self.pattern, section)
        sec = ""
        sec = "<pre>\n<code class=\"" + "".join(match[0][0]) + "\">\n" + \
              "".join(match[0][1]) + "</code>\n</pre>\n"
        return sec

