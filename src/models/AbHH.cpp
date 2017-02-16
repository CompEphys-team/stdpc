#include "AbHH.h"
#include "DCThread.h"

abHH::abHH(abHHData *inp, CurrentAssignment *a, inChannel *pre, outChannel *out) :
    p(inp),
    pre(pre),
    out(out),
    a(a),
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

    // Can't do this in the initialiser list, whyeverthefuck
    mi = m;
    hi = h;
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
  
  if (p->active && a->active && pre->active && out->active) {
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

void abHH::RK4(double, double dt, size_t n)
{
    static double powm, powh;
    static double V;
    static int i;

    if (p->active && a->active && pre->active && out->active) {
      V = pre->V;
      if (p->mExpo > 0) {
          /// Calculate km[n]=dm/dt based on the previous intermediates (Vi, mi)
          ma= p->mka*mhFunc((V-p->mVa)/p->msa, p->maFunc);
          mb= p->mkb*mhFunc((V-p->mVb)/p->msb, p->mbFunc);
          km[n] = (ma*(1.0-mi)-mb*mi);

          /// Calculate output based on (still) the previous intermediate mi (to allow calculating kV[n]=dV/dt)
          powm = mi;
          for (i= 0; i < p->mExpo-1; i++) powm*= mi;

          /// Create a new intermediate mi(="m_n") based on km[n]=dm/dt and m0, the initial value of m in this RK cycle
          if ( n < 3 ) {
              mi = m + km[n]*dt;
          } else {
              mi = m + (km[0] + 2*km[1] + 2*km[2] + km[3]) * dt / 6;
          }
          if ( mi < 0.0 ) mi = 0.0;
          if ( mi > 1.0 ) mi = 1.0;
          if ( n == 3 ) {
              m = mi;
          }
      }
      else powm= 1.0;

      if(p->hExpo > 0) {
          ha= p->hka*mhFunc((V-p->hVa)/p->hsa, p->haFunc);
          hb= p->hkb*mhFunc((V-p->hVb)/p->hsb, p->hbFunc);
          kh[n] = ha*(1.0-hi)-hb*hi;

          powh = hi;
          for (i= 0; i < p->hExpo-1; i++) powh*= hi;

          if ( n < 3 ) {
              hi = h + kh[n]*dt;
          } else {
              hi = h + (kh[0] + 2*kh[1] + 2*kh[2] + kh[3]) * dt / 6;
          }
          if ( hi < 0.0 ) hi = 0.0;
          if ( hi > 1.0 ) hi = 1.0;
          if ( n == 3 ) {
              h = hi;
          }
      }
      else powh= 1.0;
      I= p->gMax*powm*powh*(p->Vrev-V);
      out->I+= I;
    }
}
