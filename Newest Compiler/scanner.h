#ifndef SCANNER_H
#define SCANNER_H
#include "myclass.h"
#include <iostream>
#include <cstdio>
#include <fstream>
using namespace std;

class scanner
{
    private:
    public:
        scanner();
        virtual ~scanner();
           bool  isOperator(char ch);
        string  lineTrim(string str,int lineCoun);
        bool isKeyword(string word);
        void outtoken(token alltoken[],int tokenNum);
        bool isIntNum(string word,int lineCoun);
        bool isFloNum(string word,int lineCoun);
        bool isID(string word);
        bool scannerFuntion(string line,int lineCoun,token alltoken[],int &tokenNum);
   protected:
};

#endif // SCANNER_H
