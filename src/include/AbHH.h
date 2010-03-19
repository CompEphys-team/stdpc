#ifndef ABHH_H
#define ABHH_H

#include "ObjectDataTypes.h"
#include "Channels.h"
#include "LUtables.h"
#include "global_func.h"

class abHH
{
  private:
    int initial;
    abHHData *p;
    inChannel *pre;
    outChannel *out;
    function *theExp;
    function *theExpSigmoid;
    function *theTanh;
    
  protected:
    double m, h;
    double ma, mb, ha, hb;
    double I;
    
  public:
    abHH();
    void init(abHHData *, short int *, short int*, inChannel *, outChannel *);
    void currentUpdate(double, double);
    double mhFunc(double, int);
};

#endif
