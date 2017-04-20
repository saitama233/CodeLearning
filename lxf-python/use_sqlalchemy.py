# coding=utf-8
from sqlalchemy import Column,String,create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

class User(Base):
    # 表名字
    __tablename__ = 'user'
    # 表结构
    id = Column(String(20),primary_key=True)
    name = Column(String(20))

#初始化数据库连接:
engine = create_engine('mysql+mysqlconnector://root:147258@localhost:3306/cx')
#创建DBsESSION类型:
DBSession = sessionmaker(bind=engine)

#创建session对象：
session = DBSession()

#创建新User对象：
new_user = User(id='5',name='jesse')
#添加到session：
# session.add(new_user)
#提交保存到数据库：
# session.commit()

user = session.query(User).filter(User.id=='5').one()
#打印类型和对象的name属性
print 'type:',type(user)
print 'name:',user.name

#关闭session:
session.close()
