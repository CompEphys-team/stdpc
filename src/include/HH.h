#ifndef HH_H
#define HH_H

#include <cmath>
#include "ObjectDataTypes.h"
#include "Channels.h"
#include "LUtables.h"
#include "Global_func.h"

class HH
{
  private:
    mhHHData *p;
    stdpc::function *theExp;
    stdpc::function *theExpSigmoid;
    
  protected:
    double m, h;
    double minf, taum, hinf, tauh;
    double I;
    inChannel *pre;
    outChannel *out;
    CurrentAssignment a;
    
  public:
    HH(mhHHData *p, DCThread *t, CurrentAssignment a);
    void currentUpdate(double, double);

    typedef mhHHData param_type;
};

#endif
