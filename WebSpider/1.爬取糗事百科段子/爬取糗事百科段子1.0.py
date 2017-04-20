# coding=utf-8

import urllib
import urllib2
import re

for page in range(1, 5, 1):
    url = 'http://www.qiushibaike.com/hot/page/' + str(page)
    user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0'
    headers = {'User-Agent': user_agent}

    try:
        request = urllib2.Request(url, headers=headers)
        response = urllib2.urlopen(request)
        content = response.read().decode('utf-8')
        f = open(u'D:/chenxin/chenxin/PythonPrograms/Python基础教程/杂物/输出.txt', 'w+')
        f.write(content.encode("utf-8"))
        pattern = re.compile('<div.*?author.*?<h2>(.*?)</h2>.*?</div>' +
                             '.*?<div.*?content.*?<span>(.*?)</span>.*?</div>' +
                             '.*?<div.*?stats.*?<i.*?number">(.*?)</i>.*?</div>'
                             , re.S)
        items = re.findall(pattern, content)
        for item in items:
            if(int(item[2]) > 4000):
                print item[0], '\n', item[1], '\n', item[2], '\n'

    except urllib2.URLError, e:
        if hasattr(e, "code"):
            print e.code
        if hasattr(e, "reason"):
            print e.reason
