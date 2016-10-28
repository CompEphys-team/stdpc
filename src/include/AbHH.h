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
    CurrentAssignment a;
    stdpc::function *theExp;
    stdpc::function *theExpSigmoid;
    stdpc::function *theTanh;
    
  protected:
    double m, h;
    double ma, mb, ha, hb;
    double I;
    double km[4], kh[4], mi, hi;
    
  public:
    abHH(abHHData *p, DCThread *t, CurrentAssignment a);
    void currentUpdate(double, double);
    double mhFunc(double, int);
    void RK4(double t, double dt, size_t n);

    typedef abHHData param_type;
};

#endif
