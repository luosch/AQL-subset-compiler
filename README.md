##功能实现

1. select语句：从一个view中选择多列。

2. extract regex语句：
    * 正则表达式需满足匹配的是完整的TextToken，单个或多个皆可。
    * return的group语句的标号可以乱序，也可缺失，如只取了group 0、3、2，没有group 1。
    * 正则表达式请不要在最外层添加捕获块，因为默认group 0为整个捕获的串，若在最外层添加括号，则会产生捕获冗余。

3. extract pattern语句：
    * 正则表达式只能捕获单个TextToken。
    * return的group语句的标号可以乱序，也可缺失，如只取了group 0、3、2，没有group 1。
    * pattern语句请不要再最外层添加捕获块，同理 2的3）。
    * pattern里的正则会忽略内部的捕获块，但若想对该正则匹配的内容进行捕获，则需对该正则构成的atom手动添加()进行捕获，即不会自动视之为一个group。
    * 可对pattern中的atoms进行任意捕获，atom{min,max}或者atom （-> <Token> | <column> | /regex/ ），只要是atom皆可。

4. 运行环境：
    * Ubuntu, OSX, gitbash for windows, 其他环境不保证能编译成功
	 * 请升级 g++ 版本以支持 -std=c++11 选项
    * 如果无法编译可以修改 runner 为 -std=c++0x
    * 在内存较小的虚拟机环境下（内存小于2G）不加 -O2 执行会发生段错误
    * 所以 Makefile 里面都加了 -O2 优化，如果运行有问题可以去掉

##运行说明

```bash
# 首先切换到代码目录，注意：以下所有操作都在 src 目录下面进行
cd src/
# 编译 AQL 执行文件
make runner
# 或者用 Makefile 默认选项也是编译 AQL 执行文件
make
# 处理作业样例1，读取单个文件，必须为.input 或 .txt 后缀）
./AQL ../dataset/PerLoc.aql ../dataset/perloc/PerLoc.input
# 处理作业样例2，读取一个目录下的后缀为 .input 或 .txt 的文件
./AQL ../dataset/Revenue.aql ../dataset/revenue/
# 处理数据集1，这里是输出到命令行，可通过重定向输出到文件中
./AQL ../dataset/ds1.aql ../dataset/ds1/Warriors_Cavaliers_Preview.txt
./AQL ../dataset/ds1.aql ../dataset/ds1/Warriors_Hold_Off_Cavs.txt
./AQL ../dataset/ds1.aql ../dataset/ds1/Warriors_Silence_the_Jazz.txt
# 处理数据集2，这里是输出到命令行，可通过重定向输出到文件中
./AQL ../dataset/ds2.aql ../dataset/ds2/Founding_of_modern_Singapore_1819.txt
./AQL ../dataset/ds2.aql ../dataset/ds2/Post_war_period_1945.txt
./AQL ../dataset/ds2.aql ../dataset/ds2/Republic_of_Singapore_1965.txt
```

##小组分工
~~主要功能由罗添仁和罗思成实现~~
