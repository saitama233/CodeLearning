#include "apue.h"
#include "apue_db.h"
#include <fcntl.h>

int main(void)
{
    DBHANDLE    db;
    char *ptr;

    if ((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC, 
                    FILE_MODE)) == NULL)
        err_sys("db_open error");
    printf("db open ok\n");

    // 存储三条记录
    if (db_store(db, "Alpha", "data1", DB_INSERT) != 0)
        err_quit("db_store error for alpha");
    if (db_store(db, "Beta", "Data for beta", DB_INSERT) != 0)
        err_quit("db_store error for beta");
    if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
        err_quit("db_store error for gamma");
    
    // 搜索一条记录
    if ((ptr = db_fetch(db, "Alpha")) != NULL)
        printf("key Alpha: %s\n", ptr);
    else
        printf("key Alpha not found\n");
    
    // 删除Alpha
    if (db_delete(db, "Alpha") == -1)
        printf("key Alpha not found, can't be deleted\n");
    else
        printf("delete key Alpha ok\n");
    // 删除一条不存在的记录
    if (db_delete(db, "xxx") == -1)
        printf("key xxx not found, can't be deleted\n");
    else
        printf("delete key xxx ok\n");

    // 提取一条不存在的记录（刚删除）
    if ((ptr = db_fetch(db, "Alpha")) != NULL)
        printf("key Alpha: %s\n", ptr);
    else
        printf("key Alpha not found\n");

    if (db_store(db, "delta", "I am delta", DB_INSERT) != 0)
        err_quit("db_store error for delta for the first time");
    // 利用freelist中足够大的废弃字段
    if (db_store(db, "zeta1", "hello", DB_INSERT) != 0)
        err_quit("db_store error for zeta1");

    // 替换一条记录（数据长度不同）
    if (db_store(db, "delta", "I am delta1", DB_REPLACE) != 0)
        err_quit("db_store error for delta for the second time");
    // 替换一条记录（数据长度不同）
    if (db_store(db, "gamma", "record4", DB_REPLACE) != 0)
        err_quit("db_store error for delta for the second time");

    db_close(db);
    exit(0);
}
