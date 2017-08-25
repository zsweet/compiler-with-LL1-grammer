#include <iostream>
#include <iomanip>
#include <cstdio>
#include "myclass.h"
#include "scanner.h"
#include "parse.h"
#include "trans.h"
#include <fstream>
#include <stack>
using namespace std;

int main()
{
  token alltoken[Maxsize];
  int tokenNum=0;
  ifstream in("source/src.txt");
   string temp;
   int lineCoun=1;
   scanner myscanner;
   bool lexmark=1;
   while( getline(in,temp)  )
   {
     if(!myscanner.scannerFuntion(temp,lineCoun,alltoken,tokenNum))
        lexmark=0;
     lineCoun++;
   }
   myscanner.outtoken(alltoken,tokenNum); ///输出分离出的词素
  if(lexmark==0) {
    cout<<"=======================词法分析出现错误，编译失败结束！！！====================="<<endl;
    return 0;
   }

  cout<<"================词法分析结束，语法分析开始=========================\n";
  stack <token> sta;
  stack <node *> nodeSta;
  node *root=NULL;
  parse myParse;
  bool mark = myParse.function(alltoken,tokenNum,nodeSta,sta,root); ///语法分析开始
  if(!mark) { ///语法分析失败
       cout<<endl<<endl<<"语法分析发现错误！！！语义分析未进行，请调试......"<<endl;
       return 0;
  }
   cout<<"========================语法分析结束，语义分析开始==============================\n编译中.......\n";

   trans tra;
   tra.transFunction(root);///语义翻译
   tra.printID();
   tra.printResult(1); ///输出语义翻译结果
  return 0;
}
