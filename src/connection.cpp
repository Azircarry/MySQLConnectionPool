#include "connection.h"
#include "public.h"
//初始化数据库
Connection::Connection()
{
    _cnn = mysql_init(nullptr);
}

//释放数据库连接资源
Connection::~Connection()
{
    //void mysql_close(MYSQL *sock);
    if( _cnn != nullptr)
    {
        mysql_close(_cnn);
    }
}

//连接数据库
bool Connection::connect(std::string host,
                         std::string user, std::string passwd,
                         std::string db, unsigned int port)
{
    MYSQL* p = mysql_real_connect(_cnn, host.c_str(),
                                  user.c_str(), passwd.c_str(),
                                  db.c_str(), port,
                                  NULL,0);
    // if(p == nullptr)
    //     return false;
    // return true;
    return p != nullptr;
}

//数据库更新操作(增、删、改)insert drop update
bool Connection::update(const std::string sql)
{
    //int res = mysql_query(_cnn, sql.c_str());
    if(mysql_query(_cnn, sql.c_str()))
    {
        LOG("更新失败:" + sql);
        return false;
    }
    return true;
}

//数据库查询操作 select
MYSQL_RES* Connection::query(const std::string sql)
{
        if(mysql_query(_cnn, sql.c_str()))
    {
        LOG("更新失败:" + sql);
        return nullptr;
    }
    return mysql_use_result(_cnn);
}