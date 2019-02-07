
#include "VoltageClamp.h"
#include "DCThread.h"
#include "VoltageClampDlg.h"

static VoltageClampProxy *prox = VoltageClampProxy::get();
std::vector<VoltageClampData> VoltageClampProxy::p;
VoltageClampProxy *VoltageClamp::proxy() const { return prox; }
ConductanceDlg *VoltageClampProxy::createDialog(size_t condID, QWidget *parent) { return new VoltageClampDlg(condID, parent); }
Synapse *VoltageClampProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *DCT,
                                    inChannel *pre, inChannel *post, outChannel *out) {
    return new VoltageClamp(conductanceID, assignID, multiID, DCT, pre, post, out);
}

VoltageClampProxy::VoltageClampProxy()
{
    ConductanceManager::RegisterSynapse(this);
    addAP("VoltageClamp[#].active", &p, &VoltageClampData::active);
    addAP("VoltageClamp[#].activeSettling", &p, &VoltageClampData::activeSettling);
    addAP("VoltageClamp[#].label", &p, &VoltageClampData::label);
    addAP("VoltageClamp[#].assign[#].active", &p, &VoltageClampData::assign, &SynapseAssignment::active);
    addAP("VoltageClamp[#].assign[#].PreSynChannel", &p, &VoltageClampData::assign, &SynapseAssignment::PreSynChannel);
    addAP("VoltageClamp[#].assign[#].PostSynChannel", &p, &VoltageClampData::assign, &SynapseAssignment::PostSynChannel);
    addAP("VoltageClamp[#].assign[#].OutSynChannel", &p, &VoltageClampData::assign, &SynapseAssignment::OutSynChannel);
    addAP("VoltageClamp[#].assign[#].save", &p, &VoltageClampData::assign, &SynapseAssignment::save);

    addAP("VoltageClamp[#].gP", &p, &VoltageClampData::gP);
    addAP("VoltageClamp[#].gI", &p, &VoltageClampData::gI);
    addAP("VoltageClamp[#].gD", &p, &VoltageClampData::gD);
    addAP("VoltageClamp[#].decayI", &p, &VoltageClampData::decayI);
    addAP("VoltageClamp[#].tstepD", &p, &VoltageClampData::tstepD);
}


VoltageClamp::VoltageClamp(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out) :
    Synapse(condID, assignID, multiID, pre, post, out),
    p(&params()),
    a(&assignment()),
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
    cmdBuf.resize(BufMax);
    tBuf.resize(BufMax);
}

void VoltageClamp::step(double t, double, bool settling)
{
  if ( !p->active || !a->active || !pre->active || !post->active || !out->active ) {
      m_conductance = 0;
      return;
  }

  if ( !settling || p->activeSettling ) {
      // calculate VClamp current
      cmdAvg= cmdAvg*p->decayI+pre->V;
      postAvg= postAvg*p->decayI+post->V;
      denom= denom*p->decayI+1.0;
      VBuf[(Bufptr+BufSz)%BufMax]= post->V;
      cmdBuf[(Bufptr+BufSz)%BufMax]= pre->V;
      tBuf[(Bufptr+BufSz)%BufMax]= t;
      if (BufSz < BufMax-1) {
          BufSz++;
      }
      else {
          IP= p->gP * (pre->V - post->V);
          out->I+= IP;
          II= p->gI * (cmdAvg - postAvg)/denom;
          out->I+= II;
          ID= p->gD * ( pre->V - cmdBuf[Bufptr] - post->V + VBuf[Bufptr] )/(t-tBuf[Bufptr]);
          out->I+= ID;
          Bufptr++;
          if (Bufptr >= BufMax) Bufptr= 0;
      }
  }
}
