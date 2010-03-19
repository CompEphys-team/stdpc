#ifndef HH_H
#define HH_H

#include "ObjectDataTypes.h"
#include "Channels.h"
#include "LUtables.h"
#include "Global_func.h"

class HH
{
  private:
    mhHHData *p;
    function *theExp;
    function *theExpSigmoid;
    
  protected:
    double m, h;
    double minf, taum, hinf, tauh;
    double I;
    inChannel *pre;
    outChannel *out;
    
  public:
    HH();
    void init(mhHHData *, short int *, short int *, inChannel *, outChannel *);
    void currentUpdate(double, double);
};

#endif
