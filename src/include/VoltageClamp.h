#pragma once

#include "Global_func.h"
#include "Channels.h"

class VoltageClamp {
  private:
    VoltageClampData *p;
    
  protected:
    double I;
    inChannel *cmd;
    inChannel *post;
    outChannel *out;
    VoltageClampAssignment *a;
        
  public:
    VoltageClamp(VoltageClampData *, DCThread *, VoltageClampAssignment *, inChannel *cmd, inChannel *post, outChannel *out);
    void currentUpdate(double t, double dt);

    typedef VoltageClampData param_type;
};

