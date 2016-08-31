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
    
  public:
    HH(mhHHData *p, DCThread *t, CurrentAssignment *a = nullptr);
    void currentUpdate(double, double);

    static void instantiate(std::vector<HH> &, mhHHData *, DCThread *, CurrentAssignment *);
};

#endif
