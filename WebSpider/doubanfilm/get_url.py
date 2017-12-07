#coding=utf-8
import urllib2
import requests
import re

# 打印网页全部代码
# response = urllib2.urlopen("https://movie.douban.com/j/chart/top_list?type=24&interval_id=100:90&action=&start=0&limit=1000")
# print response.read()

# 提取json内容并逐条打印
# response = requests.get("https://movie.douban.com/j/chart/top_list?type=24&interval_id=100:90&action=&start=0&limit=1000")
'''
for item in response.json():
    print "name: " + item["title"]
    print "url: " + item["url"]
    print "img_url: " + item["cover_url"]
    print "rating: " + item["score"]
    print "crew: " + "/".join(item["actors"])
    types = "/".join(item["types"])
    regions = "/".join(item["regions"])
    print "misc: " + "/".join([item["release_date"], regions, types])
    print "rating_num: " + str(item["vote_count"])
'''

typeurl = "https://movie.douban.com//typerank?type_name=剧情&type=11&interval_id=100:90&action="
pattern = re.compile(r'type=(.*?)&interval')
jsonpage = re.findall(pattern, typeurl)
print jsonpage
print jsonpage[0]
