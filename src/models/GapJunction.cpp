
#include "GapJunction.h"
#include "DCThread.h"

GapJunction::GapJunction(GJunctData *inp, DCThread *t, GapJunctionAssignment *a) :
    p(inp),
    pre(t->getInChan(a->preInChannel)),
    post(t->getInChan(a->postInChannel)),
    outpre(t->getOutChan(a->preOutChannel)),
    outpost(t->getOutChan(a->postOutChannel))
{
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

