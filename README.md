# compiler-with-LL1-grammer
Implementation of compiler using LL1 grammer with C++

more:http://blog.csdn.net/u014665013/article/details/77587351

user manual：

Newest Compiler文件为源程序文件
使用CodeBlock打开Newest Compiler.cbp源程序

Newest Compiler文件下的paint文件为paint平台，程序编译完成后输出的命令将直接存储于paint//paint.txt文件
Newest Compiler文件下的source文件为中间文件的输入输出文件，并且读取的程序代码存储于该文件夹下的src.txt

source文件下的测试代码文件包含有典型测试用例，可直接黏贴至source文件下替换src.txt进行测试
source文件下的表：
   idTable.txt  语义翻译时变量表（===之间为每次变量调用或者是变化时表状态）
   Stack.txt  语法分析时的出入栈情况
   Token.txt 存储分离出的词素或者是词法分析时的错误信息
   Tree.txt  存储生成的树或者是语法分析时的错误信息
