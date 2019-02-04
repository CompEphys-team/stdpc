
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
    cmdAvg(0.),
    postAvg(0.),
    denom(0.),
    Bufptr(0),
    BufSz(0),
    BufMax(p->tstepD)
{
    VBuf.resize(BufMax);
    tBuf.resize(BufMax);
}

VoltageClamp::~VoltageClamp()
{
    cerr << "entering destructor ... " << endl;
}

void VoltageClamp::currentUpdate(double t, double)
{
  if ( p->active && a->active && cmd->active && post->active && out->active ) {
      // calculate VClamp current
      cmdAvg= cmdAvg*p->decayI+cmd->V;
      postAvg= postAvg*p->decayI+post->V;
      denom= denom*p->decayI+1.0;
      VBuf[(Bufptr+BufSz)%BufMax]= post->V;
      tBuf[(Bufptr+BufSz)%BufMax]= t;
      if (BufSz < BufMax-1) {
          BufSz++;
      }
      else {
          IP= p->gP * (cmd->V - post->V);
          out->I+= IP;
          II= p->gI * (cmdAvg - postAvg)/denom;
          out->I+= II;
          ID= p->gD * (post->V - VBuf[Bufptr])/(t-tBuf[Bufptr]);
          //cerr << post->V - VBuf[Bufptr] << " " << t-tBuf[Bufptr] << " " << ID << endl;
          out->I+= ID;
          Bufptr++;
          if (Bufptr >= BufMax) Bufptr= 0;
      }
  }
}

