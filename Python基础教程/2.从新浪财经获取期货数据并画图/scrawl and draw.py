# coding=utf-8
import urllib2
import numpy as np
import pandas as pd
from pandas import Series, DataFrame
import matplotlib.pyplot as plt
import re

class GetSourceCode:
    def __init__(self, *args):
        record = []
        row = 1
        request = urllib2.Request("".join(args))
        response = urllib2.urlopen(request)
        self.content = response.read()
        print self.content

        pattern = re.compile(r'(\["(.*?)","(.*?)","(.*?)","(.*?)","(.*?)","(.*?)"\])')

        items = re.findall(pattern, self.content)
        for item in items:
            temp = []
            temp.append(item[1])
            for col in range(2, 7, 1):
                temp.append(float(item[col]))
            record.append(temp)

        data = DataFrame(record, columns=["date", "open", "high", "low", "close", "volume"])
        # print data
        data.index = pd.to_datetime(data.pop("date"))
        # print data
        # fig = plt.figure()
        data['close'].plot()
        plt.show()

    def __name__(self):
        print "Download the Source Code from a certain url"

    def writefile(self, *args):
        f = open("".join(args).decode("utf-8"), "w")
        f.write(self.content)


url = "http://stock2.finance.sina.com.cn/futures/api/json.php/" \
      "IndexService.getInnerFuturesDailyKLine?symbol=M0"
gsc = GetSourceCode(url)
# 数据格式：日期，开盘，最高，最低，收盘，总手数
gsc.writefile("123.txt")