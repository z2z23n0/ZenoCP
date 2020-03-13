#pragma once

#include <string>
#include <mysql.h>
#include <ctime>

using namespace std;

class Connection
{
public:
    // 初始化数据库连接
    Connection();

    // 释放数据库连接资源
    ~Connection();

    // 连接数据库
    bool connect(string ip,
                 unsigned short port,
                 string username,
                 string password,
                 string dbname);

    // 更新操作 insert、delete、update
    bool update(string sql);

    // 查询操作 select
    MYSQL_RES *query(string sql);

    // 刷新连接的起始空闲时刻
    void refreshAliveTime();

    // 返回连接空闲的时长
    clock_t getAliveTime();

private:
    MYSQL *_conn; // 表示和MySQL Server的一条连接
    clock_t _alivetime; // 记录进入空闲状态后的起始存活时刻（即在队列中出现的时刻）
};