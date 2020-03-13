#include "public.h"
#include "Connection.h"

// 初始化数据库连接
Connection::Connection()
{
    _conn = mysql_init(nullptr);
}

// 释放数据库连接资源
Connection::~Connection()
{
    if (_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool Connection::connect(string ip,
                         unsigned short port,
                         string username,
                         string password,
                         string dbname)
{
    MYSQL *p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
                                  password.c_str(), dbname.c_str(),
                                  port, nullptr, 0);

    //mysql_query(_conn, "set interactive_timeout=24*3600");

    return p != nullptr;
}

// 更新操作 insert、delete、update
bool Connection::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("更新失败：" + sql + "\nmysql_error:" + mysql_error(_conn));
        return false;
    }
    return true;
}

// 查询操作 select
MYSQL_RES *Connection::query(string sql)
{
    // 查询操作 select
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("查询失败" + sql + "\nmysql_error:" + mysql_error(_conn));
        return nullptr;
    }
    return mysql_use_result(_conn);
}

// 刷新连接的起始空闲时刻
void Connection::refreshAliveTime()
{
    _alivetime = clock();
}

// 返回连接空闲的时长
clock_t Connection::getAliveTime()
{
    return clock() - _alivetime;
}

