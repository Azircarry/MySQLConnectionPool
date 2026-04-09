#include "connectionPool.h"
#include "public.h"
#include <string>
//线程安全的懒汉式单例模式
connectionPool *connectionPool::getConnectionPool()    //用于返回该类唯一实例对象 的 函数接口
{
    static connectionPool pool;
    return &pool;
}

//单例模式 构造函数私有化
connectionPool::connectionPool()
{
    //加载配置项
    if( !loadConfigFile() )
        return;

    //创建初始数量的连接
    for (int i = 0; i < _initSize; i++)
    {
        Connection *p = new Connection();
        p->connect(_ip, _username, _password, _dbname, _port);
        _connectionQue.push(p);
        _connectionCnt++;
    }

    //启动一个新的线程，作为 连接的生产者
    std::thread produceConnection(std::bind(&connectionPool::produceConnectionTask,this));
    produceConnection.detach();

    //启动一个新的线程  用于扫描 超过maxIdleTime时间的空闲连接 ，将多余的空闲连接进行回收
    std::thread scannerConnection(std::bind(&connectionPool::scannerConnectionTask,this));
    scannerConnection.detach();
}

//加载配置文件
bool connectionPool::loadConfigFile()
{
    FILE *pf = fopen("mysql.ini", "r");
    if(pf == nullptr)
    {
        LOG("mysql.ini is not exist!");
        return false;
    }

    //feof 到达文件末尾时，返回一个非零值
    while( !feof(pf) )
    {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        std::string str = line;
        int idx = str.find('=', 0);

        if( idx == -1)
            continue;

        int endidx = str.find('\n', idx);

        std::string key = str.substr(0, idx);
        std::string value = str.substr(idx + 1, endidx - idx - 1);

        if(key == "ip")
            _ip = value;
        else if(key == "username")
            _username = value;
        else if(key == "password")
            _password = value;
        else if(key == "dbname")
            _dbname = value;
        else if(key == "port")
            _port = std::atoi(value.c_str());  //atoi将字符串转换为int  ASCII->int
        else if(key == "initSize")
            _initSize = std::atoi(value.c_str());
        else if(key == "maxSize")
            _maxSize = std::atoi(value.c_str());
        else if(key == "maxIdleTime")
            _maxIdleTime = std::atoi(value.c_str());
        else if(key == "connectionTimeout")
            _connectionTimeout = std::atoi(value.c_str());
    }
    return true;
}


//专门生产新的数据库服务器连接
void connectionPool::produceConnectionTask()
{
    for (;;)
    {
        std::unique_lock<std::mutex> lck(_queueMtx);

        cv.wait(lck, [&]() -> bool
                { return _connectionQue.empty(); });

        if( _connectionCnt < _maxSize)
        {
            Connection *cp = new Connection();
            cp->connect(_ip, _username, _password, _dbname, _port);
            _connectionQue.push(cp);
            _connectionCnt++;
        }

        cv.notify_all();
    }

}


//给外部提供接口 从连接池中获取一个可用的空闲连接  消费者
//使用【智能指针】+【自定义删除器】来让释放的连接回到数据库连接池 
std::shared_ptr<Connection> connectionPool::getConnection()
{
    std::unique_lock<std::mutex> lck(_queueMtx);
    /*
    最基本的流程  很多细节都没有考虑
    首先应该考虑连接超时的问题  【向连接队列中取一个可用连接时，发现没有可用的连接，应该等待一段时间看是否有空闲连接返回连接队列中，如果超过这段时间(连接超时时间)，依然没有可用连接，则应该返回连接失败信息】
    其次使用完连接之后，应该将连接放回连接队列中，而不能直接将其销毁 【智能指针默认的析构函数会直接销毁所用的连接，而不是将其放回连接队列中，因此需要自定义删除器】 
    cv.wait(lck, [&]() -> bool
            { return !_conncetionQue.empty(); });
    std::shared_ptr<Connection> sp = std::make_shared<Connection>(_connectionQue.front());
    _conncetionQue.pop();
    _conncetionCnt--;
    cv.notify_all();
    */

    // cv.wait_for(lck, std::chrono::milliseconds(_connectionTimeout), [&]() -> bool
    //             { return !_conncetionQue.empty(); });
    if(!cv.wait_for(lck, std::chrono::milliseconds(_connectionTimeout), 
        [&]() -> bool{ 
            return !_connectionQue.empty(); 
        }))
    {
        LOG("连接超时... 连接失败！");
        return nullptr;
    }

    /*shared_ptr析构会调用Connection析构，直接delete了，会关闭连接
	而我们要归还资源，让连接回到队列，所以要自定义删除器*/
    std::shared_ptr<Connection> sp(_connectionQue.front(),
        [&](Connection *pcon) -> void
        {
            std::unique_lock<std::mutex> lck(_queueMtx);
            pcon->refreshConnectionTime();
            _connectionQue.push(pcon);
        });

    _connectionQue.pop();
    _connectionCnt--;
    cv.notify_all();
    return sp;
}


//扫描 超过maxIdleTime时间 的 空闲连接 ，将多余的空闲连接进行回收
void connectionPool::scannerConnectionTask()
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::seconds(_maxIdleTime));

        std::unique_lock<std::mutex> lck(_queueMtx);
        //对连接队列进行扫描  
        while(_connectionCnt > _initSize)  //不能把初始的连接也回收掉
        {
            Connection *p = _connectionQue.front();
            if(p->getAliveTime() > _maxIdleTime)  //将每一个连接的空闲时间与_maxIdleTime进行比较
            {
                _connectionQue.pop();
                _connectionCnt--;
                delete p;
            }
            else
            {
                //队头的连接没有超过_maxIdleTime,其他连接肯定没有
                break;  
            }
        }
    }
}







