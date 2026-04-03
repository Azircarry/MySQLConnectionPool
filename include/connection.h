#ifndef CONNECTION_H
#define CONNECTION_H

#include <mysql/mysql.h>
#include <string>

//MySQL数据库操作类
//主要通过调用API实现数据库的增删改查操作
class Connection
{
public:
    //初始化数据库连接
    Connection();
    //释放数据库连接资源
    ~Connection();
    //连接数据库
    bool connect(std::string host,
                 std::string user, std::string passwd,
                 std::string db, unsigned int port);
    // MYSQL *STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
    //                               const char *user, const char *passwd,
    //                               const char *db, unsigned int port,
    //                               const char *unix_socket,
    //                               unsigned long clientflag);

    //数据库更新操作(增、删、改)insert drop update
    bool update(const std::string sql);
    //int STDCALL mysql_query(MYSQL *mysql, const char *q);

    //数据库查询操作 select
    MYSQL_RES* query(const std::string sql);

private:
    MYSQL *_cnn;   //
};

#endif