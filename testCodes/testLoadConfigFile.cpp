#include "../Connection.h"
#include "../CommonConnectionPool.h"
#include <iostream>

using namespace std;

// 用于测试对mysql.ini配置文件的读取
// 注：运行此测试代码需要先将ConnectionPool类中下面代码用到的方法与属性设置成public

int main()
{
    ConnectionPool *cp = ConnectionPool::getConnectionPool();

    cp->loadConfigFile();

    cout << cp->_ip << endl;
    cout << cp->_port << endl;
    cout << cp->_username << endl;
    cout << cp->_password << endl;
    cout << cp->_dbname << endl;
    cout << cp->_initSize << endl;
    cout << cp->_maxSize << endl;
    cout << cp->_maxIdleTime << endl;
    cout << cp->_connectionTimeout << endl;

    return 0;
}