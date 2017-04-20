# coding=utf-8
import urllib2
import codecs

class GetSourceCode:
    def __init__(self, *args):
        request = urllib2.Request("".join(args))
        response = urllib2.urlopen(request)
        self.content = response.read()
        # content = response.read().decode("utf-8")# 不能直接这样写，否则下面调用不了

    def __name__(self):
        print "Download the Source Code from a certain url"

    def writefile(self, *args):
        f = open("".join(args).decode("utf-8"), "w")
        f.write(self.content)

