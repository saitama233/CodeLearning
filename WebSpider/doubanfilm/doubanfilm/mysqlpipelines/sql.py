import mysql.connector
from doubanfilm import settings

MYSQL_HOSTS = settings.MYSQL_HOSTS
MYSQL_USER = settings.MYSQL_USER
MYSQL_PASSWORD = settings.MYSQL_PASSWORD
MYSQL_PORT = settings.MYSQL_PORT
MYSQL_DB = settings.MYSQL_DB

cnx = mysql.connector.connect(user=MYSQL_USER, password=MYSQL_PASSWORD, 
                              host=MYSQL_HOSTS, database=MYSQL_DB)
cur = cnx.cursor(buffered=True)

class sqlobj():
    # 把item存储到数据库
    @classmethod
    def insert_douban_item(cls, name, url, img, crew, misc, 
                           rating, comment_num):
        insertsql = "INSERT INTO dbfilm_type (`name`, `url`, `img`, `crew`, `misc`, `rating`, `comment_num`) VALUES (%(name)s, %(url)s, %(img)s, %(crew)s, %(misc)s, %(rating)s, %(comment_num)s)"
        values = {
            'name': name, 
            'url': url, 
            'img': img, 
            'crew': crew, 
            'misc': misc, 
            'rating': rating, 
            'comment_num': comment_num,
        }
        cur.execute(insertsql, values)
        cnx.commit()

    # 检查item是否已经存在
    @classmethod
    def search_name(cls, url):
        sql = "SELECT EXISTS(SELECT 1 FROM dbfilm_type WHERE url=%(url)s)"
        item = {'url': url}
        cur.execute(sql, item)
        return cur.fetchall()[0]
