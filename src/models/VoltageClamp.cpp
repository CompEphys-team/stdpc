
#include "VoltageClamp.h"
#include "DCThread.h"

VoltageClamp::VoltageClamp(VoltageClampData *inp, DCThread *t, VoltageClampAssignment *a, inChannel *cmd, inChannel *post, outChannel *out) :
    p(inp),
    cmd(cmd),
    post(post),
    out(out),
    a(a)
{
}

void VoltageClamp::currentUpdate(double, double)
{
  if ( p->active && a->active && cmd->active && post->active && out->active ) {
      // calculate VClamp current
      I= p->gP * (cmd->V - post->V);
      out->I+= I;
  }
}

