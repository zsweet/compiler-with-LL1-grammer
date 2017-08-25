#include "trans.h"
#include "myclass.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
using namespace std;

ofstream  outCommand("paint/paint.txt",ios::out);///���ʷ������ķ�����Ĵ���д��Token.txt��
ofstream  outID("source/idTable.txt",ios::out);  ///���������idTable.txt

IDTable idTable;

int markDataType = 0;

void trans:: printResult(bool b){
if(b==1)  cout<<"\n\n======================================����ɹ�==================================\n";
else  cout<<"\n\n======================================����ʧ��==================================\n";
}

int findIDIndex(string str){
for(int i=0;i<idTable.idnum;i++)
    if(idTable.id[i].name==str)
      return i;
return -1;
}

string judgeIntOrFloat(string num){
    int len = num.length();
    for(int i=0;i<len;i++)
      if(num[i]=='.')
         return "float";
    return "int";
}

string trans:: getFloString(float flo)
{
    ofstream  outt("temp.txt",ios::out);
    ifstream inn("temp.txt");
    string temp;
    outt<<flo;
    outt.close();
    if(getline(inn,temp))
        return temp;
    else
    {
        cout<<"ת�����ִ��󣡣�������ֹͣ����";
        printResult(0);
        exit(0);
    }
}

bool trans :: getStandarExp(node * root){  ///Term Expr2 || Factor Term2
   int i=0;//name
   int len = root->child[1]->tokenn.value.length() ;
   string str = root->child[1]->tokenn.value; ///��벿������ʽ
   if(root->child[0]->tokenn.name=="float"||root->child[1]->tokenn.name=="float")
     for(int i=0;i<len;i++)
        if(str[i]=='%'){
           cout<<"��error����"<<root->tokenn.loca<<"�����巭����ָ�����ȡ��"<<endl;
           return 0;
        }
   float  x = atof(root->child[0]->tokenn.value.c_str());
   while(i<len){
   	 char op=str[i];
   	 i++;
     string temp="";
     int tem,strnu;
     while( i<len && (((str[i]<='9'&&str[i]>='0')||str[i]=='.')||(temp==""&&str[i]=='-')) )
        temp+=str[i++];
     switch(op){
     	case '+': x=x+atof(temp.c_str()); break;
     	case '-': x=x-atof(temp.c_str()); break;
     	case '*': x=x*atof(temp.c_str()); break;
     	case '/':
     	    if(temp=="0") {
                cout<<"��error�����д���,��ĸΪ0������";
                printResult(0);
                exit(0);
     	    }
     	    x=x/atof(temp.c_str()); break;
     	case '%':
     	    tem=(int)x,strnu=atof(temp.c_str());
     	    x=tem%strnu; break;
     }
  }
   root->tokenn.name=((root->child[0]->tokenn.name=="float"||root->child[1]->tokenn.name=="float")?"float":"int");
   root->tokenn.value=getFloString(x);
   return 1;
}

void trans :: printID()
{
    for(int i=0; i<idTable.idnum; i++)
        outID<<idTable.id[i].name<<"     "<<idTable.id[i].value<<"     "<<idTable.id[i].intOrFlo<<endl;
    outID<<"========================================="<<endl<<endl;
}

void getCompExp(node *&root){   ///Expr Comp-OP Expr | true |false


    string leftExpTyp = root->child[0]->tokenn.name;  ///������ʽֵ����
    string righExpTyp = root->child[2]->tokenn.name;  ///������ʽֵ����
    if(leftExpTyp != righExpTyp)
      cout<<"��wearning����"<<root->tokenn.loca<<"�� :��������������Ƚ�ʱ"<<root->child[1]->tokenn.value<<"�����ұȽ�ֵ���Ͳ�ͬ,��ǿ������ת������\n" ;

  /// > ���� <
        float leftValue = atof(root->child[0]->tokenn.value.c_str());
        float rightValue = atof(root->child[2]->tokenn.value.c_str());

        if(root->child[1]->tokenn.value == "<")
            (leftValue < rightValue) ? (root->tokenn.value = "true") : root->tokenn.value ="false";
        else if(root->child[1]->tokenn.value == ">")
            (leftValue > rightValue) ? root->tokenn.value= "true" : root->tokenn.value ="false";
        else{
            if((leftValue-rightValue)>-0.0000000001&&(leftValue-rightValue)<0.0000000001 )
                root->tokenn.value = "true";
            else
               root->tokenn.value = "false";
        }

}

void  trans::transFunction(node *&root){
if(root->tokenn.index=="program"){
    transFunction(root->child[1]);
}
else if(root->tokenn.index=="program2"){
    if(root->childNum>=2) ///Stmt-List end
    transFunction(root->child[0]);
}
else if(root->tokenn.index=="Stmt-List"){
    transFunction(root->child[0]);///Stmt
    transFunction(root ->child[1]);/// Stmt-List2
}
else if(root->tokenn.index=="Stmt-List2"){
    if(root->childNum>=2){ ///Stmt Stmt-List2
      transFunction(root->child[0]);
      transFunction(root->child[1]);
    }
}
else if(root->tokenn.index=="Stmt"){
    transFunction(root->child[0]); ///���־���
    root->tokenn.loca=root->child[0]->tokenn.loca ;
}
else if(root->tokenn.index=="Assign-Stmt"){ ///  ID = Expr
    root->tokenn.loca=root->child[0]->tokenn.loca ;
    int idIndex= findIDIndex(root->child[0]->tokenn.name);
    if(idIndex==-1){
      printf("��error����%d�� ����%sδ��������ʹ�ã��Ƿ���ֵ������\n",root->tokenn.loca,root->child[0]->tokenn.name.c_str());
      printResult(0);
      exit(0);
    }
    transFunction(root->child[2]);
    if(idTable.id[idIndex].intOrFlo == root->child[2]->tokenn.name){ ///��������������ͬ
         float xxx= atof(root->child[2]->tokenn.value.c_str());
         if(idTable.id[idIndex].intOrFlo=="int"&&xxx>0x7fffffff){
           cout<<"��wearning����"<<root->tokenn.loca<<"�� :��ʽ�ұ���ֵ�������͵����ֵ"<<endl;
         int temp = xxx;
         char ch[20] ;
         itoa(temp,ch,10);
         root->child[0]->tokenn.value = ch;
         }
         else{
         root->child[0]->tokenn.value = root->child[2]->tokenn.value;
         idTable.id[idIndex].value = root->child[2]->tokenn.value;
         }
    }
    else if( idTable.id[idIndex].intOrFlo == "float"  &&  root->child[2]->tokenn.name == "int" ){  ///float = int
            cout<<"��wearning����"<<root->tokenn.loca<<"�� :��ֵ�����=�����������Ͳ���ͬ����ǿ������ת��"<<endl;
        root->child[0]->tokenn.value = root->child[2]->tokenn.value;
        idTable.id[idIndex].value = root->child[2]->tokenn.value;
    }
    else {   ///int = float ;
      cout<<"��wearning����"<<root->tokenn.loca<<"�� :��ֵ�����=�����������Ͳ���ͬ����ǿ������ת��"<<endl;
      float xxx= atof(root->child[2]->tokenn.value.c_str());
      if(xxx>0x7fffffff)
         cout<<"��wearning����"<<root->tokenn.loca<<"�� :��ʽ�ұ���ֵ�������͵����ֵ"<<endl;
      int temp = xxx;
      char str[100];
      itoa(temp,str,10);
      root->child[0]->tokenn.value = str;
      idTable.id[idIndex].value  = str;
    }
  printID();
}
else if(root->tokenn.index=="If-Stmt"){  ///if ( Compare-Exp ) Stmt-List If-Stmt2
    transFunction(root->child[2]);
    root->tokenn.loca=root->child[0]->tokenn.loca ;
    if(root->child[2]->tokenn.value == "true")
        transFunction(root->child[4]);
    else {
        if(root->child[5]->childNum==3) ///If-stmt2
          transFunction(root->child[5]);
    }
}
else if(root->tokenn.index=="If-Stmt2"){  ///end | else Stmt-List end
    if(root->childNum==3){  ///else Stmt-List end
        transFunction(root->child[1]);
        root->tokenn.loca=root->child[1]->tokenn.loca ;
    }
}
else if(root->tokenn.index=="Declare-Stmt"){ /// int ID-List ; | float ID-List;
    root->tokenn.loca=root->child[0]->tokenn.loca ;
    if(root->child[0]->tokenn.value == "int"){
        markDataType=0;
        transFunction(root->child[1]);
    }
    if(root->child[0]->tokenn.value == "float"){
        markDataType=1;
        transFunction(root->child[1]);
    }
}
else if(root->tokenn.index=="ID-List"){ /// ID ID-List2
    if(findIDIndex(root->child[0]->tokenn.name)==-1){
    if(markDataType==0){  ///��������
        idTable.id[idTable.idnum].name=root->child[0]->tokenn.name;
        idTable.id[idTable.idnum].value = "";
        idTable.id[idTable.idnum++].intOrFlo="int";

        root->child[0]->tokenn.value = "";  ///�˴���ʵ������///��������ֵ����Ϊδ��ʼ��
        transFunction(root->child[1]);
        root->child[1]->tokenn.value=="@"?(root->tokenn.loca=root->child[0]->tokenn.loca):(root->tokenn.loca=root->child[1]->tokenn.loca);
    }
    else{  ///��������
        idTable.id[idTable.idnum].name=root->child[0]->tokenn.name;
        root->child[0]->tokenn.value = "";  ///��������ֵ����Ϊδ��ʼ��
        idTable.id[idTable.idnum++].intOrFlo="float";

        root->child[0]->tokenn.value = "";///�˴���ʵ������
        transFunction(root->child[1]);
        root->child[1]->tokenn.value=="@"?(root->tokenn.loca=root->child[0]->tokenn.loca):(root->tokenn.loca=root->child[1]->tokenn.loca);
        }
    }
    else {cout<<"��wearning����"<<root->child[0]->tokenn.loca<<"�� ������������б����ظ��������ظ�������Ч������\n";}
}
else if(root->tokenn.index=="ID-List2"){ ///, ID ID-List2 | ��
    if(root->childNum!=1)  ///��Ϊ�մ�
      { ///, ID ID-List2
        if(markDataType==0){  ///��������
        idTable.id[idTable.idnum].name=root->child[1]->tokenn.name;
        idTable.id[idTable.idnum].value = "";
        idTable.id[idTable.idnum++].intOrFlo = "int";

        root->child[1]->tokenn.value = "";  ///��������ֵ����Ϊδ��ʼ��  ///�˴���ʵ������
        root->tokenn.loca = root->child[1]->tokenn.loca;
        root->tokenn.name = "int";
        transFunction(root->child[2]);
        root->child[2]->tokenn.value=="@"?(root->tokenn.loca=root->child[1]->tokenn.loca):(root->tokenn.loca=root->child[2]->tokenn.loca);
        }
        else{  ///��������
          idTable.id[idTable.idnum].name=root->child[1]->tokenn.name;
          idTable.id[idTable.idnum].value = "";
          idTable.id[idTable.idnum].intOrFlo = "float";

          root->child[1]->tokenn.value = "";  ///��������ֵ����Ϊδ��ʼ��  ///�˴���ʵ������
          root->tokenn.loca = root->child[1]->tokenn.loca;
          root->tokenn.name = "float";
          transFunction(root->child[2]);
          root->child[2]->tokenn.value=="@"?(root->tokenn.loca=root->child[1]->tokenn.loca):(root->tokenn.loca=root->child[2]->tokenn.loca);
        }
    }
    else{
        root->tokenn.value = "@";
    }

}
else if(root->tokenn.index=="Repeat-Stmt"){ ///while ( Compare-Exp ) Stmt-List end
    root->tokenn.loca=root->child[0]->tokenn.loca;
    transFunction(root->child[2]);
    while(root->child[2]->tokenn.value=="true"){
        transFunction(root->child[4]);
        transFunction(root->child[2]);
    }
}
else if(root->tokenn.index=="Compare-Exp"){   ///Expr Comp-OP Expr | true |false
    if(root->childNum==1){
        if(root->child[0]->tokenn.value=="true")
            root->tokenn.value = "true", root->tokenn.name="KEYWORD"  ,root->tokenn.loca=root->child[0]->tokenn.loca;
        else if(root->child[0]->tokenn.value=="false")
            root->tokenn.value = "false", root->tokenn.name="KEYWORD"  ,root->tokenn.loca=root->child[0]->tokenn.loca;
    }
    else    ///Expr Comp-OP Expr
    {
         transFunction(root->child[0]);
         transFunction(root->child[1]);
         transFunction(root->child[2]);
         root->tokenn.loca=root->child[2]->tokenn.loca;
         getCompExp(root);
    }
}
else if(root->tokenn.index=="Command"){
    transFunction(root->child[0]);
    root->tokenn.loca = root->child[0]->tokenn.loca;
}
else if(root->tokenn.index=="Expr"){   ///Term Expr2
    transFunction(root->child[0]);
    transFunction(root->child[1]);
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[0]->tokenn.name=="float"||root->child[1]->tokenn.name=="float")
       root->tokenn.name = "float";
      else root->tokenn.name="int";

    if(root->child[1]->tokenn.value=="@"){ ///�ڶ������ʽΪ��
        root->tokenn.loca = root->child[0]->tokenn.loca;
        root->tokenn.value = root->child[0]->tokenn.value;
    }

    else {///Expr2��Ϊ��
      if(getStandarExp(root)==0) {
            printResult(0);
             exit(0);
        }
    }
}
else if(root->tokenn.index=="Expr2"){  ///�õ��ı��ʽ��ʽΪ +8-9+10...
    if(root->childNum==3){  ///Add-OP Term Expr2
      transFunction(root->child[0]);
      transFunction(root->child[1]);
      transFunction(root->child[2]);
      root->tokenn.loca = root->child[1]->tokenn.loca;

      if(root->child[1]->tokenn.name=="float"||root->child[2]->tokenn.name=="float")
       root->tokenn.name = "float";
      else root->tokenn.name="int";

      root->tokenn.value = root->child[0]->tokenn.value + root->child[1]->tokenn.value ;
      if(root->child[2]->tokenn.value!="@"){  ///���������ʽ��Ϊ��
        root->tokenn.loca = root->child[2]->tokenn.loca;
        root->tokenn.value = root->tokenn.value+root->child[2]->tokenn.value;
        if(root->child[2]->tokenn.name == "float")  root->tokenn.name = "float";
      }
    }
    else
        root->tokenn.value="@";
}
else if(root->tokenn.index=="Term"){ //Factor Term2
    transFunction(root->child[0]);
    transFunction(root->child[1]);
    if(root->child[0]->tokenn.name=="float"||root->child[1]->tokenn.name=="float")
       root->tokenn.name = "float";
    else root->tokenn.name="int";
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="@"){
       root->tokenn.value = root->child[0]->tokenn.value;
    }
    else ///term2��Ϊ��
        if(getStandarExp(root)==0) {printResult(0);  exit(0);} ///��õ�ǰ�ڵ�ֵ
}
else if(root->tokenn.index=="Term2"){  ///Multi-OP Factor Term2   �õ��ı��ʽ��ʽΪ *8/9*10...
    if(root->childNum==3){  ///Multi-OP Factor Term2
      transFunction(root->child[0]);
      transFunction(root->child[1]);
      transFunction(root->child[2]);
      root->tokenn.loca = root->child[0]->tokenn.loca;
      root->tokenn.value = root->child[0]->tokenn.value + root->child[1]->tokenn.value;
      root->tokenn.name = root->child[1]->tokenn.name;
      if(root->child[2]->tokenn.value!="@"){
        root->tokenn.loca = root->child[2]->tokenn.loca;
        root->tokenn.value = root->tokenn.value+root->child[2]->tokenn.value;
        if(root->child[2]->tokenn.name == "float")  root->tokenn.name = "float";
      }
    }
    else
        root->tokenn.value="@";
}
else if(root->tokenn.index=="Factor"){
     if(root->childNum==3){  ///( Expr )
        transFunction(root->child[1]);
        root->tokenn.loca = root->child[0]->tokenn.loca;
        root->tokenn.value = root->child[1]->tokenn.value;
        root->tokenn.name = root->child[1]->tokenn.name;
     }
     else{ ///  ID-NUM
        transFunction(root->child[0]);
        root->tokenn.loca = root->child[0]->tokenn.loca;
        root->tokenn.value = root->child[0]->tokenn.value;
        root->tokenn.name = root->child[0]->tokenn.name;
     }
}
else if(root->tokenn.index=="ID-NUM"){
   if(root->child[0]->tokenn.index=="ID"){   ///ID
      root->tokenn.loca = root->child[0]->tokenn.loca;
      int index = findIDIndex(root->child[0]->tokenn.name);
      if(index==-1){  ///δ���ҵ��ñ���
         cout<<"��error����"<<root->tokenn.loca<<"�� �����ñ���δ����"<<endl;
         printResult(0);
         exit(0);
      }
      else {///���ҵ��ñ���
            if(idTable.id[index].value == ""){
               cout<<"��wearning����"<<root->tokenn.loca<<"�� ����"<<idTable.id[index].name<<"δ����ʼ��,�������ѽ��ñ�����ֵĬ��Ϊ0��������δ��ʼ��"<<endl;
               root->tokenn.value = "0";
               ///idTable.id[index];
           }
          else ///�ܲ��ҵ��ñ��������Ѿ���ʼ��
             root->tokenn.value = idTable.id[index].value;
    }
    root->tokenn.name = idTable.id[index].intOrFlo;
   }
   else if(root->child[0]->tokenn.value=="-"){  ///- ID-NUM2
      transFunction(root->child[1]);
      root->tokenn.loca = root->child[1]->tokenn.loca;
      root->tokenn.value = "-"+root->child[1]->tokenn.value;
      root->tokenn.name =  root->child[1]->tokenn.name;
    }
   else{  /// NUM
      root->tokenn.loca = root->child[0]->tokenn.loca;
      root->tokenn.value = root->child[0]->tokenn.value;

      root->tokenn.name = judgeIntOrFloat(root->child[0]->tokenn.value);
   }
}
else if(root->tokenn.index=="ID-NUM2"){  ///NUM | ID

    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[0]->tokenn.index=="ID"){   ///ID
      int index = findIDIndex(root->child[0]->tokenn.name);
      root->tokenn.name = idTable.id[index].intOrFlo;

      if(index==-1){  ///δ���ҵ��ñ���
         cout<<"��error����"<<root->tokenn.loca<<"�� �����ñ���δ����"<<endl;
         printResult(0);
         exit(0);
      }
      else {///���ҵ��ñ���
            if(idTable.id[index].value == ""){
               cout<<"��wearning����"<<root->tokenn.loca<<"�� ����"<<idTable.id[index].name<<"δ����ʼ��,�������ѽ��ñ�����ֵĬ��Ϊ0��������δ��ʼ��"<<endl;
               root->tokenn.value = "0";
           }
          else ///�ܲ��ҵ��ñ��������Ѿ���ʼ��
             root->tokenn.value = idTable.id[index].value;              ///����ǿ��ת��δ���
      }
   }
   else{   ///NUM
      root->tokenn.name =  judgeIntOrFloat(root->child[0]->tokenn.value);
      root->tokenn.value = root->child[0]->tokenn.value;
    }
}
else if(root->tokenn.index=="Add-OP"){
    root->tokenn.value=root->child[0]->tokenn.value;
    root->tokenn.loca = root->child[0]->tokenn.loca;
}
else if(root->tokenn.index=="Multi-OP"){
    root->tokenn.value=root->child[0]->tokenn.value;
    root->tokenn.loca = root->child[0]->tokenn.loca;
}
else if(root->tokenn.index=="Comp-OP"){
    root->tokenn.value=root->child[0]->tokenn.value;
    root->tokenn.loca = root->child[0]->tokenn.loca;
}
else if(root->tokenn.index=="File-Com"){  /// file File-Com2
    transFunction(root->child[1]);
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value !="" )  /// ��·������
        outCommand<<root->child[0]->tokenn.value<<":"<<root->child[1]->tokenn.value<<";"<<endl;  ///���ļ���������file������ļ�·��
    else
        outCommand<<root->child[0]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="File-Com2"){
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[0]->tokenn.index == "filename"){
        root->tokenn.value=root->child[0]->tokenn.value;
    }
    else
        root->tokenn.value ="";
}
else if(root->tokenn.index=="Delay-Com"){  ///delay Param-1
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"delay"<<";"<<endl;
    else
        outCommand<<"delay "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Set-Window-Com"){  ///set window size Param-1
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"set window size"<<";"<<endl;
    else
        outCommand<<"set window size "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Set-Color-Com"){  ///set color Param-3
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    outCommand<<"set color "<<root->child[1]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="Set-PointSiz-Com"){  ///  set point size Param-1
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"set point size"<<";"<<endl;
    else
        outCommand<<"set point size "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Set-LineWid-Com"){  ///   set line width Param-1
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"set point size"<<";"<<endl;
    else
        outCommand<<"set point size "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Point-Com"){  ///point Param-2
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"point"<<";"<<endl;
    else
        outCommand<<"point "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Scale-Com"){  ///scale Param-2
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"scale"<<";"<<endl;
    else
        outCommand<<"scale "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Translate-Com"){  ///translate Param-2
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[0]->tokenn.loca;
    if(root->child[1]->tokenn.value=="")
        outCommand<<"translate"<<";"<<endl;
    else
        outCommand<<"translate "<< root->child[1]->tokenn.value <<";"<<endl;
}
else if(root->tokenn.index=="Reset-Com"){  ///reset
    root->tokenn.loca = root->child[0]->tokenn.loca;
    outCommand<<"reset"<<";"<<endl;
}
else if(root->tokenn.index=="Get-windsiz-Com"){  ///get window size
    root->tokenn.loca = root->child[0]->tokenn.loca;
    outCommand<<"get window size"<<";"<<endl;
}
else if(root->tokenn.index=="MoveTo-Com"){  ///move to ID-NUM,ID-NUM
    transFunction(root->child[1] );
    transFunction(root->child[3] );
    root->tokenn.loca = root->child[3]->tokenn.loca;
    outCommand<<"move to "<<root->child[1]->tokenn.value<<","<<root->child[3]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="LineTo-Com"){  ///line to ID-NUM,ID-NUM
    transFunction(root->child[1] );
    transFunction(root->child[3] );
    root->tokenn.loca = root->child[3]->tokenn.loca;
    outCommand<<"line to "<<root->child[1]->tokenn.value<<","<<root->child[3]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="Line-Com"){  ///line ID-NUM,ID-NUM,ID-NUM,ID-NUM
    transFunction(root->child[1] );
    transFunction(root->child[3] );
    transFunction(root->child[5] );
    transFunction(root->child[7] );
    root->tokenn.loca = root->child[7]->tokenn.loca;
    outCommand<<"line "<<root->child[1]->tokenn.value<<","<<root->child[3]->tokenn.value<<","<<root->child[5]->tokenn.value<<","<<root->child[7]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="Rotate-Com"){  ///rotate ID-NUM
    transFunction(root->child[1] );
    root->tokenn.loca = root->child[1]->tokenn.loca;
    outCommand<<"rotate "<<root->child[1]->tokenn.value<<";"<<endl;
}
else if(root->tokenn.index=="Help-Com"){  ///help
    outCommand<<"help"<<";"<<endl;
    root->tokenn.loca = root->child[0]->tokenn.loca;
}
else if(root->tokenn.index=="Param-1"){
   if( root->child[0]->tokenn.index == "@" )
       root->tokenn.value="",root->tokenn.loca=-1;
   else
   {
     transFunction(root->child[0]);
     root->tokenn.value =  root->child[0]->tokenn.value;
     root->tokenn.loca = root->child[0]->tokenn.loca;
     if(root->child[0]->tokenn.name=="float")  root->tokenn.name="float";
     else  root->tokenn.name="int";
   }
}
else if(root->tokenn.index=="Param-2"){
    if( root->child[0]->tokenn.index =="@" )
        root->tokenn.value="",root->tokenn.loca=-1;
    else{
        transFunction(root->child[0]);
        transFunction(root->child[2]);
        root->tokenn.value = root->child[0]->tokenn.value+","+root->child[2]->tokenn.value;
        root->tokenn.loca = root->child[2]->tokenn.loca;
        if(root->child[0]->tokenn.name=="float"||root->child[2]->tokenn.name=="float")
            root->tokenn.name="float";
        else
            root->tokenn.name="int";
    }
}
else if(root->tokenn.index=="Param-3"){
   if( root->child[0]->tokenn.index =="@" )
        root->tokenn.value="",root->tokenn.loca=-1;
    else{
        transFunction(root->child[0]);
        transFunction(root->child[2]);
        transFunction(root->child[4]);
        root->tokenn.value = root->child[0]->tokenn.value+","+root->child[2]->tokenn.value+","+root->child[4]->tokenn.value;
        if(root->child[0]->tokenn.name=="float"||root->child[2]->tokenn.name=="float"||root->child[4]->tokenn.name=="float")
            root->tokenn.name="float";
        else
            root->tokenn.name="int";
        root->tokenn.loca = root->child[4]->tokenn.loca;
    }
}
else{
    cout<<"������,����Ҫ���ҵķ��ս��Ϊ��"<<root->tokenn.index<<"��"<<root->tokenn.loca<<"��"<<endl;
    printResult(0);
    exit(0);
}
}

trans::trans()
{
  outID<<"===================������=====================\nName value type\n\n";
}
trans::~trans()
{

}
