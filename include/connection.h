#ifndef CONNECTION_H
#define CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <chrono>

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

    //刷新一下连接的起始的空闲时间点
    void refreshConnectionTime() { _aliveTime = std::chrono::steady_clock::now();}

    //返回存活的时间 以秒为单位
    int getAliveTime() 
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        //auto durtime = now - _aliveTime;
        auto durtion = std::chrono::duration_cast<std::chrono::seconds>(now - _aliveTime);
        return durtion.count();
    }

private:
    MYSQL *_cnn; // 表示和MySQL Serve的一条连接
    std::chrono::steady_clock::time_point _aliveTime; //记录时间点               记录进入空闲状态后的存活时间点  通过记录时间点来获得  
};

#endif