# coding=utf-8
import urllib2
import re
import os
from HandleDB import HandleDBdouban250
from HandleWord import HandleWord


class DBfilm250:
    def __init__(self):
        self.user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
        # 初始化headers
        self.headers = {'User-Agent': self.user_agent}
        self.items = []
        # self.hw = HandleWord("db250.doc")
        self.imgindex = 1
        self.imgPath = os.path.join(os.getcwd(), 'picture')
        os.chdir(os.getcwd())
        if not os.path.isdir('picture'):
            os.mkdir('picture')
        else:
            print "picture目录已存在"
        f = open(r'douban250.txt', 'w')
        f.close()


    def getPage(self, pageIndex):
        try:
            url = 'https://movie.douban.com/top250?start=' + str(pageIndex)
            request = urllib2.Request(url, headers=self.headers)
            response = urllib2.urlopen(request)
            pageCode = response.read()
            return pageCode

        except urllib2.URLError, e:
            if hasattr(e, "reason"):
                print u"连接豆瓣失败，错误原因", e.reason
                return None

    def getPageItems(self,pagecode):
        pattern = re.compile(r'<div class="item.*?<em class="">(.*?)</em>.*?' +  # 排名
                             '<a href="(.*?)">.*?' +              # 影片链接
                             '<img.*?src="(.*?)" class="".*?' +   # 影片图片
                             '<span class="title">(.*?)</span>.*?' +    # 电影名
                             '<p class="">(.*?)</p>.*?' +          # 影片信息
                             '"rating_num".*?>(.*?)</span>.*?' +   # 影片评分
                             '<span>(.*?)</span>.*?' +        # 评价人数
                             '(class="inq">(.*?)</span>|</li>)'     # 简评
                             , re.S)
        items = re.findall(pattern, pagecode)

        f = open(r'douban250.txt', 'a')
        for item in items:
            text =  "排名：" + "".join(item[0]) + "\n影片链接：" + "".join(item[1]) \
                    + "\n影片图片：" + "".join(item[2]) + "\n影片名：" + "".join(item[3])  \
                    + "\n影片信息：" + "".join(item[4]) + "\n影片评分：" + "".join(item[5]) \
                    + "\n评价人数：" + "".join(item[6]) + "\n简评：" + "".join(item[8]) + "\n\n\n"
            print "排名：" + "".join(item[0]) + "\n影片链接：" + "".join(item[1]) \
                    + "\n影片图片：" + "".join(item[2]) + "\n影片名：" + "".join(item[3])  \
                    + "\n影片信息：" + "".join(item[4]) + "\n影片评分：" + "".join(item[5]) \
                    + "\n评价人数：" + "".join(item[6]) + "\n简评：" + "".join(item[8]) + "\n\n\n"
            f.write(text)
        f.close()

        return items

    def downloadImg(self, url):
        img = urllib2.urlopen(url).read()
        # print img
        filename = os.path.join(self.imgPath, str(self.imgindex)+'.jpg')
        with open(filename, 'wb') as f:
            f.write(img)
        self.imgindex += 1
        return filename

    def start(self):
        db = HandleDBdouban250()
        for pageIndex in range(0, 250, 25):
            pagecode = self.getPage(pageIndex)
            pageitems = self.getPageItems(pagecode)
            for item in pageitems:
                filename = self.downloadImg(item[2])
                temp = (item[0], item[1], item[2], item[3], item[4], item[5], item[6], item[8])
                db.insert_item(temp)


 # if __name__ == '__main__':
print os.path.join(os.getcwd(), 'picture')
db250 = DBfilm250()
db250.start()
