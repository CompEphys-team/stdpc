#ifndef GAPJUNCTION_H
#define GAPJUNCTION_H

#include "Global_func.h"
#include "Channels.h"

class GapJunction {
  private:
    GJunctData *p;
    
  protected:
    double I;
    inChannel *pre;
    inChannel *post;
    outChannel *outpre;
    outChannel *outpost;
    GapJunctionAssignment a;
        
  public:
    GapJunction(GJunctData *, DCThread *, GapJunctionAssignment);
    void currentUpdate(double, double);

    typedef GJunctData param_type;
};

#endif
