#coding=utf8

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
    headers = {
        'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0'
    }

    def start_requests(self):
        request = Request(url=self.url, headers=self.headers, callback=self.gettype_url)
        yield request

    def gettype_url(self, response):
        dbitem = typepageurl()
        htm = BeautifulSoup(response.text, "lxml")
        urls = htm.find('div', class_="types").find_all('a')
        # for url in urls:
        typeurl = self.base_url + urls[0]['href'] 
            # print typeurl
        request = Request(url=typeurl, headers=self.headers, callback=self.get_items)
        request.meta['PhantomJS'] = True
        yield request

    def get_items(self, response):
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
