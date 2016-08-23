
#include "GapJunction.h"
#include "DCThread.h"

GapJunction::GapJunction(GJunctData *inp, DCThread *t, GapJunctionAssignment *ina) :
    p(inp),
    a(ina)
{
    if ( !a ) {
        legacy.active = !p->noLegacyAssign;
        legacy.preInChannel = p->preInChannel;
        legacy.postInChannel = p->postInChannel;
        legacy.preOutChannel = p->preOutChannel;
        legacy.postOutChannel = p->postOutChannel;
        a =& legacy;
    }
    pre = t->getInChan(a->preInChannel);
    post = t->getInChan(a->postInChannel);
    outpre = t->getOutChan(a->preOutChannel);
    outpost = t->getOutChan(a->postOutChannel);
}

void GapJunction::currentUpdate(double t, double dt)
{
  // calculate synaptic current
  I= p->gSyn * (pre->V - post->V);
  if ((p->type == 1) && (I < 0.0)) {
       I= 0.0;
  } 
  outpost->I+= I;
  outpre->I-= I;
}

