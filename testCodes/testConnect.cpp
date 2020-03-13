#include <iostream>
#include "../Connection.h"

using namespace std;

// 用于测试数据库的连接、更新操作

int main()
{

    Connection conn;

    string sql = "insert into t_user (name,age,sex) values ('Tom', 20, 'male');";

    conn.connect("127.0.0.1", 3306, "root", "zyz3426237", "user");

    conn.update(sql);

    return 0;
}
