#include "CommonConnectionPool.h"
#include "public.h"

// 线程安全的懒汉单例函数接口
ConnectionPool *ConnectionPool::getConnectionPool()
{
    // 对于静态局部变量的初始化，编译器自动进行lock和unlock
    static ConnectionPool pool;
    return &pool;
}

// 从配置文件中加载配置项
bool ConnectionPool::loadConfigFile()
{
    FILE *pf = fopen("mysql.ini", "r");
    if (pf == nullptr)
    {
        LOG("File 'mysql.ini' is not existing!");
        return false;
    }

    // 逐行处理配置文件中的配置字符串
    while (!feof(pf))
    {
        // 配置字符串举例：username=root\n

        // 从文件中获取一行配置字符串
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;

        // 找到配置字符串中的'='
        int idx = str.find('=', 0);

        // 无效的配置项
        if (idx == -1)
        {
            // 当配置字符串中找不到'='时说明该配置字符串有问题或者是注释，将其忽略
            continue;
        }

        // 分别取出该行配置中的key和value
        int endIdx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endIdx - idx - 1);

        if (key == "ip")
        {
            _ip = value;
        } else if (key == "port")
        {
            _port = atoi(value.c_str());
        } else if (key == "username")
        {
            _username = value;
        } else if (key == "password")
        {
            _password = value;
        } else if (key == "dbname")
        {
            _dbname = value;
        } else if (key == "maxSize")
        {
            _maxSize = atoi(value.c_str());
        } else if (key == "maxIdleTime")
        {
            _maxIdleTime = atoi(value.c_str());
        } else if (key == "connectionTimeout")
        {
            _connectionTimeout = atoi(value.c_str());
        }
    }

    return true;
}

// 连接池的构造函数
ConnectionPool::ConnectionPool()
{
    // 加载配置项
    if (!loadConfigFile())
    {
        return;
    }

    // 创建初始数量的连接
    for (int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _port, _username, _password, _dbname);
        p->refreshAliveTime(); // 记录连接的起始空闲时刻
        _connectionQue.push(p);
        _connectionCnt++;
    }

    // 启动一个新的线程，作为连接的生产者
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();

    // 启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，并对其进行回收
    thread scanner(std::bind(&ConnectionPool::produceConnectionTask, this));
    scanner.detach();
}

// 运行在独立的线程中，专门负责产生新连接
void ConnectionPool::produceConnectionTask()
{
    for (;;)
    {
        unique_lock<mutex> lock(_queueMutex);
        while (!_connectionQue.empty())
        {
            // 队列非空时，此处生产线程进入等待状态
            cv.wait(lock);
        }

        // 连接数量没有到达上限，继续创建新的连接
        if (_connectionCnt < _maxSize)
        {
            Connection *p = new Connection();
            p->connect(_ip, _port, _username, _password, _dbname);
            _connectionQue.push(p);
            _connectionCnt++;
        }

        // 通知消费者线程，可以消费连接了
        cv.notify_all();
    }
}

// 给外部提供接口，从连接池中获取一个可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(_queueMutex);
    while (_connectionQue.empty())
    {
        if (cv_status::timeout == cv.wait_for(lock, std::chrono::milliseconds(_connectionTimeout)))
        {
            if (_connectionQue.empty())
            {
                LOG("Failed to get connection:got idle connection timeout!");
                return nullptr;
            }
        }
    }

    /*
     * shared_ptr智能指针析构时，会把connection资源直接delete掉，
     * 相当于调用connection的析构函数，connection就被close掉了。
     * 这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue当中*/
    shared_ptr<Connection> sp(_connectionQue.front(),
                              [&](Connection *pcon)
                              {
                                  // 这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全操作
                                  unique_lock<mutex> lock(_queueMutex);
                                  pcon->refreshAliveTime(); //在归还回空闲连接队列之前要记录一下连接开始空闲的时刻
                                  _connectionQue.push(pcon);
                              });

    _connectionQue.pop();

    // 消费者取出一个连接之后，通知生产者，生产者检查队列，如果为空则生产
    cv.notify_all();

    return sp;
}

// 扫描超过maxIdleTime时间的空闲连接，进行对于连接的回收
void ConnectionPool::scannerConnectionTask()
{
    for (;;)
    {
        // 通过sleep实现定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));

        // 扫描整个队列，释放多余的连接
        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize)
        {
            Connection *p = _connectionQue.front();
            if (p->getAliveTime() >= (_maxIdleTime * 1000))
            {
                _connectionQue.pop();
                _connectionCnt--;
                delete p; // 调用~Connection()释放连接
            } else
            {
                // 队头的连接没有超过_maxIdleTime，其它连接肯定也没有
                break;
            }
        }
    }
}
