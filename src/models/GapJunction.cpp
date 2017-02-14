
#include "GapJunction.h"
#include "DCThread.h"

GapJunction::GapJunction(GJunctData *inp, DCThread *t, GapJunctionAssignment a) :
    p(inp),
    pre(t->getInChan(a.preInChannel)),
    post(t->getInChan(a.postInChannel)),
    outpre(t->getOutChan(a.preOutChannel)),
    outpost(t->getOutChan(a.postOutChannel)),
    a(a)
{
}

void GapJunction::currentUpdate(double t, double dt)
{
  if ( p->active && *a.actP && pre->active && post->active && outpre->active && outpost->active ) {
      // calculate synaptic current
      I= p->gSyn * (pre->V - post->V);
      if ((p->type == 1) && (I < 0.0)) {
           I= 0.0;
           return;
      }
      outpost->I+= I;
      outpre->I-= I;
  }
}

