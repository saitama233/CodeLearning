#coding=utf-8
import sys
from selenium import webdriver
from scrapy.http import HtmlResponse
import time
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.keys import Keys

# 都是测试滚动的JavaScript，不过只有jsb配合循环能用
jsp = """
function scrollToBottom() {
    var Height = document.body.clientHeight,
    screenHeight = window.innerHeight,
    INTERVAL = 100,
    delta = 500,
    curScrollTop = 0;
    var scroll = function() {
        curScrollTop += delta;
        window.scrollTo(0, curScrollTop);
    };
    var timer = setInterval(function() {
        scroll();
        if (curScrollTop >= document.body.clientHeight) {
            clearInterval(timer);
        }
    }, INTERVAL)
}
scrollToBottom();
"""

jsb = """
    window.scrollTo(0, document.body.clientHeight)
"""

js = """
    var q=document.documentElement.scrollTop=100000
"""

jstest = """
    document.write(document.body.scrollTop + "br");
    document.write(document.body.clientHeight + "br");
    document.write(document.body.innerHeight + "br");
"""

class PhantomJSMiddleware(object):
    @classmethod
    def process_request(cls, request, spider):
        if request.meta.has_key('PhantomJS'):
            # dcap = dict(DesiredCapabilities.PHANTOMJS)
            # dcap["phantomjs.page.settings.userAgent"] = (
            #    "Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)",
            # )
            driver = webdriver.PhantomJS()#desired_capabilities=dcap)
            driver.get(request.url)
            #  driver.save_screenshot("douban.png")
            # action = ActionChains(driver)
            # action.click()
            # element = driver.find_element_by_tag_name("body")
            # 让页面滚动到最下方，否则一些信息无法加载，方法不太好
            for i in range(100):
                # element.send_keys(Keys.PAGE_DOWN)
                # action.send_keys(Keys.ARROW_DOWN)
                driver.execute_script(jsb)
                time.sleep(1)
            time.sleep(2)
            # driver.save_screenshot("newdouban.png")
            content = driver.page_source.encode('utf-8')
            '''
            tmp = sys.stdout
            fp = open("output.txt", "a")
            sys.stdout = fp
            print '进入selenium'
            print content
            sys.stdout = tmp
            fp.close()
            '''
            # print driver.page_source
            driver.quit()
            return HtmlResponse(request.url, encoding='utf-8',
                               body=content, request=request)
