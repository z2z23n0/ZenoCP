这里的配置文件是一个示例，需要改成可以连接自己数据库的配置：

- ip：MySQL Server的ip地址
- port：MySQL Server的端口号
- username：MySQL Server的用户名
- password：MySQL Server的密码
- dbname：MySQL数据库名称
- initSize：初始连接量
- maxSize：最大连接量
- maxIdleTime：最大空闲时间
- connectionTImeout：连接超时时间

需要注意的是，不能在配置文件中的非注释行加入空格