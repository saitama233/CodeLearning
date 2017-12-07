#coding=utf-8
import random
'''
这个类主要用于产生随机UserAgent
'''

class random_user_agent(object):
    def __init__(self, agents):
        self.agents = agents

    @classmethod
    def from_crawler(cls, crawler):
        return cls(crawler.settings.getlist('USER_AGENT'))

    def process_request(self, request, spider):
        request.headers.setdefault('User-Agent', random.choice(self.agents))
