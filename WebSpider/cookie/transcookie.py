#coding=utf-8

class transcookie:
    def __init__(self, cookie):
        self.cookie = cookie

    def stringtodict(self):
        '''
        将浏览器上Copy来的cookie字符串转化为Scrapy能使用的Dict
        '''
        itemdict = {}
        items = self.cookie.split(';')
        for item in items:
            key = item.split('=')[0].replace(' ','')
            value = item.split('=')[1]
            itemdict[key] = value
        return itemdict

if __name__ == "__main__":
     cookie = "_pk_id.100001.4cf6=bb5e1837bf02623e.1510210510.1.1510212279.1510210510.; _pk_ses.100001.4cf6=*; bid=rAWM3eO1Ams; __utma=30149280.623879687.1510210510.1510210510.1510210510.1; __utmb=30149280.0.10.1510210510; __utmc=30149280; __utmz=30149280.1510210510.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __utma=223695111.1984115178.1510210510.1510210510.1510210510.1; __utmb=223695111.0.10.1510210510; __utmc=223695111; __utmz=223695111.1510210510.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none)"
     trans = transcookie(cookie)
     print trans.stringtodict()
