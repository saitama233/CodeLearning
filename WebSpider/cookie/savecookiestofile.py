import cookielib
import urllib2

filename = 'cookie.txt'
cookie = cookielib.MozillaCookieJar(filename)
handler = urllib2.HTTPCookieProcessor(cookie)
opener = urllib2.build_opener(handler)
response = opener.open("https://movie.douban.com/typerank?type_name=%E5%8E%86%E5%8F%B2&type=4&interval_id=100:90&action=")
cookie.save(ignore_discard=True, ignore_expires=True)
