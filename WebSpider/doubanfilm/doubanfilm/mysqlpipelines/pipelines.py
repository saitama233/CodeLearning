#coding=utf-8
import sys
from doubanfilm.items import doubanfilmitem
from .sql import sqlobj

class dbpipeline():
    count = 0
    @classmethod
    # 本来是想给item计数的，后来没有使用
    def countid(cls):
        cls.count += 1
        return cls.count
    def process_item(self, item, spider):
        if isinstance(item, doubanfilmitem):
            # 先查看item是否已经存储，没有的话进行存储
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
        # 之前调试用的，一般不会进入
        else:
            tmp = sys.stdout
            fp = open("output.txt", "a")
            sys.stdout = fp
            print '不是doubanfilmitem类型'
            sys.stdout = tmp
            fp.close()
