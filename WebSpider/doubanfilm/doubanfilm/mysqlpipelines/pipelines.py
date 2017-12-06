#coding=utf-8
import sys
from doubanfilm.items import doubanfilmitem
from .sql import sqlobj

class dbpipeline():
    count = 0
    @classmethod
    def countid(cls):
        cls.count += 1
        return cls.count
    def process_item(self, item, spider):
        if isinstance(item, doubanfilmitem):
            url = item['url']
            ret = sqlobj.search_name(url)
            if ret[0] == 1:
                tmp = sys.stdout
                fp = open("output.txt", "a")
                sys.stdout = fp
                print '已经存在了'
                sys.stdout = tmp
                fp.close()
                pass
            else:
                name = item['name']
                url = item['url']
                img = item['img']
                crew = item['crew']
                misc = item['misc']
                rating = item['rating']
                comment_num = item['comment_num']
                tmp = sys.stdout
                fp = open("output.txt", "a")
                sys.stdout = fp
                print '开始存储item'
                sys.stdout = tmp
                fp.close()
                sqlobj.insert_douban_item(name, url, img, crew, misc, rating, comment_num)
                tmp = sys.stdout
                fp = open("output.txt", "a")
                sys.stdout = fp
                print '存储完成item'
                sys.stdout = tmp
                fp.close()
        else:
            tmp = sys.stdout
            fp = open("output.txt", "a")
            sys.stdout = fp
            print '不是doubanfilmitem类型'
            sys.stdout = tmp
            fp.close()
