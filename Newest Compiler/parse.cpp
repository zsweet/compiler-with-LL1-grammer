#include "parse.h"
#include "myclass.h"
#include <iostream>
#include <stack>
#include <iomanip>
#include <queue>
#include <fstream>
#define TerNUM 42
#define NTerNUM 44
using namespace std;

int wrongMark=0;

ofstream  outTrees("source/Tree.txt",ios::out);  ///产生的树写入Tree.txt中
ofstream  outStack("source/Stack.txt",ios::out);///将语法分析产生过程出入栈写入Stack.txt


///终结符
string terminal[TerNUM]={
                      "BEGIN","END","if","else","while","int","float","ID","NUM","true","false","+","-","*","/","%","(",")",";",",",
                      ">","<","==","filename","help","file","delay","reset","point","line","rotate","translate","scale",
                      "get window size","set window size","set color","set point size","set line width","move to","line to","=",
                    };
///非终结符
string nterminal[NTerNUM] ={
                       "program","program2","Stmt-List","Stmt-List2","Stmt","Assign-Stmt","If-Stmt","If-Stmt2","Declare-Stmt",
                       "ID-List","ID-List2","Repeat-Stmt","Compare-Exp","Command","Expr","Expr2","Term","Term2","Factor",
                       "ID-NUM","ID-NUM2","Add-OP","Multi-OP","Comp-OP","File-Com","File-Com2","Delay-Com","Set-Window-Com","Set-Color-Com",
                       "Set-PointSiz-Com","Set-LineWid-Com","Point-Com","Scale-Com","Translate-Com","Reset-Com","Get-windsiz-Com",
                       "MoveTo-Com","LineTo-Com","Line-Com","Rotate-Com", "Help-Com","Param-1","Param-2","Param-3"
                      };

int findTerminal(token ttok){///查找终结符
   // cout<<"******************"<<"查找的词素为："<<ttok.index<<"种类："<<ttok.kind;
    int zz=-1;
    for(int i=0;i<TerNUM;i++)
     if(ttok.index==terminal[i])
       zz=i;
    //   cout<<"查找结果："<<zz<<endl;
    return zz;
}
int findNTerminal(token ttok){///查找非终结符
  for(int i=0;i<NTerNUM;i++){
    if(ttok.index==nterminal[i])
        return i;
  }
  return -1;
}

void parse::printStack(stack <token> ssta){  ///输出栈中的字符串
    cout<<"\n当前栈为：";
    outStack<<"\n当前栈为：";
    while(!ssta.empty())
    {
    cout<<ssta.top().index<<' ';
    outStack<<ssta.top().index<<' ';
    ssta.pop();
    }
    cout<<endl;
    outStack<<endl;
}

void gettoken(string gramm,token splitToken[],int &tokNum){
  int len = gramm.length();
  int i=0;
  while(i<len){
    int space=0;
    string ttoken="";
    while(i<len&&space<2){
      if(gramm[i]==' '){///空格忽略
         space++;
         if(ttoken=="get"||ttoken=="get window"||ttoken=="set"||ttoken=="set window"||ttoken=="set point"||ttoken=="set line"||ttoken=="move")
            ttoken+=gramm[i];
         if(ttoken=="line"&&i+1<len&&gramm[i+1]=='t')
            ttoken+=gramm[i];
      }
      else
        ttoken+=gramm[i],space=0;
      i++;
    }

    token temptok(ttoken, ttoken , ttoken ,-1);
    splitToken[tokNum++]=temptok;
  }
}


void buildTree(node *father,stack <node *> &tempsta){  ///建立树节点
	while(!tempsta.empty())
    {
      father->child[father->childNum] =  tempsta.top();
      father->childNum++;
      tempsta.pop();
    }
    return;
}

void pushstack(string gramm,stack <token> &sta,stack <node *> &nodeSta,node *father)///入栈建树过程
{
    int tokNum=0;
    token splitToken[10];
    gettoken(gramm,splitToken,tokNum);
    node *p;
    stack <node *> tempsta; ///用来暂存正向的树节点
    for(int i=tokNum-1; i>=0; i--)
    {
        p=new node();
        p->tokenn=splitToken[i];
        p->childNum=0;
        sta.push(splitToken[i]);
        nodeSta.push(p);
        tempsta.push(p);///放入暂存栈，用来之后建树
        cout<<"字符栈入栈"<<splitToken[i].index<<endl;
        outStack<<"字符栈入栈"<<splitToken[i].index<<endl;
    }
    buildTree(father,tempsta);
}

void parse::printTree (node *p) ///按照层输出建立的树
{
    queue <node *> que;
    que.push(p);
    outTrees<<"父节点 -->   孩子节点 \n\n";
    while(!que.empty())
    {
        node *top=que.front();
        que.pop();
        cout<<"节点"<<top->tokenn.index <<":" ;///    top->tokenn.index <<":" ;
        outTrees<<setiosflags(ios::fixed)<<setiosflags(ios::right)<<setw(12)<<top->tokenn.index <<"--> ";
        for(int i=0; i<top->childNum; i++)
        {
            if(top->child[i]->tokenn.index=="filename")
                cout<<top->child[i]->tokenn.value<<" ";
            else
                cout<<top->child[i]->tokenn.index<<' ';
            outTrees<<top->child[i]->tokenn.index<<"  ";
            if( findNTerminal( top->child[i]->tokenn )!=-1 )
              que.push(top->child[i]);
        }

        if(top->childNum==0)
            cout<<"NULL";
        cout<<endl;
        outTrees<<endl;
    }
}


bool parse::function(token alltoken[],int tokenNum,stack <node * > &nodeSta,stack <token> sta,node * &root){
    int nowPoint=0;
    int terminalIndex=findTerminal(alltoken[nowPoint]); ///存储终结符的列号

    token temptoken("program","program","",-1);
    sta.push(temptoken);
    cout<<"字符栈入栈"<<"pragram"<<endl;
    outStack<<"字符栈入栈"<<"pragram"<<endl;

    node *p;
    p=new node();
    p->tokenn=temptoken;
    p->childNum=0;
    root=p;
    nodeSta.push(p);

    while(!sta.empty()&&nowPoint<=tokenNum )
    {
        printStack(sta);
        if(sta.top().index=="@"){ ///如果是空串，直接将栈顶的退出
          nodeSta.top()->tokenn.value="@";
          sta.pop();

          nodeSta.pop();
        }
        else if(findTerminal(sta.top())!=-1)///栈里是终结符，直接判断并匹配
        {
           if(sta.top().index==alltoken[nowPoint].index) ///正确匹配
            {
                cout<<"字符出栈"<<sta.top().index<<endl;
                outStack<<"字符出栈"<<sta.top().index<<endl;
                nodeSta.top()->tokenn.name=alltoken[nowPoint].name;
                //else
                nodeSta.top()->tokenn.value=alltoken[nowPoint].value;
                nodeSta.top()->tokenn.index=alltoken[nowPoint].index;
                nodeSta.top()->tokenn.loca=alltoken[nowPoint].loca;

                sta.pop();
                nodeSta.pop();


                nowPoint++;
                if(sta.empty())
                    continue;
                terminalIndex=findTerminal(alltoken[nowPoint]); ///存储终结符的列号
            }
            else ///匹配出错
            {
                cout<<"\n\n\n【error】第"<<alltoken[nowPoint].loca<<"行的中间字符"<<alltoken[nowPoint].index<<"匹配错误"<<"期望的字符为："<<sta.top().index<<endl;
                return 0;
            }
        }
        else {  ///栈中为非终结符
            int notTerIndex = findNTerminal(sta.top());
            if(notTerIndex<0||notTerIndex>=45)  {
                cout<<"查找终结符的时候出错！！！查找的行数为："<<notTerIndex<<"该字符为："<<sta.top().index<<"种类："<<sta.top().name<<endl;
                return 0;
            }
            if( preTable[notTerIndex][ terminalIndex ] == "")   ///语法出现错误
            {
               // if
                cout<<alltoken[nowPoint].index<<"\n\n\n【error】语法错误，出现不被期望字符，错误位置第";
                if(alltoken[nowPoint].loca==0)  cout<<alltoken[nowPoint-1].loca ;
                else cout<<alltoken[nowPoint].loca ;
                cout<<"行,匹配错误的非终结符:";
                if(sta.top().index=="ID")  cout<<sta.top().name<<endl;
                else cout<<sta.top().value<<endl;
                break;
            }
            else if( preTable[notTerIndex][ terminalIndex ] == "@"){ ///出现空串，直接弹出后继续
                cout<<"字符出栈"<<sta.top().index<<endl;
                outStack<<"字符出栈"<<sta.top().index<<endl;

                node * father=nodeSta.top();
                sta.pop();
                nodeSta.pop();

                temptoken.name="@",temptoken.index="@",temptoken.value="",temptoken.loca=0;
                sta.push(temptoken); ///依然推到栈，便于之后统一操作

                p=new node();///建立新的树节点

                p->tokenn=temptoken ;
                p->childNum=0;
                nodeSta.push(p);
                father->child[father->childNum] = p;
                father->childNum++;
            }

            else{  ///预测分析表中不是空串
                cout<<"字符出栈"<<sta.top().index<<endl;
                outStack<<"字符出栈"<<sta.top().index<<endl;
                node *father=nodeSta.top();

                nodeSta.pop();///将当前的非终结符推出
                sta.pop();
                pushstack(preTable[notTerIndex][ terminalIndex ],sta,nodeSta,father);  ///入栈的同时，建立树节点
            }
        }
    }
    if(nowPoint!=tokenNum){
      cout<<"匹配出错后出现剩余未匹配词素，且剩余词素为 ："<<endl;
      outTrees<<"\n========================语法分析错误信息=====================\n剩余词素为：\n";
      for(int i=nowPoint;i<tokenNum;i++){
        printf("(%s,%s,%s,%d)\n",alltoken[i].index.c_str(),alltoken[i].name.c_str(),alltoken[i].value.c_str(),alltoken[i].loca);
        outTrees<<"("<<alltoken[i].index<<","<<alltoken[i].name<<","<<alltoken[i].value<<","<<alltoken[i].loca<<")"<<endl;
      }
      return false;
    }
    else if(!sta.empty()){
        cout<<"匹配完成时，栈中还有字符，语法错误\n 栈中剩余节点内容为："<<endl;
        outTrees<<"\n========================语法分析错误信息=====================\n栈中剩余节点为：\n";
        while(!sta.empty()){
            printf("(%s,%s,%s,%d)\n",sta.top().index.c_str(),sta.top().name.c_str(),sta.top().value.c_str(),sta.top().loca);
            outTrees<<"("<<sta.top().index<<","<<sta.top().name<<","<<sta.top().value<<","<<sta.top().loca<<")";
            sta.pop();
        }
        return 0;
    }
    else {
            cout<<"语法分析结束，成功！！！"<<"\n生成的语法树为：\n-----------------------------------------------------------------------------\n";
            outTrees<<"语法分析结束，成功！！！"<<"\n生成的语法树为：\n-----------------------------------------------------------------------------\n";
            printTree(root);
            return 1;
    }
}


parse::parse()
{
    //ctor
}

parse::~parse()
{
    //dtor
}
