#include "scanner.h"
#include "myclass.h"
#include <iostream>
#include <cstdio>
#include <iomanip>
#include<stdlib.h>
#include <fstream>
using namespace std;
ofstream  outToken("source/Token.txt",ios::out);///将词法分析的分离出的词素写入Token.txt中
bool mark=1;
int keywordNum = 27;
string keyword[] = {"help","file","delay","reset","set window size",
                   "get window size","set color",
                   "set point size","set line width",
                   "move to","line to","point","line","rotate",
                   "ratate","scale","translate","if","then",
                   "else","BEGIN","END","while","int","float",
                   "true","false"
                   };
bool scanner::isOperator(char ch){
  if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%')
      return true;
  return false;
}

void scanner::outtoken(token alltoken[],int tokenNum){
   outToken<<"词法分析成功全部词素为：\n"<<"(索引项        名字        值      所在源文件行)"<<endl;
   printf("词法分析全部词素为：\n((%-14s,%-10s,%-5s,%s)\n","索引项","名字","值","所在源文件行");


   for(int i=0;i<tokenNum;i++){
     printf("(%-14s,%-10s,%-5s,%d)\n",alltoken[i].index.c_str(),alltoken[i].name.c_str(),alltoken[i].value.c_str(),alltoken[i].loca );
     outToken<<setiosflags(ios::fixed)<<setiosflags(ios::left)<<"("<<setw(14)<<alltoken[i].index<<setw(1)<<","<<setw(10)<<alltoken[i].name<<setw(1)<<","<<setw(10)<<alltoken[i].value<<setw(1)<<','<<alltoken[i].loca<<")"<<endl;
   }
}
string scanner::lineTrim(string str,int lineCoun)///调整为标准模式
{
    int len = str.length();
    string temp="";
    for(int i=0; i<len; i++)
    {
      if(  (temp.length()==0&&str[i]==' ') || ( temp.length()>0&&(temp[temp.length()-1]==' '||temp[temp.length()-1]=='\t')&&(str[i]==' '||str[i]=='\t') )   )///去除空格或制表符
         continue;
      else if(str[i]==';'){
        if((temp.length()!=0 && temp[temp.length()-1]!=' '))
            temp+=" ";
        temp=temp+str[i]+" ";
      }
      else if(str[i]=='\"'){
        string ttemp="";
         if((temp.length()!=0 && temp[temp.length()-1]!=' '))
            ttemp+=" ";
        ttemp+="\"";
        i++;
        while(i<len&&str[i]!='\"'){
          if(str[i]==' '||str[i]=='\t')
            {
                  i++;
                  continue;
            }
            ttemp+=str[i++];
        }
        if(i<len&&str[i]=='"'){
            ttemp=ttemp+str[i++]+" ";
            temp+=ttemp;
        }

        else{
            printf("词法分析第%d行定义不符合规范！！！\"\"内路径缺少右\"\n",lineCoun);
            outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！\"\"内路径缺少右\""<<endl;
            mark=0;
            while(str[i]!=' ')
              i++;
        }
      }
      else if(str[i]=='/'  ){ /// 为注释
          if(i+1<len&&str[i+1]=='/')
            return temp;
          else{  ///分离为除号
            if((temp.length()!=0 && temp[temp.length()-1]!=' '))
            temp+=" ";
            temp=temp+str[i]+" ";
          }
      }
      else if( isOperator(str[i]) ||  str[i]=='(' || str[i]==')' || str[i]=='<'  || str[i]=='>' ||str[i]==',' ){     ///运算符和 (   )  >   <
        if((temp.length()!=0 && temp[temp.length()-1]!=' '))
            temp+=" ";
        temp=temp+str[i]+" ";
      }
      else if( str[i]=='=' ){      /// = ==
        if(temp.length()==0||(temp.length()!=0 && temp[temp.length()-1]!=' '))
           temp+=" ";
        if(i+1<len&&str[i+1]=='=')
            temp=temp+"=="+" ",i++;
        else
            temp=temp+str[i]+" ";
      }
      else{
        temp+=str[i];
      }
    }
    return temp;
}

bool scanner::isKeyword(string word){
  for(int i=0;i<keywordNum;i++)
    if(word==keyword[i])
      return true;
  return false;
}

bool scanner::isIntNum(string word,int lineCoun){
    int len = word.length();
  for(int i=0;i<len;i++)
    if(!(word[i]<='9'&&word[i]>='0'))
        if(i!=0&&word[i]!='.')
        {
            printf("词法分析第%d行定义不符合规范！！！变量不能以数字开头，出错词素：",lineCoun);
            outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！变量不能以数字开头，出错词素："<<word<<endl;
            cout<<word<<endl;
            mark=0;
        }
        else
            return false;
    return true;
}

bool scanner::isFloNum(string word,int lineCoun)
{
    int len = word.length();
    bool mark=0;
    for(int i=0; i<len; i++)
        if(word[i]<='9'&&word[i]>='0')
            continue;
        else if(  mark==0&&word[i]=='.'  )
            mark=1;
        else if(i!=0){
            printf("词法分析第%d行定义不符合规范！！！变量不能以数字开头，出错词素：",lineCoun);
            outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！变量不能以数字开头，出错词素："<<word<<endl;
            cout<<word<<endl;
           mark=0;
        }
        else
            return false;
    if(word[0]=='.'||word[len-1]=='.')
    {
        printf("词法分析第%d行定义不符合规范！！！数字中不能有字符，出错词素：",lineCoun);
        outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！变量不能以数字开头，出错词素："<<word<<endl;
        cout<<word<<endl;
        mark=0;
    }
    return true;
}
bool scanner::isID(string word){ ///判断是否符合变量规则
  int len = word.length();
  for(int i=0;i<len;i++)
    if( (i==0&&((word[i]<='z'&&word[i]>='a') ||  (word[i]<='Z'&&word[i]>='A') ||  word[i]=='_'))
       ||(i!=0&&(((word[i]<='z'&&word[i]>='a') ||  (word[i]<='Z'&&word[i]>='A') || (word[i]<='9'&&word[i]>='0') || word[i]=='_')) ) )
        continue;
    else
      return false;
return true;
}
bool scanner::scannerFuntion(string line,int lineCoun,token alltoken[],int &tokenNum){
  line = lineTrim( line,lineCoun );
  if(line=="\n"||line==" "||line=="\t"||line=="") ///空白行不做处理
     return 1;
  /// cout<<line<<endl;  ///测试
  int i=0,len = line.length();

  while(i<len){

    string word="";
    while( line[i]!=' '&&line[i]!='\t' && i<len )
      word+=line[i++];

    if(word=="set"){///获得包含set字符的指令串
        int j=i+11;
        for(;i<=j&&i<len;i++){
            word+=line[i];
           if(i==j-6&&word=="set color"){
                i++;
                break;
           }
           if(i==j-1&&word=="set point size"){
                i++;
                break;
           }
           if(i==j-1&&word=="set line width"){
                i++;
                break;
           }
        }
        if(!isKeyword(word) ||(  i<len && line[i]!='\t'&& line[i]!=' '  ) ){
          printf("词法分析第%d行定义不符合规范！！！不能用关键字作为变量，出错词素：",lineCoun);
          outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！不能用关键字作为变量，出错词素："<<word<<endl;
             cout<<word<<endl;
          mark=0;
          while(line[i]!=' ')
             i++;
        }
        else{
            alltoken[tokenNum].name="KEYWORD";
            alltoken[tokenNum].index = word;
            alltoken[tokenNum].loca=lineCoun;
            alltoken[tokenNum++].value=word;
        }
    }
    else if(word=="line"){///获得包含line字符的指令串;
      if(i+2<len&&word+line[i]+line[i+1]+line[i+2]=="line to" ){ ///当前字符为line to
         if( i+3>=len ||(line[i+3]=='\t'||line[i+3]==' ') ){  ///保证line to 后面紧跟的没有其他字母，有的话说明不是line to,使用了关键字报错
          word=word+line[i]+line[i+1]+line[i+2];
          i+=3;
           alltoken[tokenNum].name="KEYWORD";
           alltoken[tokenNum].index = word;
           alltoken[tokenNum].loca=lineCoun;
           alltoken[tokenNum++].value=word;
         }
         else{
              cout<<(i+3>=len)<<(line[i+3]=='\t')<<(line[i+3]==' ')<<endl;
              printf("a词法分析第%d行定义不符合规范！！！\n不能用关键字作为变量，出错词素：",lineCoun);
              outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！不能用关键字作为变量，出错词素："<<word<<endl;
              cout<<word<<endl;
              mark=0;
              while(line[i]!=' ')
                i++;
         }
      }
      else{
           alltoken[tokenNum].name="KEYWORD";
           alltoken[tokenNum].index = word;
           alltoken[tokenNum].loca=lineCoun;
           alltoken[tokenNum++].value=word;
      }

    }
    else if(word=="get"){  ///含get关键字分离
        int j=i+11;
        for(;i<=j&&i<len;i++)
            word+=line[i];
        if(!isKeyword(word) ||(  i<len && line[i]!='\t'&& line[i]!=' '  ) ){
          printf("词法分析第%d行定义不符合规范！！！\n不能用关键字作为变量，出错词素：",lineCoun);
          outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！不能用关键字作为变量，出错词素："<<word<<endl;
          cout<<word<<endl;
          mark=0;
          while(line[i]!=' ')
            i++;
         }
         else{
          alltoken[tokenNum].name="KEYWORD";
          alltoken[tokenNum].index = word;
          alltoken[tokenNum].loca=lineCoun;
          alltoken[tokenNum++].value=word;
         }
    }
    else if(word=="move"){
       int j=i+2;
        for(;i<=j&&i<len;i++)
            word+=line[i];
        if(!isKeyword(word) ||(  i<len && line[i]!='\t'&& line[i]!=' '  ) ){
          printf("词法分析第%d行定义不符合规范！！！\n不能用关键字作为变量，出错词素：",lineCoun);
          outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！不能用关键字作为变量，出错词素："<<word<<endl;
          cout<<word<<endl;
          mark=0;
          while(line[i]!=' ')
             i++;
        }
        else{
          alltoken[tokenNum].name="KEYWORD";
          alltoken[tokenNum].index = word;
          alltoken[tokenNum].loca=lineCoun;
          alltoken[tokenNum++].value=word;
        }
    }

    else if(isKeyword(word)){  ///关键字
        alltoken[tokenNum].name="KEYWORD";
        alltoken[tokenNum].index = word;
        alltoken[tokenNum].loca=lineCoun;
            alltoken[tokenNum++].value = word;
    }
    else if(isIntNum(word,lineCoun)){
        alltoken[tokenNum].name= "NUM";  ///整型数字
        alltoken[tokenNum].index = "NUM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
        }
    else if(isFloNum(word,lineCoun)){
        alltoken[tokenNum].name= "NUM";   ///浮点型数字
        alltoken[tokenNum].index = "NUM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
        }
    else if(isID(word)){     ///变量
        alltoken[tokenNum].name=word;
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].index = "ID";
        alltoken[tokenNum++].value ="";
    }
    else if(word=="+"||word=="-"||word=="*"||word=="/"||word=="%") {  ///运算符
        alltoken[tokenNum].name="OP";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].value = word;
        alltoken[tokenNum++].index = word;

        }
    else if(word==">"||word=="<" || word=="=="){   ///比较符号
        alltoken[tokenNum].name="COMP_SYM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].index = word ;
        alltoken[tokenNum++].value = word;

        }
    else if(word[0]=='\"'){  ///获取文件名
         while(i<len&&line[i]!=' ')
            word+=line[i++];
         word.erase(0,1);
         word.erase(word.length()-1,1);
         alltoken[tokenNum].value = word;
         alltoken[tokenNum].name = "filename";
         alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].index = "filename";
    }
    else if(word=="="){    ///赋值符号
        alltoken[tokenNum].name="ASS_SYM";
        alltoken[tokenNum].index = word;
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
    }
    else if(word=="("||word==")"){
        alltoken[tokenNum].name="BRA_SYM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].index = word;
        alltoken[tokenNum++].value = word;
    }
    else if(word==";"|| word == ","){
        alltoken[tokenNum].name="SPL_SYM";
        alltoken[tokenNum].index = word;
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
    }
    else {
        printf("词法分析第%d行定义不符合规范！！！出错词素：",lineCoun);
        cout<<word<<endl;
        outToken<<"词法分析第"<<lineCoun<<"行定义不符合规范！！！出错词素："<<word<<endl;
        mark=0;
        while(line[i]!=' ')
            i++;
    }
   i++;
  }
  return mark;
}
scanner::scanner()
{

}

scanner::~scanner()
{
    //dtor
}
