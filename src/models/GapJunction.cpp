
#include "GapJunction.h"

GapJunction::GapJunction() 
{
}

void GapJunction::init(GJunctData *inp, short int *inIdx, short int *outIdx, inChannel *inChn, outChannel *outChn)
{
  p= inp;
  pre= &inChn[inIdx[p->preInChannel]];
  post= &inChn[inIdx[p->postInChannel]];
  outpre= &outChn[outIdx[p->preOutChannel]];
  outpost= &outChn[outIdx[p->postOutChannel]];
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

