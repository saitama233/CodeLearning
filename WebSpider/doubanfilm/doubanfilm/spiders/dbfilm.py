#coding=utf8

import json
import sys
import re
import scrapy
from bs4 import BeautifulSoup
from scrapy.http import Request
from doubanfilm.items import doubanfilmitem
from doubanfilm.items import typepageurl

class Myspider(scrapy.Spider):
    name = 'doubanfilm'
    allowed_domains = ['movie.douban.com']
    url = "https://movie.douban.com/chart"
    base_url = 'https://movie.douban.com/'

#   cookies = {'__utmz': '223695111.1510210510.1.1.utmcsr', 'bid': 'rAWM3eO1Ams', '_pk_ses.100001.4cf6': '*', '_pk_id.100001.4cf6': 'bb5e1837bf02623e.1510210510.1.1510212279.1510210510.', '__utma': '223695111.1984115178.1510210510.1510210510.1510210510.1', '__utmb': '223695111.0.10.1510210510', '__utmc': '223695111'}
    # 这里的User-Agent应该没有用上，因为之后调用了中间件重新给headers赋值了
    headers = {
        'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0'
    }

    # 向排行榜主页面发出请求
    def start_requests(self):
        request = Request(url=self.url, headers=self.headers, callback=self.gettype_url)
        yield request

    # 从主页面的response里解析出每个类型排行榜的url
    def gettype_url(self, response):
        htm = BeautifulSoup(response.text, "lxml")
        urls = htm.find('div', class_="types").find_all('a')
        for url in urls:
            typeurl = self.base_url + url['href']
            # print typeurl
            # parse相当于一个开关，控制每个类型页面是直接获取JSON还是使用selenium+PhantomJS的方式 
            parse = "JSON"
            if parse == "JSON":
            # pattern = re.compile(r'type=(.*?)\&interval')
                pattern = re.compile(r'type=(.*?)&interval')
                typenum = re.findall(pattern, typeurl)
                jsonpage = "https://movie.douban.com/j/chart/top_list?type=" + typenum[0] + "&interval_id=100:90&action=&start=0&limit=1000"
                request = Request(url=jsonpage, headers=self.headers, callback=self.get_items_json)
                yield request
            elif parse == "PhantomJS":
                request = Request(url=typeurl, headers=self.headers, callback=self.get_items_phantomjs)
                request.meta['PhantomJS'] = True
                yield request

    # 使用JSON解析页面的话使用这个函数来提取元素
    def get_items_json(self, response):
        item = doubanfilmitem()
        items = json.loads(response.text)
        for temp in items:
            item["name"] = temp["title"]
            item["url"] = temp["url"]
            item["img"] = temp["cover_url"]
            item["crew"]  = "/".join(temp["actors"])
            types = "/".join(temp["types"])
            regions = "/".join(temp["regions"])
            item["misc"] = "/".join([temp["release_date"], regions, types])
            item["rating"] = temp["score"]
            item["comment_num"] = str(temp["vote_count"]) + "人评价"
            tmp = sys.stdout
            fp = open("output.txt", "a");
            sys.stdout = fp
            print "name: " + item["name"].encode('utf-8')
            print "url: " + item["url"].encode('utf-8')
            print "img_url: " + item["img"].encode('utf-8')
            print "rating: " + item["rating"].encode('utf-8')
            print "crew: " + item["crew"].encode('utf-8')
            print "misc: " + item["misc"].encode('utf-8')
            print "rating_num: " + item["comment_num"]
            sys.stdout = tmp
            fp.close()
            yield item

    # 使用selenium+PhantomJS解析页面的话使用这个函数来提取元素
    def get_items_phantomjs(self, response):
        htm = BeautifulSoup(response.text, "lxml").find('div', class_="movie-list-panel pictext")
        items = htm.find_all('div', class_="movie-content")
        for temp in items:
            item = doubanfilmitem()
            item['name'] = temp.find('span', class_='movie-name-text').a.get_text()
            item['url'] = str(temp.find('a')['href'])
            if temp.find('img') != None:
                item['img'] = str(temp.find('img')['data-original'])
            else:
                item['img'] = ""
            if temp.find('div', class_="movie-crew") != None:
                item['crew'] = temp.find('div', class_="movie-crew").get_text()
            else:
                item['crew'] = ""
            if temp.find('div', class_="movie-misc") != None:
                item['misc'] = temp.find('div', class_="movie-misc").get_text()
            else:
                item['misc'] = ""
            item['rating'] = temp.find('span', class_="rating_num").get_text()
            item['comment_num'] = temp.find('span', class_="comment-num").get_text()
            tmp = sys.stdout
            fp = open("output.txt", "a");
            sys.stdout = fp
            print item['name'].encode('utf-8')
            print item['url'].encode('utf-8')
            print item['img'].encode('utf-8')
            print item['crew'].encode('utf-8')
            print item['misc'].encode('utf-8')
            print item['rating'].encode('utf-8')
            print item['comment_num'].encode('utf-8')
            sys.stdout = tmp
            fp.close()
            yield item
