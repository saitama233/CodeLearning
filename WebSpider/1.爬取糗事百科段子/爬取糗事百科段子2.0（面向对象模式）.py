__author__ = 'cx'
# coding=utf-8
import urllib
import urllib2
import re
from ConfigParser import ConfigParser
import thread
import time

# 把内容中的<\br>替换成换行
# 添加个筛选：只显示点赞数大于4000的段子
# 把筛选放在配置文件里
# 用py2exe创建可执行程序（运行exe文件时按下回车闪下就没了，原来是要把配置文件放在dist文件夹里）

# 糗事百科爬虫类
class QSBK:
    # 初始化方法
    def __init__(self):
        self.pageIndex = 1
        self.user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0'
        # 初始化headers
        self.headers = {'User-Agent': self.user_agent}
        # 存放段子的变量，每一个元素是每一页的段子们
        self.stories = []
        # 存放程序是否继续运行的变量
        self.enable = False
        # 配置文件初始化
        CONFIGfILE = '糗事百科配置.cfg'
        self.config = ConfigParser()
        self.config.read(CONFIGfILE.decode('utf-8'))

    # 获取某一页的源代码
    def getPageSourceCode(self, pageIndex):
        try:
            url = 'http://www.qiushibaike.com/hot/page/' + str(pageIndex)
            # 构建请求的request
            request = urllib2.Request(url, headers=self.headers)
            # 利用urlopen获取页面代码
            response = urllib2.urlopen(request)
            # 将页面转化为UTF-8编码
            pageSourceCode = response.read().decode('utf-8')
            return pageSourceCode
        except urllib2.URLError, e:
            if hasattr(e, "reason"):
                print u"连接糗事百科失败，错误原因", e.reason
                return None

    # 把需要的内容用正则表达式选出来并放在一个列表里（这里只爬当前页的内容）
    def getPageItems(self, pageIndex):
        pageSourceCode = self.getPageSourceCode(pageIndex)
        if not pageSourceCode:
            print "页面加载失败"
            return None
        pattern = re.compile('<div.*?author.*?<h2>(.*?)</h2>.*?</div>' +
                             '.*?<div.*?content.*?<span>(.*?)</span>.*?</div>' +
                             '.*?<div.*?stats.*?<i.*?number">(.*?)</i>.*?</div>'
                             , re.S)
        items = re.findall(pattern, pageSourceCode)
        pageStories = []
        for item in items:
            if (int(item[2]) > self.config.getfloat('numbers','vote_filter')):
                content = item[1].replace('<br/>','\n')
                pageStories.append([item[0].strip(), content.strip(), item[2].strip()])

        return pageStories

    def loadPage(self):
        # 如果当前未看的页数少于2页，则加载新一页
        if self.enable:
            if len(self.stories) < 2:
                # 获取新一页
                pageStories = self.getPageItems(self.pageIndex)
                # 将该页的段子存放在全局变量stories中
                if pageStories:
                    self.stories.append(pageStories)
                    # 获取完之后页码索引加1，表示下次读取下一页
                    self.pageIndex += 1

    def getOneStory(self, pageStories, page):
        for story in pageStories:
            # 等待用户输入
            input = raw_input()
            # 每当输入一次回车，判断一下是否要加载新页面
            self.loadPage()
            # 如果输入Q则结束程序
            if input == 'Q' or input == 'q' or input == 'quit':
                self.enable = False
                return
            print u"作者：%s\n内容：\n%s\n点赞数:%s\n" % (story[0], story[1], story[2])

    def start(self):
        print u"正在读取糗事百科，按回车查看新段子，输入Q,q或quit退出"
        # 使变量为true，程序可以正常运行
        self.enable = True
        # 先加载一页内容
        self.loadPage()
        # 局部变量，控制当前读到了第几页
        nowPage = 0
        while self.enable:
            if len(self.stories) > 0:
                pageStories = self.stories[0]
                nowPage += 1
                del self.stories[0]
                self.getOneStory(pageStories, nowPage)


spider = QSBK()
spider.start()
