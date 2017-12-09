# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

import scrapy

# 没有用到
class typepageurl(scrapy.Item):
    # typeurl
    url = scrapy.Field()

class doubanfilmitem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    # 名称
    name = scrapy.Field()
    # 图片
    img = scrapy.Field()
    # 演员
    crew = scrapy.Field()
    # 标签
    misc = scrapy.Field()
    # 分数
    rating = scrapy.Field()
    # 评价人数
    comment_num = scrapy.Field()
    # 链接
    url = scrapy.Field()
