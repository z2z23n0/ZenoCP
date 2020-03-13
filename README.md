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
- [使用](#Usage)
- [使用许可](#License)

## Background

为了提高MySQL数据库（基于C/S设计）的访问瓶颈，除了在服务器端增加缓存服务器缓存常用的数据（例如redis）之外，还可以增加连接池，来提高MySQL Server的访问效率，在高并发情况下，大量的TCP三次握手、MySQL Server连接认证、MySQL Server关闭连接回收资源和TCP四次挥手所耗费的性能时间也是很明显的，增加连接池就是为了减少这一部分的性能损耗。

在市场上比较流行的连接池包括阿里的Druid、C3P0以及apache DBCP，它们对于短时间内大量的数据库增删改查操作的性能提升是很明显的，但是它们都有一个共同点，都是由java实现的。

本项目就是为了在C/C++项目中，提供MySQL Server的访问效率，实现基于C++代码的数据库连接池。

## Usage

## License

[MIT © YuzeZhang.](https://github.com/YuzeZhang/ZenoCP/blob/master/LICENSE)