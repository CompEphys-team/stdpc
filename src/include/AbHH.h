#ifndef ABHH_H
#define ABHH_H

#include "ObjectDataTypes.h"
#include "Channels.h"
#include "LUtables.h"
#include "Global_func.h"

class abHH
{
  private:
    abHHData *p;
    inChannel *pre;
    outChannel *out;
    stdpc::function *theExp;
    stdpc::function *theExpSigmoid;
    stdpc::function *theTanh;
    
  protected:
    double m, h;
    double ma, mb, ha, hb;
    double I;
    
  public:
    abHH(abHHData *p, DCThread *t, CurrentAssignment *a = nullptr);
    void currentUpdate(double, double);
    double mhFunc(double, int);

    typedef abHHData param_type;
};

#endif
