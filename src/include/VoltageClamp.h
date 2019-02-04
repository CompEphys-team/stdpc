#pragma once

#include "Global_func.h"
#include "Channels.h"

class VoltageClamp {
  private:
    VoltageClampData *p;
    
  protected:
    double I;
    double IP, II, ID;
    double cmdAvg, postAvg;
    double *VBuf, *tBuf;
    size_t Bufptr, BufSz, BufMax;
    inChannel *cmd;
    inChannel *post;
    outChannel *out;
    VoltageClampAssignment *a;
        
  public:
    VoltageClamp(VoltageClampData *, DCThread *, VoltageClampAssignment *, inChannel *cmd, inChannel *post, outChannel *out);
    ~VoltageClamp();
    void currentUpdate(double t, double dt);

    typedef VoltageClampData param_type;
};

