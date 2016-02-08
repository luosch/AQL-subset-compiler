功能实现：

1、select语句：从一个view中选择多列。

2、extract regex语句：
    1）正则表达式需满足匹配的是完整的TextToken，单个或多个皆可。
    2）return的group语句的标号可以乱序，也可缺失，如只取了group 0、3、2，没有group 1。
    3）正则表达式请不要在最外层添加捕获块，因为默认group 0为整个捕获的串，若在最外层添加括号，则会产生捕获冗余。

3、extract pattern语句：
    1）正则表达式只能捕获单个TextToken。
    2）return的group语句的标号可以乱序，也可缺失，如只取了group 0、3、2，没有group 1。
    3）pattern语句请不要再最外层添加捕获块，同理 2的3）。
    4）pattern里的正则会忽略内部的捕获块，但若想对该正则匹配的内容进行捕获，则需对该正则构成的atom手动添加()进行捕获，即不会自动视之为一个group。
    5）可对pattern中的atoms进行任意捕获，atom{min,max}或者atom （-> <Token> | <column> | /regex/ ），只要是atom皆可。

4、运行环境：
    Ubuntu, OSX, gitbash for windows, 其他环境不保证能编译成功
    请升级 g++ 版本以支持 -std=c++11 选项
    如果无法编译可以修改 runner 为 -std=c++0x
    在内存较小的虚拟机环境下（内存小于2G）不加 -O2 执行会发生段错误
    所以 Makefile 里面都加了 -O2 优化，如果运行有问题可以去掉
    有任何问题可以联系 QQ 475884772 或者发邮件给我 475884772@qq.com
    辛苦 TA 了 ~~~

5、运行说明：
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

6、除以上特殊说明外，project的实现皆满足作业的基本要求。

小组分工：
    队长0：13331193 | 罗思成 | tokenizer 分词器 和 lexer 词法分析
    队员1：13331194 | 罗添仁 | parser 语法分析 和 pattern 匹配
    队员2：13331087 | 黄俊杰 | 编写 Makefile 的单元测试，并及时汇报 bug
    队员3：13331090 | 黄王爵 | 编写公共库 global.cpp global.h 和 AQL 入口 main.cpp
    队员4：13331291 | 肖逸祺 | 查找数据集 ds1 和 ds2 共 6 篇文章，并编写合适的 aql 脚本 