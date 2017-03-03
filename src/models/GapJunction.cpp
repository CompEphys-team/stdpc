
#include "GapJunction.h"
#include "DCThread.h"

GapJunction::GapJunction(GJunctData *inp, GapJunctionAssignment *a, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost) :
    p(inp),
    pre(pre),
    post(post),
    outpre(outpre),
    outpost(outpost),
    a(a)
{
}

void GapJunction::currentUpdate(double t, double dt)
{
  if ( p->active && a->active && pre->active && post->active && outpre->active && outpost->active ) {
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

