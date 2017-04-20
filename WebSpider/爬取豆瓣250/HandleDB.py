from __future__ import print_function
import mysql.connector
from mysql.connector import errorcode


class HandleDB:
    def __init__(self):
        self.config = {'user': 'root',
                       'password': '147258',
                       'host': '16q82555i1.iok.la',
                       'port': 3306}


class HandleDBdouban250(HandleDB):
    def __init__(self):
        HandleDB.__init__(self)
        self.DB_NAME = "SAITAMA"
        self.cnx = mysql.connector.connect(**self.config)
        self.cursor = self.cnx.cursor()
        self.choose_database()
        self.tables = {}
        self.tables['db250'] = ("CREATE TABLE `db250` ("
                                  "`num` int(3) NOT NULL AUTO_INCREMENT,"
                                  "`rank` int(3) NOT NULL,"
                                  "`link` varchar(55) NOT NULL,"
                                  "`image` varchar(88) NOT NULL,"
                                  "`name` varchar(20) NOT NULL,"
                                  "`info` varchar(300) NOT NULL,"
                                  "`rate` float(5) NOT NULL,"
                                  "`ratenum` varchar(30) NOT NULL,"
                                  "`evaluation` varchar(100) NOT NULL,"
                                  "PRIMARY KEY(`num`)"
                                  ") ENGINE=InnoDB")
        self.create_table()

    def choose_database(self):
        try:
            self.cnx.database = self.DB_NAME
        except mysql.connector.Error as err:
            if err.errno == errorcode.ER_BAD_DB_ERROR:
                self.create_database(self.cursor)
                self.cnx.database = self.DB_NAME

    def create_database(self, cursor):
        print("create database")
        try:
            # cursor.execute("CREATE DATABASE db250 DEFAULT CHARACTER SET 'utf8'")
            cursor.execute("CREATE DATABASE {} DEFAULT CHARACTER SET 'utf8'".format(self.DB_NAME))
        except mysql.connector.Error as err:
            print("Failed creating database:{}", format(err))
            exit(1)

    def create_table(self):
        for name, ddl in self.tables.iteritems():
            try:
                print("Creating table {}:".format(name),end='')
                self.cursor.execute(ddl)
            except mysql.connector.Error as err:
                if err.errno == errorcode.ER_TABLE_EXISTS_ERROR:
                    print("table {} already exists".format(name))
                else:
                    print(err.msg)
            else:
                print("OK")

    def insert_item(self, item):
        add_item = ("INSERT INTO db250"
                    "(rank,link,image,name,info,rate,ratenum,evaluation)"
                    " VALUES(%s,%s,%s,%s,%s,%s,%s,%s)")
        try:
            self.cursor.execute(add_item, item)
            self.cnx.commit()
            # print(item[0])
        except mysql.connector.Error as err:
            print(err.msg)






