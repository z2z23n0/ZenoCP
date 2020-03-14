# ZenoCP

![language](https://img.shields.io/badge/language-c++-DeepPink.svg) [![GitHub license](https://img.shields.io/github/license/YuzeZhang/ZenoCP.svg)](https://github.com/YuzeZhang/ZenoCP/blob/master/LICENSE) [![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

本仓库是使用C++实现的基于生产者消费者线程模型的连接池项目，能大幅提高MySQL数据库的访问效率，同时由于使用C++语言级的多线程编程，提供了可移植性，适用于Windows和Linux等不同平台。

本仓库包含以下内容：

1. ZenoCP源码
2. 分功能测试代码以及压力测试源码
3. 使用所需MySQL动态链接库
4. 示例配置文件

## Table of Contents

- [背景](#Background)
- [连接池功能点介绍](#Features)
- [使用](#Usage)
- [压力测试](#Stress-Testing)
- [使用许可](#License)

## Background

为了提高MySQL数据库（基于C/S设计）的访问瓶颈，除了在服务器端增加缓存服务器缓存常用的数据（例如redis）之外，还可以增加连接池，来提高MySQL Server的访问效率，在高并发情况下，大量的**TCP三次握手**、**MySQL Server连接认证**、**MySQL Server关闭连接回收资源**和**TCP四次挥手**所耗费的性能时间也是很明显的，增加连接池就是为了减少这一部分的性能损耗。

在市场上比较流行的连接池包括阿里的Druid、C3P0以及apache DBCP，它们对于短时间内大量的数据库增删改查操作的性能提升是很明显的，但是它们都有一个共同点，都是由java实现的。

本项目就是为了在C/C++项目中，提供MySQL Server的访问效率，实现基于C++代码的数据库连接池。

## Features

连接池一般包含了数据库连接所用的ip地址、port端口号、用户名和密码以及其它的性能参数，例如初始连接量、最大连接量、最大空闲时间、连接超时时间等，该项目是基于C++语言实现的连接池，主要也是实现以上几个所有连接池都支持的通用基础功能。

**初始连接量（initSize）**：表示连接池事先会和MySQL Server创建initSize个connection连接，当应用发起MySQL访问时，不用再创建和MySQL Server的新连接，直接从连接池中获取一个可用的连接就可以，使用完成后，并不去释放connection，而是把当前connection再归还到连接池当中。

**最大连接量（maxSize）**：当并发访问MySQL Server的请求增多时，初始连接量已经不够用了，此时会根据新的请求数量去创建更多的连接给应用去使用，但是新创建的连接数量上限是maxSize，不能无限制的创建连接，因为每一个连接都会占用一个socket资源，一般连接池和服务器程序是部署在一台主机上的，如果连接池占用过多的socket资源，那么服务器就不能接收太多的客户端请求了。当这些连接使用完成后，再次归还到连接池当中来维护。

**最大空闲时间（maxIdleTime）**：当访问MySQL的并发请求多了以后，连接池里面的连接数量会动态增加，上限是maxSize个，当这些连接使用完成后，会再次归还到连接池当中。如果在指定的maxIdleTime里面，这些新增加的连接都没有被再次使用过，那么新增加的这些连接资源就要被回收掉，只需要保持初始连接量initSize个连接就可以了。

**连接超时时间（connectionTimeout）**：当MySQL的并发请求量过大，连接池中的连接数量已经到达maxSize了，而此时没有空闲的连接可供使用，那么此时应用从连接池获取连接无法成功，它通过阻塞的方式获取连接的时间如果超过connectionTimeout时间，那么获取连接失败，无法访问数据库。

该项目主要实现上述的连接池基本功能，将来可能会增加更多的连接池扩展功能。

## Usage

**使用说明：**

1. 在编译环境中添加对应环境（Windows/Linux、32位/64位等）的MySQL依赖的include文件夹和动态链接库

2. 在需要使用数据库连接池的项目中include本库中src目录下代码

   ![连接池代码示例1](https://zhangyuzechn.cn/wp-content/uploads/2020/03/连接池代码.gif)

   ![连接池代码示例2](https://zhangyuzechn.cn/wp-content/uploads/2020/03/连接池代码2.gif)

3. 仿照testStressTesting.cpp中测试多线程使用连接池的用法，即可在其它项目中使用数据库连接池

   ![连接池测试代码](https://zhangyuzechn.cn/wp-content/uploads/2020/03/连接池测试代码.gif)

## Stress Testing

验证数据的插入操作所花费的时间，分别使用单线程、多线程的普通的数据库访问操作与单线程、多线程的带连接池的数据库访问操作，并记录操作同样数据量所花费的时间，性能压力测试结果如下：

![数据库连接池压力测试](https://zhangyuzechn.cn/wp-content/uploads/2020/03/数据库连接池压力测试.png)

## License

[MIT © YuzeZhang.](https://github.com/YuzeZhang/ZenoCP/blob/master/LICENSE)