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

ofstream  outTrees("source/Tree.txt",ios::out);  ///��������д��Tree.txt��
ofstream  outStack("source/Stack.txt",ios::out);///���﷨�����������̳���ջд��Stack.txt


///�ս��
string terminal[TerNUM]={
                      "BEGIN","END","if","else","while","int","float","ID","NUM","true","false","+","-","*","/","%","(",")",";",",",
                      ">","<","==","filename","help","file","delay","reset","point","line","rotate","translate","scale",
                      "get window size","set window size","set color","set point size","set line width","move to","line to","=",
                    };
///���ս��
string nterminal[NTerNUM] ={
                       "program","program2","Stmt-List","Stmt-List2","Stmt","Assign-Stmt","If-Stmt","If-Stmt2","Declare-Stmt",
                       "ID-List","ID-List2","Repeat-Stmt","Compare-Exp","Command","Expr","Expr2","Term","Term2","Factor",
                       "ID-NUM","ID-NUM2","Add-OP","Multi-OP","Comp-OP","File-Com","File-Com2","Delay-Com","Set-Window-Com","Set-Color-Com",
                       "Set-PointSiz-Com","Set-LineWid-Com","Point-Com","Scale-Com","Translate-Com","Reset-Com","Get-windsiz-Com",
                       "MoveTo-Com","LineTo-Com","Line-Com","Rotate-Com", "Help-Com","Param-1","Param-2","Param-3"
                      };

int findTerminal(token ttok){///�����ս��
   // cout<<"******************"<<"���ҵĴ���Ϊ��"<<ttok.index<<"���ࣺ"<<ttok.kind;
    int zz=-1;
    for(int i=0;i<TerNUM;i++)
     if(ttok.index==terminal[i])
       zz=i;
    //   cout<<"���ҽ����"<<zz<<endl;
    return zz;
}
int findNTerminal(token ttok){///���ҷ��ս��
  for(int i=0;i<NTerNUM;i++){
    if(ttok.index==nterminal[i])
        return i;
  }
  return -1;
}

void parse::printStack(stack <token> ssta){  ///���ջ�е��ַ���
    cout<<"\n��ǰջΪ��";
    outStack<<"\n��ǰջΪ��";
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
      if(gramm[i]==' '){///�ո����
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


void buildTree(node *father,stack <node *> &tempsta){  ///�������ڵ�
	while(!tempsta.empty())
    {
      father->child[father->childNum] =  tempsta.top();
      father->childNum++;
      tempsta.pop();
    }
    return;
}

void pushstack(string gramm,stack <token> &sta,stack <node *> &nodeSta,node *father)///��ջ��������
{
    int tokNum=0;
    token splitToken[10];
    gettoken(gramm,splitToken,tokNum);
    node *p;
    stack <node *> tempsta; ///�����ݴ���������ڵ�
    for(int i=tokNum-1; i>=0; i--)
    {
        p=new node();
        p->tokenn=splitToken[i];
        p->childNum=0;
        sta.push(splitToken[i]);
        nodeSta.push(p);
        tempsta.push(p);///�����ݴ�ջ������֮����
        cout<<"�ַ�ջ��ջ"<<splitToken[i].index<<endl;
        outStack<<"�ַ�ջ��ջ"<<splitToken[i].index<<endl;
    }
    buildTree(father,tempsta);
}

void parse::printTree (node *p) ///���ղ������������
{
    queue <node *> que;
    que.push(p);
    outTrees<<"���ڵ� -->   ���ӽڵ� \n\n";
    while(!que.empty())
    {
        node *top=que.front();
        que.pop();
        cout<<"�ڵ�"<<top->tokenn.index <<":" ;///    top->tokenn.index <<":" ;
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
    int terminalIndex=findTerminal(alltoken[nowPoint]); ///�洢�ս�����к�

    token temptoken("program","program","",-1);
    sta.push(temptoken);
    cout<<"�ַ�ջ��ջ"<<"pragram"<<endl;
    outStack<<"�ַ�ջ��ջ"<<"pragram"<<endl;

    node *p;
    p=new node();
    p->tokenn=temptoken;
    p->childNum=0;
    root=p;
    nodeSta.push(p);

    while(!sta.empty()&&nowPoint<=tokenNum )
    {
        printStack(sta);
        if(sta.top().index=="@"){ ///����ǿմ���ֱ�ӽ�ջ�����˳�
          nodeSta.top()->tokenn.value="@";
          sta.pop();

          nodeSta.pop();
        }
        else if(findTerminal(sta.top())!=-1)///ջ�����ս����ֱ���жϲ�ƥ��
        {
           if(sta.top().index==alltoken[nowPoint].index) ///��ȷƥ��
            {
                cout<<"�ַ���ջ"<<sta.top().index<<endl;
                outStack<<"�ַ���ջ"<<sta.top().index<<endl;
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
                terminalIndex=findTerminal(alltoken[nowPoint]); ///�洢�ս�����к�
            }
            else ///ƥ�����
            {
                cout<<"\n\n\n��error����"<<alltoken[nowPoint].loca<<"�е��м��ַ�"<<alltoken[nowPoint].index<<"ƥ�����"<<"�������ַ�Ϊ��"<<sta.top().index<<endl;
                return 0;
            }
        }
        else {  ///ջ��Ϊ���ս��
            int notTerIndex = findNTerminal(sta.top());
            if(notTerIndex<0||notTerIndex>=45)  {
                cout<<"�����ս����ʱ������������ҵ�����Ϊ��"<<notTerIndex<<"���ַ�Ϊ��"<<sta.top().index<<"���ࣺ"<<sta.top().name<<endl;
                return 0;
            }
            if( preTable[notTerIndex][ terminalIndex ] == "")   ///�﷨���ִ���
            {
               // if
                cout<<alltoken[nowPoint].index<<"\n\n\n��error���﷨���󣬳��ֲ��������ַ�������λ�õ�";
                if(alltoken[nowPoint].loca==0)  cout<<alltoken[nowPoint-1].loca ;
                else cout<<alltoken[nowPoint].loca ;
                cout<<"��,ƥ�����ķ��ս��:";
                if(sta.top().index=="ID")  cout<<sta.top().name<<endl;
                else cout<<sta.top().value<<endl;
                break;
            }
            else if( preTable[notTerIndex][ terminalIndex ] == "@"){ ///���ֿմ���ֱ�ӵ��������
                cout<<"�ַ���ջ"<<sta.top().index<<endl;
                outStack<<"�ַ���ջ"<<sta.top().index<<endl;

                node * father=nodeSta.top();
                sta.pop();
                nodeSta.pop();

                temptoken.name="@",temptoken.index="@",temptoken.value="",temptoken.loca=0;
                sta.push(temptoken); ///��Ȼ�Ƶ�ջ������֮��ͳһ����

                p=new node();///�����µ����ڵ�

                p->tokenn=temptoken ;
                p->childNum=0;
                nodeSta.push(p);
                father->child[father->childNum] = p;
                father->childNum++;
            }

            else{  ///Ԥ��������в��ǿմ�
                cout<<"�ַ���ջ"<<sta.top().index<<endl;
                outStack<<"�ַ���ջ"<<sta.top().index<<endl;
                node *father=nodeSta.top();

                nodeSta.pop();///����ǰ�ķ��ս���Ƴ�
                sta.pop();
                pushstack(preTable[notTerIndex][ terminalIndex ],sta,nodeSta,father);  ///��ջ��ͬʱ���������ڵ�
            }
        }
    }
    if(nowPoint!=tokenNum){
      cout<<"ƥ���������ʣ��δƥ����أ���ʣ�����Ϊ ��"<<endl;
      outTrees<<"\n========================�﷨����������Ϣ=====================\nʣ�����Ϊ��\n";
      for(int i=nowPoint;i<tokenNum;i++){
        printf("(%s,%s,%s,%d)\n",alltoken[i].index.c_str(),alltoken[i].name.c_str(),alltoken[i].value.c_str(),alltoken[i].loca);
        outTrees<<"("<<alltoken[i].index<<","<<alltoken[i].name<<","<<alltoken[i].value<<","<<alltoken[i].loca<<")"<<endl;
      }
      return false;
    }
    else if(!sta.empty()){
        cout<<"ƥ�����ʱ��ջ�л����ַ����﷨����\n ջ��ʣ��ڵ�����Ϊ��"<<endl;
        outTrees<<"\n========================�﷨����������Ϣ=====================\nջ��ʣ��ڵ�Ϊ��\n";
        while(!sta.empty()){
            printf("(%s,%s,%s,%d)\n",sta.top().index.c_str(),sta.top().name.c_str(),sta.top().value.c_str(),sta.top().loca);
            outTrees<<"("<<sta.top().index<<","<<sta.top().name<<","<<sta.top().value<<","<<sta.top().loca<<")";
            sta.pop();
        }
        return 0;
    }
    else {
            cout<<"�﷨�����������ɹ�������"<<"\n���ɵ��﷨��Ϊ��\n-----------------------------------------------------------------------------\n";
            outTrees<<"�﷨�����������ɹ�������"<<"\n���ɵ��﷨��Ϊ��\n-----------------------------------------------------------------------------\n";
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
