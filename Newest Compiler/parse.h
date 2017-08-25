#ifndef PARSE_H
#define PARSE_H
#include "myclass.h"
#include <iostream>
#include <stack>

class parse
{
    public:
        parse();
        virtual ~parse();
         void  printTree (node *p);
        bool  function(token alltoken[],int tokenNum,stack <node*> &nodeSta,stack <token> sta,node * &root);
        void printStack(stack <token> ssta);
    protected:
    private:
};

#endif // PARSE_H
