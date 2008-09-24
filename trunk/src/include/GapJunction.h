#ifndef GAPJUNCTION_H
#define GAPJUNCTION_H

#include "global_func.h"
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
        
  public:
    GapJunction();
    void init(GJunctData *, short int *, short int *, inChannel *, outChannel *);
    void currentUpdate(double, double);
};

#endif
