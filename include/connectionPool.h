/*
初始连接量(initSize)
最大连接量(maxSize)
最大空闲时间(maxIdleTime)
连接超时时间(connectionTimeout)
单例模式
*/
//#pragma once
#ifndef COMMECTIONPOOL_H
#define COMMECTIONPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>

#include "connection.h"


class connectionPool
{
public:
    //线程安全的懒汉式单例模式
    static connectionPool* getConnectionPool();    //用于返回该类唯一实例对象 的 函数接口

    //给外部提供接口 从连接池中获取一个可用的空闲连接  消费者
    //使用智能指针+自定义删除器来让释放的连接回到数据库连接池 
    //而不是重新定义一个backConnection函数，还要去调用这个来返回用完的连接，太麻烦了
    //Connection* getConnection(); 
    std::shared_ptr<Connection> getConnection();

    

private:
    //单例模式 构造函数私有化
    connectionPool();

    //加载配置文件
    bool loadConfigFile();

    //生产新的数据库服务器连接
    void produceConnectionTask();

    //扫描 超过maxIdleTime时间 的 空闲连接 ，将多余的空闲连接进行回收
    void scannerConnectionTask();

    // 登录数据库所需要的基本信息 根据下面的函数所需的参数对基本信息进行定义即可
    //  bool connect(std::string host,
    //               std::string user, std::string passwd,
    //               std::string db, unsigned int port);
    std::string _ip;   //MySQL服务器ip地址
    std::string _username; //MySQL登录用户名称  
    std::string _password; //登录密码
    std::string _dbname;   //数据库名称
    unsigned _port; // MySQL服务器端口

    int _initSize;  //连接池初始连接量
    int _maxSize;   //最大连接量
    int _maxIdleTime; //最大空闲时间
    int _connectionTimeout; //连接超时时间

    std::queue<Connection*> _connectionQue; // MySQL服务器连接 队列
    std::mutex _queueMtx;  //维护 连接队列 的线程互斥锁
    std::atomic_int _connectionCnt;  //记录所创建的connection连接的总数量  C++中提供的queue容器有size函数可以获取其大小，但其不保证线程安全，所以重新定义一个原子量表示连接队列中的元素个数
    std::condition_variable cv;  //用于生产者和消费者之间通信的条件变量

    //删除拷贝构造函数和赋值重载函数
    connectionPool(const connectionPool &) = delete;
    connectionPool& operator=(const connectionPool &) = delete;
};


#endif
