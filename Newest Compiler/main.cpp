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
   myscanner.outtoken(alltoken,tokenNum); ///���������Ĵ���
  if(lexmark==0) {
    cout<<"=======================�ʷ��������ִ��󣬱���ʧ�ܽ���������====================="<<endl;
    return 0;
   }

  cout<<"================�ʷ������������﷨������ʼ=========================\n";
  stack <token> sta;
  stack <node *> nodeSta;
  node *root=NULL;
  parse myParse;
  bool mark = myParse.function(alltoken,tokenNum,nodeSta,sta,root); ///�﷨������ʼ
  if(!mark) { ///�﷨����ʧ��
       cout<<endl<<endl<<"�﷨�������ִ��󣡣����������δ���У������......"<<endl;
       return 0;
  }
   cout<<"========================�﷨�������������������ʼ==============================\n������.......\n";

   trans tra;
   tra.transFunction(root);///���巭��
   tra.printID();
   tra.printResult(1); ///������巭����
  return 0;
}
