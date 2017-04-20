from DownloadSourceCode import GetSourceCode
dsc = GetSourceCode("http://www.baidu.com")
dsc.writefile(r"123.txt".decode("utf-8"))

