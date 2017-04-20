#coding=utf-8
import pymongo
from pymongo import MongoClient
import time

# conn = MongoClient('mongodb://cc:111@127.0.0.1:27017/test')
conn = pymongo.MongoClient("127.0.0.1", 27017)
db = conn.get_database("test")
db.authenticate('cc', '111')
# print conn.database_names()
print db.collection_names()
account = db.get_collection("records")


item = {"id":1,
        "author":"jesse",
        "text":"first item",
        "tags":["linux","vim","gdb","gdb"],
        "date":time.strftime('%Y-%d %H:%M:%S')}

account.insert(item)
# print account.find_one()
print account.find_one().keys()
for i in account.find():
    print i

#多条重复记录时只修改了第一个
account.update({"author":"jesse"}, {"$set":{"text":"I love python"}})
account.update({"author":"jesse"}, {"$set":{"text":"I love python","time":"20170417"}})

print account.find().count()
for i in account.find({"id":1}):
    print i

account.remove({u"id":1})
# account.remove()
