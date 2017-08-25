#include "scanner.h"
#include "myclass.h"
#include <iostream>
#include <cstdio>
#include <iomanip>
#include<stdlib.h>
#include <fstream>
using namespace std;
ofstream  outToken("source/Token.txt",ios::out);///���ʷ������ķ�����Ĵ���д��Token.txt��
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
   outToken<<"�ʷ������ɹ�ȫ������Ϊ��\n"<<"(������        ����        ֵ      ����Դ�ļ���)"<<endl;
   printf("�ʷ�����ȫ������Ϊ��\n((%-14s,%-10s,%-5s,%s)\n","������","����","ֵ","����Դ�ļ���");


   for(int i=0;i<tokenNum;i++){
     printf("(%-14s,%-10s,%-5s,%d)\n",alltoken[i].index.c_str(),alltoken[i].name.c_str(),alltoken[i].value.c_str(),alltoken[i].loca );
     outToken<<setiosflags(ios::fixed)<<setiosflags(ios::left)<<"("<<setw(14)<<alltoken[i].index<<setw(1)<<","<<setw(10)<<alltoken[i].name<<setw(1)<<","<<setw(10)<<alltoken[i].value<<setw(1)<<','<<alltoken[i].loca<<")"<<endl;
   }
}
string scanner::lineTrim(string str,int lineCoun)///����Ϊ��׼ģʽ
{
    int len = str.length();
    string temp="";
    for(int i=0; i<len; i++)
    {
      if(  (temp.length()==0&&str[i]==' ') || ( temp.length()>0&&(temp[temp.length()-1]==' '||temp[temp.length()-1]=='\t')&&(str[i]==' '||str[i]=='\t') )   )///ȥ���ո���Ʊ��
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
            printf("�ʷ�������%d�ж��岻���Ϲ淶������\"\"��·��ȱ����\"\n",lineCoun);
            outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶������\"\"��·��ȱ����\""<<endl;
            mark=0;
            while(str[i]!=' ')
              i++;
        }
      }
      else if(str[i]=='/'  ){ /// Ϊע��
          if(i+1<len&&str[i+1]=='/')
            return temp;
          else{  ///����Ϊ����
            if((temp.length()!=0 && temp[temp.length()-1]!=' '))
            temp+=" ";
            temp=temp+str[i]+" ";
          }
      }
      else if( isOperator(str[i]) ||  str[i]=='(' || str[i]==')' || str[i]=='<'  || str[i]=='>' ||str[i]==',' ){     ///������� (   )  >   <
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
            printf("�ʷ�������%d�ж��岻���Ϲ淶�������������������ֿ�ͷ��������أ�",lineCoun);
            outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������������ֿ�ͷ��������أ�"<<word<<endl;
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
            printf("�ʷ�������%d�ж��岻���Ϲ淶�������������������ֿ�ͷ��������أ�",lineCoun);
            outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������������ֿ�ͷ��������أ�"<<word<<endl;
            cout<<word<<endl;
           mark=0;
        }
        else
            return false;
    if(word[0]=='.'||word[len-1]=='.')
    {
        printf("�ʷ�������%d�ж��岻���Ϲ淶�����������в������ַ���������أ�",lineCoun);
        outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�������������������ֿ�ͷ��������أ�"<<word<<endl;
        cout<<word<<endl;
        mark=0;
    }
    return true;
}
bool scanner::isID(string word){ ///�ж��Ƿ���ϱ�������
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
  if(line=="\n"||line==" "||line=="\t"||line=="") ///�հ��в�������
     return 1;
  /// cout<<line<<endl;  ///����
  int i=0,len = line.length();

  while(i<len){

    string word="";
    while( line[i]!=' '&&line[i]!='\t' && i<len )
      word+=line[i++];

    if(word=="set"){///��ð���set�ַ���ָ�
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
          printf("�ʷ�������%d�ж��岻���Ϲ淶�����������ùؼ�����Ϊ������������أ�",lineCoun);
          outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������ùؼ�����Ϊ������������أ�"<<word<<endl;
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
    else if(word=="line"){///��ð���line�ַ���ָ�;
      if(i+2<len&&word+line[i]+line[i+1]+line[i+2]=="line to" ){ ///��ǰ�ַ�Ϊline to
         if( i+3>=len ||(line[i+3]=='\t'||line[i+3]==' ') ){  ///��֤line to ���������û��������ĸ���еĻ�˵������line to,ʹ���˹ؼ��ֱ���
          word=word+line[i]+line[i+1]+line[i+2];
          i+=3;
           alltoken[tokenNum].name="KEYWORD";
           alltoken[tokenNum].index = word;
           alltoken[tokenNum].loca=lineCoun;
           alltoken[tokenNum++].value=word;
         }
         else{
              cout<<(i+3>=len)<<(line[i+3]=='\t')<<(line[i+3]==' ')<<endl;
              printf("a�ʷ�������%d�ж��岻���Ϲ淶������\n�����ùؼ�����Ϊ������������أ�",lineCoun);
              outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������ùؼ�����Ϊ������������أ�"<<word<<endl;
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
    else if(word=="get"){  ///��get�ؼ��ַ���
        int j=i+11;
        for(;i<=j&&i<len;i++)
            word+=line[i];
        if(!isKeyword(word) ||(  i<len && line[i]!='\t'&& line[i]!=' '  ) ){
          printf("�ʷ�������%d�ж��岻���Ϲ淶������\n�����ùؼ�����Ϊ������������أ�",lineCoun);
          outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������ùؼ�����Ϊ������������أ�"<<word<<endl;
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
          printf("�ʷ�������%d�ж��岻���Ϲ淶������\n�����ùؼ�����Ϊ������������أ�",lineCoun);
          outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶�����������ùؼ�����Ϊ������������أ�"<<word<<endl;
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

    else if(isKeyword(word)){  ///�ؼ���
        alltoken[tokenNum].name="KEYWORD";
        alltoken[tokenNum].index = word;
        alltoken[tokenNum].loca=lineCoun;
            alltoken[tokenNum++].value = word;
    }
    else if(isIntNum(word,lineCoun)){
        alltoken[tokenNum].name= "NUM";  ///��������
        alltoken[tokenNum].index = "NUM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
        }
    else if(isFloNum(word,lineCoun)){
        alltoken[tokenNum].name= "NUM";   ///����������
        alltoken[tokenNum].index = "NUM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].value = word;
        }
    else if(isID(word)){     ///����
        alltoken[tokenNum].name=word;
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].index = "ID";
        alltoken[tokenNum++].value ="";
    }
    else if(word=="+"||word=="-"||word=="*"||word=="/"||word=="%") {  ///�����
        alltoken[tokenNum].name="OP";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].value = word;
        alltoken[tokenNum++].index = word;

        }
    else if(word==">"||word=="<" || word=="=="){   ///�ȽϷ���
        alltoken[tokenNum].name="COMP_SYM";
        alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum].index = word ;
        alltoken[tokenNum++].value = word;

        }
    else if(word[0]=='\"'){  ///��ȡ�ļ���
         while(i<len&&line[i]!=' ')
            word+=line[i++];
         word.erase(0,1);
         word.erase(word.length()-1,1);
         alltoken[tokenNum].value = word;
         alltoken[tokenNum].name = "filename";
         alltoken[tokenNum].loca=lineCoun;
        alltoken[tokenNum++].index = "filename";
    }
    else if(word=="="){    ///��ֵ����
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
        printf("�ʷ�������%d�ж��岻���Ϲ淶������������أ�",lineCoun);
        cout<<word<<endl;
        outToken<<"�ʷ�������"<<lineCoun<<"�ж��岻���Ϲ淶������������أ�"<<word<<endl;
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
