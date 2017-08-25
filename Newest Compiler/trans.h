#ifndef TRANS_H
#define TRANS_H
#include "myclass.h"

class trans
{
    public:
        trans();
        virtual ~trans();
        void   printID();
        void  printResult(bool b);
        void  transFunction(node *&root);
        bool  getStandarExp(node * root);
        string getFloString(float flo);
    private:
};

#endif // TRANS_H
