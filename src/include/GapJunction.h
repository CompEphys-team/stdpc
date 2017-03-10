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
    GapJunctionAssignment *a;
        
  public:
    GapJunction(GJunctData *, GapJunctionAssignment *, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost);
    void currentUpdate(double t, double dt);

    typedef GJunctData param_type;
};

#endif
