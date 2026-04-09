#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include "connection.h" 
#include "connectionPool.h"
 

using namespace std;
int main()
{
    /*
    Connection conn;
	char sql[1024] = { 0 };
    //插入一条信息
    sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
	conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);
	conn.update(sql);
	return 0;
    */
    
#if 1
    //单线程无连接池
	clock_t begin = clock();
    //auto begin = std::chrono::steady_clock::now();
	for (int i = 0;i < 1000;++i)
	{
		Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
		conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);  
		conn.update(sql);
	}
	clock_t end = clock();
    //auto end = std::chrono::steady_clock::now();
	std::cout << (end - begin) << "ms" << std::endl;
    // std::chrono::duration<double> diff = end - begin;
    // std::cout << diff.count() << "s" << std::endl;
	/*1000：5217ms*/
	/*5000：27932ms*/
	/*10000：55293ms*/
	
	//单线程有连接池
	clock_t begin1 = clock();
    connectionPool *cp = connectionPool::getConnectionPool();
    for (int i = 0; i < 1000; ++i)
    {
		std::shared_ptr<Connection> sp=cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
		sp->update(sql);
	}
	clock_t end1 = clock();
	std::cout << (end1 - begin1) << "ms" << std::endl;
	
	/*1000：3412ms*/
	/*5000：16225ms*/
	/*10000：31749ms*/
#endif
#if 0
    //四线程有连接池
	//clock_t begin = clock();
    auto begin = std::chrono::steady_clock::now();
    std::cout << "start..." << std::endl;
	connectionPool* cp = connectionPool::getConnectionPool();
	std::thread t1([&]() {
		for (int i = 0;i < 2500;++i)
		{
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			sp->update(sql);
            std::cout << "thread 1 ..." << std::endl;
        }
		});
	std::thread t2([&]() {
		for (int i = 0;i < 2500;++i)
		{
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			sp->update(sql);
		}
		});
	std::thread t3([&]() {
		for (int i = 0;i < 2500;++i)
		{
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			sp->update(sql);
		}
		});
	std::thread t4([&]() {
		for (int i = 0;i < 2500;++i)
		{
			std::shared_ptr<Connection> sp = cp->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			sp->update(sql);
		}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//clock_t end = clock();
    auto end = std::chrono::steady_clock::now();
    auto diff = end - begin;
    std::cout << diff.count() << "ns" << std::endl;
    /*1000：2208ms*/
	/*5000：10748ms*/
	/*10000：20639ms*/
#endif
#if 0
    //四线程无连接池
	Connection conn;
	//clock_t begin = clock();
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto begin = std::chrono::steady_clock::now();
    conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);
    std::thread t1([&]() {
		for (int i = 0;i < 2500;++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306); 
			conn.update(sql);
		}
		});
	std::thread t2([&]() {
		for (int i = 0;i < 2500;++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);  
			conn.update(sql);
		}
		});
	std::thread t3([&]() {
		for (int i = 0;i < 2500;++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);  
			conn.update(sql);
		}
		});
	std::thread t4([&]() {
		for (int i = 0;i < 2500;++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", "root", "123456", "mysqlConnnectionPool", 3306);  
			conn.update(sql);
		}
		});
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//clock_t end = clock();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - begin;
    std::cout << diff.count() << "s" << std::endl;
#endif

    return 0;

}










































































































/*
#include <iostream>
#include <mysql/mysql.h>
using namespace std;

// int main() {
//     MYSQL *conn = mysql_init(nullptr);
//     if (conn == nullptr) {
//         cerr << "mysql_init() failed" << endl;
//         return 1;
//     }
//     cout << "MySQL library works!" << endl;
//     mysql_close(conn);
//     return 0;
// }

#include <stdio.h>
#include <mysql/mysql.h>

int main()
{
    // 1. 初始化连接环境
    MYSQL* mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        printf("mysql_init() error\n");
        return -1;
    }

    // 2. 连接数据库服务器
    mysql = mysql_real_connect(mysql, "localhost", "root", "123456", 
                               "game", 0, NULL, 0);
    if(mysql == NULL)
    {
        printf("mysql_real_connect() error\n");
        return -1;
    }

    // printf("mysql api使用的默认编码: %s\n", mysql_character_set_name(mysql));

    // // 设置编码为utf8
    // mysql_set_character_set(mysql, "utf8");

    // printf("mysql api使用的修改之后的编码: %s\n", mysql_character_set_name(mysql));

    // printf("恭喜, 连接数据库服务器成功了...\n");

    // 3. 执行一个sql语句
    // 查询scott数据库下的dept部门表
    const char* sql = "select * from player";
    // 执行这个sql语句
    int ret = mysql_query(mysql, sql);
    if(ret != 0)
    {
        printf("mysql_query() a失败了, 原因: %s\n", mysql_error(mysql));
        return -1;
    }

    // 4. 取出结果集
    MYSQL_RES* res = mysql_store_result(mysql);
    if(res == NULL)
    {
        printf("mysql_store_result() 失败了, 原因: %s\n", mysql_error(mysql));
        return -1;
    }

    // 5. 得到结果集中的列数
    int num = mysql_num_fields(res);

    // 6. 得到所有列的名字, 并且输出
    MYSQL_FIELD * fields = mysql_fetch_fields(res);
    for(int i=0; i<num; ++i)
    {
        printf("%s\t\t", fields[i].name);
    }
    printf("\n");

    // 7. 遍历结果集中所有的行
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(res)) != NULL)
    {
        // 将当前行中的每一列信息读出
        for(int i=0; i<num; ++i)
        {
            printf("%s\t\t", row[i]);
        }
        printf("\n");
    }

    // 8. 释放资源 - 结果集
    mysql_free_result(res);

    // 9. 写数据库
    // 以下三条是一个完整的操作, 对应的是一个事务
    // 设置事务为手动提交
    mysql_autocommit(mysql, 0); 
    int ret1 = mysql_query(mysql, "insert into player (name,sex) values('海军', '男')");
    int ret2 = mysql_query(mysql, "insert into player (name,sex) values('七武海', '男')");
    int ret3 = mysql_query(mysql, "insert into player (name,sex) values('女帝', '女')");
    printf("ret1 = %d, ret2 = %d, ret3 = %d\n", ret1, ret2, ret3);

    if(ret1==0 && ret2==0 && ret3==0)
    {
        // 提交事务
        mysql_commit(mysql);
    }
    else
    {
        mysql_rollback(mysql);
    }

    while( (row = mysql_fetch_row(res)) != NULL)
    {
        // 将当前行中的每一列信息读出
        for(int i=0; i<num; ++i)
        {
            printf("%s\t\t", row[i]);
        }
        printf("\n");
    }

    // 释放数据库资源
    mysql_close(mysql);
    
  return 0;
}

*/

