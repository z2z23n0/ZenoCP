#include "../Connection.h"
#include "../CommonConnectionPool.h"
#include <iostream>

using namespace std;

// 压力测试
const int n = 10000;

// 下面的代码用于测试单线程不使用连接池插入n条数据所用时间
#if 0
int main()
{
    clock_t begin = clock();


    for(int i = 0; i < n; ++i)
    {
        Connection conn; // 创建连接的代码一定要放在循环内部

        string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

        conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

        conn.update(sql);
    }

    clock_t  end = clock();
    cout << (end - begin) << "ms" << endl;

    return 0;
}
#endif

#if 0
// 下面的代码用于测试单线程使用连接池插入n条数据所用时间
int main()
{
    clock_t begin = clock();

    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    for (int i = 0; i < n; ++i)
    {
        shared_ptr<Connection> sp = cp->getConnection();

        string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

        sp->update(sql);
    }

    clock_t end = clock();
    cout << (end - begin) << "ms" << endl;

    return 0;
}
#endif

#if 1
// 下面的代码用于测试多线程不使用连接池插入n条数据所用时间
int main()
{
    Connection conn;
    conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");
    clock_t begin = clock();

    thread t1([]()
              {
                  for(int i = 0; i < n / 4; ++i)
                  {
                      Connection conn;

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

                      conn.update(sql);

                  }
              });
    thread t2([]()
              {
                  for(int i = 0; i < n / 4; ++i)
                  {
                      Connection conn;

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

                      conn.update(sql);
                  }
              });
    thread t3([]()
              {
                  for(int i = 0; i < n / 4; ++i)
                  {
                      Connection conn;

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

                      conn.update(sql);
                  }
              });
    thread t4([]()
              {
                  for(int i = 0; i < n / 4; ++i)
                  {
                      Connection conn;

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

                      conn.update(sql);
                  }
              });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    clock_t end = clock();
    cout << (end - begin) << "ms" << endl;

    return 0;
}
#endif

#if 0
// 下面的代码用于测试多线程使用连接池插入n条数据所用时间
int main()
{
    clock_t begin = clock();

    thread t1([]()
              {
                  ConnectionPool *cp = ConnectionPool::getConnectionPool();
                  for(int i = 0; i < n / 4; ++i)
                  {
                      shared_ptr<Connection> sp = cp->getConnection();

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      sp->update(sql);
                  }
              });
    thread t2([]()
              {
                  ConnectionPool *cp = ConnectionPool::getConnectionPool();
                  for(int i = 0; i < n / 4; ++i)
                  {
                      shared_ptr<Connection> sp = cp->getConnection();

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      sp->update(sql);
                  }
              });
    thread t3([]()
              {
                  ConnectionPool *cp = ConnectionPool::getConnectionPool();
                  for(int i = 0; i < n / 4; ++i)
                  {
                      shared_ptr<Connection> sp = cp->getConnection();

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      sp->update(sql);
                  }
              });
    thread t4([]()
              {
                  ConnectionPool *cp = ConnectionPool::getConnectionPool();
                  for(int i = 0; i < n / 4; ++i)
                  {
                      shared_ptr<Connection> sp = cp->getConnection();

                      string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

                      sp->update(sql);
                  }
              });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    clock_t end = clock();
    cout << (end - begin) << "ms" << endl;

    return 0;
}
#endif