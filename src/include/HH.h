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
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    
  protected:
    double m, h;
    double km[4], kh[4], mi, hi;
    double minf, taum, hinf, tauh;
    double I;
    inChannel *pre;
    outChannel *out;
    CurrentAssignment *a;
    
  public:
    HH(mhHHData *p, CurrentAssignment *a, inChannel *pre, outChannel *out);
    void currentUpdate(double, double);
    void RK4(double, double, size_t);

    typedef mhHHData param_type;
};

#endif
