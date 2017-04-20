#!/usr/bin/env python
#coding=utf-8
import time, uuid, functools, threading, logging

__author__ = 'Michael Liao'

'''
Database operation module.
'''

#Dict object:
class Dict(dict):
    '''
    Simple dict but support access as x.y style.
    >>> d1 = Dict()
    >>> d1['x'] = 100     #普通字典行为
    >>> d1.x              #重载了的行为，通过__getattr__
    100
    >>> d1.y = 200        #重载了的行为，通过__setattr__
    >>> d1['y']
    200
    >>> d3 = Dict(('a','b','c'),(1,2,3))    #这种是重载了的，普通字典没法
    >>> d3.a
    1
    >>> d3.b
    2
    '''
    def __init__(self, names=(), values=(), **kw): 
        #默认给names和values赋值为空元组
        super(Dict, self).__init__(**kw)
        for k, v in zip(names, values):
            self[k] = v
    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            raise AttributeError(r"'Dict' object has no attribute '%s'" % key)
    def __setattr__(self, key, value):
        self[key] = value

def next_id(t=None):
    '''
    用于生成一个唯一id，由当前时间+随机数（由SHA-1算法生成）拼接而成
    Return next id as 50-char string.
    Args:
        t: unix timestamp,default to None and using time.time().
    '''
    if t is None:
        t = time.time()
    # uuid是根据一些算法生成唯一标识符的模块
    return '%015d%s000' % (int(t * 1000), uuid.uuid4().hex)

def _profiling(start, sql=''):
    # 用于计算SQL运行的时间
    # t显示了SQL的运行时间
    t = time.time() - start
    # ？？？没看懂为什么运行时间大于0.1就要写警告
    if t > 0.1:
        logging.warning('[PROFILING] [DB] %s: %s' % (t, sql))
    else:
        logging.info('[FROFILING] [DB] %s: %s' % (t, sql))

class DBError(Exception):
    pass

class MulticolumnsError(DBError):
    pass

# object是所有类的基类:the most base type
class _LasyConnection(object):
    def __init__(self):
        self.connection = None

    def cursor(self):
        if self.connection is None:
            connection = engine.connect()
            logging.info('open connection <%s> ...' % hex(id(connection)))
            self.connection = connection
        return self.connection.cursor()
    
    def commit(self):
        self.connection.commit()

    def rollback(self):
        self.connection.rollback()

    def cleanup(self):
        if self.connection:
            connection = self.connection
            self.connection = None
            logging.info('close connection <%s> ...' % hex(id(connection)))
            connection.close()

# 相当于_DbCtx从threading.local继承，python一切皆对象，
# 从这个类派生，然后赋值给_db_ctx，说明_db_ctx具有threading.local的性质
class _DbCtx(threading.local):
    '''
    Thread local object that holds connection info.
    '''
    def __init__(self):
        self.connection = None
        self.transactions = 0

    def is_init(self):
        return not self.connection is None

    def init(self):
        logging.info('open lazy connection...')
        self.connection = _LasyConnection()
        self.transactions = 0

    def cleanup(self):
        self.connection.cleanup()
        self.connection = None

    def cursor(self):
        '''
        Return cursor
        '''
        return self.connection.cursor()

# thread-local db context:
_db_ctx = _DbCtx()

#global engine object:
# 之后会把_Engine类赋值给engine
engine = None

class _Engine(object):
    def __init__(self, connect):
        # 此时_connect = mysql.connector.connect(**parms)
        self._connect = connect
    def connect(self):
        # ？？？根据这看出来上面的self._connect应该代表一个函数，也就是说connect是一个函数
        return self._connect()

def create_engine(user, password, database, host='127.0.0.1', port=3306, **kw):
    import mysql.connector
    global engine
    if engine is not None:
        raise DBError('Engine is already initialized.')
    params = dict(user=user, password=password, database=database, 
                  host=host,port=port)
    defaults = dict(use_unicode=True, charset='utf8',
                   collation='utf8_general_ci', autocommit=False)
    for k, v in defaults.iteritems():
        params[k] = kw.pop(k, v)
    params.update(kw)
    params['buffered'] = True
    # ？？？下面的lambda表达式没有在网上找到用法，它后面还可以接函数吗
    engine = _Engine(lambda: mysql.connector.connect(**params))
    # test connection...
    # id函数返回对象的标识符，hex函数把数字转换成16进制
    logging.info('Init mysql engine <%s> ok.' % hex(id(engine)))

class _ConnectionCtx(object):
    '''
    _ConnectionCtx object that can open and close connection.
    _ConnectionCtx object can be nested and only the most outer connection
    has effect.
    with connection():
        pass
    '''
    def __enter__(self):
        global _db_ctx
        self.should_cleanup = False
        if not _db_ctx.is_init():
            _db_ctx.init()
            self.should_cleanup = True
        return self

    def __exit__(self, exctype, excvalue, traceback):
        global _db_ctx
        if self.should_cleanup:
            _db_ctx.cleanup()


def with_connection(func):
    '''
    Decorator for reuse connection.
    @with_connection
    def for(*args, **kw):
        f1()
    '''
    @functools.wraps(func)
    def _wrapper(*args, **kw):
        with _ConnectionCtx():
            return func(*args, **kw)
    return _wrapper

# 好像就是事务transaction，要么完全执行，要么完全不执行
class _TransactionCtx(object):
    '''
    _TransactionCtx object that can handle transactions.
    with _TransactionCtx():
        pass
    '''
    def __enter__(self):
        global _db_ctx
        self.should_close_conn = False
        if not _db_ctx.is_init():
            # needs open a connection first:
            _db_ctx.init()
            self.should_close_conn = True
        _db_ctx.transactions = _db_ctx.transactions + 1
        logging.info('begin transactions...' if _db_ctx.transactions==1 \
                      else 'join current transactions...') 
        return self

    def __exit__(self, exctype, exvalue, traceback):
        global _db_ctx
        _db_ctx.transactions = _db_ctx.transactions - 1
        try:
            if _db_ctx.transactions == 0:
                if exctype is None:
                    self.commit()
                else:
                    self.rollback()
        finally:
            if self.should_close_conn:
                _db_ctx.cleanup()

    def commit(self):
        global _db_ctx
        logging.info('commit transactions...')
        try:
            _db_ctx.connection.commit()
            logging.info('commit ok.')
        except:
            logging.warning('commit failed. try rollback...')
            _db_ctx.connection.rollback()
            logging.warning('rollback ok.')
            # 这一句表示传递异常
            raise

    def rollback(self):
        global _db_ctx
        logging.warning('rollback transaction...')
        _db_ctx.connection.rollback()
        logging.info('rollback ok.')

def transaction():
    '''
    Create a transaction object so can use with statement:
    with transaction():
        pass
    >>> def update_profile(id, name, rollback):
    ...     u = dict(id=id, name=name, email='%s@test.org' % name, \
                     passwd=name, last_modified=time.time())
    ...     insert('user', **u)
    ...     r = update('update user set passwd=? where id=?',  \
                       name.upper(), id) 
    ...     if rollback:
    ...         raise StandardError('will cause rollback...')
    >>> with transaction():
    ...     update_profile(900301, 'Python', False)
    >>> select_one('select * from user where id=?', 900301).name
    u'Python'
    >>> with transaction():
    ...     update_profile(900302, 'Ruby', True)
    Traceback (most recent call last):
      ...
    StandardError: will cause rollback...
    >>> select_one('select * from user where id=?', 900302)
    '''
    return _TransactionCtx()

def with_transaction(func):
    '''
    A decorator that makes function around transaction.
    >>> @with_transaction
    ... def update_profile(id, name, rollback):
    ...     u = dict(id=id, name=name, email='%s@test.org' % name, 
    ...                 passwd=name, last_modified=time.time())
    ...     insert('user', **u)
    ...     r = update('update user set passwd=? where id=?', 
    ...                 name.upper(), id) 
    ...     if rollback:
    ...         raise StandardError('will cause rollback...')
    >>> update_profile(8080, 'Julia', False)
    >>> select_one('select * from user where id=?', 8080).passwd
    u'JULIA'
    >>> update_profile(9090, 'Robert', True)
    Traceback (most recent call last):
      ...
    StandardError: will cause rollback...
    >>> select_one('select * from user where id=?', 9090)
    '''
    @functools.wraps(func)
    def _wrapper(*args, **kw):
        _start = time.time()
        with _TransactionCtx():
            return func(*args, **kw)
        _profiling(_start)
    return _wrapper

def _select(sql, first, *args):
    'execute select SQL and return unique result or list results.'
    global _db_ctx
    cursor = None
    sql = sql.replace('?', '%s')
    logging.info('SQL: %s, ARGS: %s' % (sql, args))
    try:
        cursor = _db_ctx.connection.cursor()
        cursor.execute(sql, args)
        # cursor.description返回表都有哪些字段
        if cursor.description:
            # 这段代码实际上把所有的字段名保存在了names里
            names = [x[0] for x in cursor.description]
        if first:
            values = cursor.fetchone()
            if not values:
                return None
            # 这种初始化方法dict里没有,是Dict专有的
            return Dict(names, values)
        return [Dict(names, x) for x in cursor.fetchall()]
    finally:
        if cursor:
            cursor.close()

@with_connection
def select_one(sql, *args):
    '''
    Execute select SQL and expected one result.
    If no result found, reurn None.
    If multiple results found, the first one returned.
    >>> n = update('delete from user')
    >>> u1 = dict(id=100, name='Alice', email='alice@test.org', \
                  passwd='A-12345', last_modified=time.time())
    >>> u2 = dict(id=101, name='Sarah', email='sarah@test.org', \
                  passwd='A-12345', last_modified=time.time())
    >>> insert('user', **u1)
    1
    >>> insert('user', **u2)
    1
    >>> u = select_one('select * from user where id=?', 100)
    >>> u.name
    u'Alice'
    >>> select_one('select * from user where email=?', 'abc@email.com')
    >>> u2 = select_one('select * from user where passwd=? order by email'\
                        , 'A-12345')
    >>> u2.name
    u'Alice'
    '''
    return _select(sql, True, *args)

@with_connection
def select_int(sql, *args):
    '''
    Execute select SQL and expected one int and only one int result.
    >>> n = update('delete from user')
    >>> u1 = dict(id=96900, name='Ada', email='ada@test.org', \
                  passwd='A-12345', last_modified=time.time())
    >>> u2 = dict(id=96901, name='Adam', email='adam@test.org', \
                  passwd='A-12345', last_modified=time.time())
    >>> insert('user', **u1)
    1
    >>> insert('user', **u2)
    1
    >>> select_int('select count(*) from user')
    2
    >>> select_int('select id from user where email=?', 'ada@test.org')
    96900
    '''
    d = _select(sql, True, *args)
    if len(d)!=1:
        raise MulticolumnsError('Expect only one column.')
    # 实际上这只是返回那条数据的第一个值，也就是id
    return d.values()[0]

@with_connection
def select(sql, *args):
    '''
    Execute select SQL and return list or empty list if no result.
    >>> n = update('delete from user')
    >>> u1 = dict(id=200, name='Wall.E', email='wall.e@test.org', \
        passwd='back-to-earth', last_modified=time.time())
    >>> u2 = dict(id=201, name='Eva', email='eva@test.org', \
        passwd='back-to-earth', last_modified=time.time())
    >>> insert('user', **u1)
    1
    >>> insert('user', **u2)
    1
    >>> L = select('select * from user where id=?', 900900900)
    >>> L
    []
    >>> L = select('select * from user where id=?', 200)
    >>> L[0].name
    u'Wall.E'
    >>> L = select('select * from user where passwd=? order by id desc',
    ...             'back-to-earth')
    >>> L[0].name
    u'Eva'
    >>> L[1].name
    u'Wall.E'
    '''
    return _select(sql, False, *args)

def insert(table, **kw):
    '''
    Execute insert SQL.
    >>> u1 = dict(id=2000, name='Jesse', email='Jesse@test.org',
    ... passwd='back-to-earth', last_modified=time.time())
    >>> insert('user', **u1)
    1
    >>> u2 = select_one('select * from user where id=?', 2000)
    >>> type(u2)
    <class '__main__.Dict'>
    >>> u2.name
    u'Jesse'
    >>> insert('user', **u2)
    Traceback (most recent call last):
      ...
    IntegrityError: 1062(23000): Duplicate entry '2000' for key 'PRIMARY'
    '''
    cols, args = zip(*kw.iteritems())
    # 下面的句子看懂了，但为什么会用``始终没看懂
    # 这里``的用法和repr相同，参考python基础教程1.11.3
    sql = 'insert into `%s` (%s) values (%s)' % (table,  \
            ','.join(['`%s`' % col for col in cols]),   \
            ','.join(['?' for i in range(len(cols))]))
    return _update(sql, *args)

@with_connection
def _update(sql, *args):
    global _db_ctx
    cursor = None
    sql = sql.replace('?', '%s')
    logging.info('SQL:%s, ARGS: %s' % (sql, args))
    try:
        cursor = _db_ctx.connection.cursor()
        cursor.execute(sql, args)
        r = cursor.rowcount
        if _db_ctx.transactions==0:
            # no transaction environment:
            logging.info('auto commit')
            _db_ctx.connection.commit()
        return r
    finally:
        if cursor:
            cursor.close()

def update(sql, *args):
    r'''
    Execute update SQL.
    >>> u1 = dict(id=1000, name='Michael', email='michael@test.org',
    ...           passwd='123456', last_modified=time.time())
    >>> insert('user', ** u1)
    1
    >>> u2 = select_one('select * from user where id=?', 1000)
    >>> u2.email
    u'michael@test.org'
    >>> u2.passwd
    u'123456'
    >>> update('update user set passwd=? where where id=?',
    ...         '***', 123)
    0
    '''
    return _update(sql, *args)

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    create_engine('root', '147258', 'test')
    update('drop table if exists user')
    update('create table user(id int primary key, name text, \
           email text, passwd text, last_modified real)')
    import doctest
    doctest.testmod()
