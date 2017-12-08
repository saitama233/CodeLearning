import random
from doubanfilm import settings
# from doubanfilm.db.db_helper import db_helper

'''
这个类主要用于产生随机代理
'''
class randomproxy(object):
    def __init__(self):
        pass
        # self.db_helper = db_helper()
        # self.count = self.db_helper.proxys.count()

    def process_request(self, request, spider):
        request.meta['proxy'] = settings.HTTP_PROXY
