#include "AbHH.h"
#include "DCThread.h"

abHH::abHH(abHHData *inp, DCThread *t, CurrentAssignment *a) :
    p(inp),
    pre(t->getInChan(a->VChannel)),
    post(t->getOutChan(a->IChannel)),
    m(0.0),
    h(1.0),
    ma(0.0),
    mb(1.0),
    ha(0.0),
    hb(0.0)
{
    if (p->LUTables) {
        theExp= &expLU;
        expLU.require(-50.0, 50.0, 0.02);
        theExpSigmoid= &expSigmoidLU;
        expSigmoidLU.require(-50.0, 50.0, 0.02);
        theTanh= &tanhLU;
        tanhLU.require(-10.0, 10.0, 0.01);
    }
    else {
        theExp= &expFunc;
        theExpSigmoid= &expSigmoidFunc;
        theTanh= &tanhFunc;
    }
}

inline double abHH::mhFunc(double x, int thetype)
{
   switch (thetype) {
    case 0:
         return x/((*theExp)(x)-1.0);
         break;
    case 1:
         return (*theExp)(x);
         break;
    case 2:
         return (*theExpSigmoid)(x);
         break;
   }
   return 0.0;
}

void abHH::currentUpdate(double t, double dt) 
{
  static double powm, powh;
  static double V;
  static int i;
  
  if (p->active) {
    V= pre->V;
    if (p->mExpo > 0) {
      // Linear Euler:
      m+= (ma*(1.0-m)-mb*m)*dt;    // calculate m with previous values of ma, m, mb
      ma= p->mka*mhFunc((V-p->mVa)/p->msa, p->maFunc);
      mb= p->mkb*mhFunc((V-p->mVb)/p->msb, p->mbFunc);
      powm= m;
      for (i= 0; i < p->mExpo-1; i++) powm*= m;
    }
    else powm= 1.0;
    
    if(p->hExpo > 0) {
      // Linear Euler:
      h+= (ha*(1.0-h)-hb*h)*dt;     // calculate h with previous values of ha, h, hb
      ha= p->hka*mhFunc((V-p->hVa)/p->hsa, p->haFunc);
      hb= p->hkb*mhFunc((V-p->hVb)/p->hsb, p->hbFunc);
      powh= h;
      for (i= 0; i < p->hExpo-1; i++) powh*= h;
    }
    else powh= 1.0;
    I= p->gMax*powm*powh*(p->Vrev-V);
    out->I+= I;
  }
}
