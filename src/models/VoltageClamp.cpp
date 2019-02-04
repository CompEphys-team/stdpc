
#include "VoltageClamp.h"
#include "DCThread.h"

VoltageClamp::VoltageClamp(VoltageClampData *inp, DCThread *t, VoltageClampAssignment *a, inChannel *cmd, inChannel *post, outChannel *out) :
    p(inp),
    cmd(cmd),
    post(post),
    out(out),
    a(a),
    IP(0.),
    II(0.),
    ID(0.),
    Bufptr(0),
    BufSz(1),
    BufMax(p->tstepD)
{
    VBuf= new double[BufMax];
    tBuf= new double[BufMax];
    VBuf[0]= post->V;
}

VoltageClamp::~VoltageClamp()
{
    delete[] VBuf;
    delete[] tBuf;
}

void VoltageClamp::currentUpdate(double t, double)
{
  if ( p->active && a->active && cmd->active && post->active && out->active ) {
      // calculate VClamp current
      IP= p->gP * (cmd->V - post->V);
      out->I+= IP;
      II= p->gI * (cmdAvg - postAvg);
      out->I+= II;
      VBuf[(Bufptr+BufSz)%BufMax]= post->V;
      tBuf[(Bufptr+BufSz)%BufMax]= t;
      ID= p->gD * (post->V - VBuf[Bufptr])/(t-tBuf[Bufptr]);
      out->I+= ID;
      if (BufSz < BufMax) {
          BufSz++;
      }
      else {
          Bufptr++;
          if (Bufptr >= BufMax) Bufptr= 0;
      }
  }
}

