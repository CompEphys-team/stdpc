
#include "Wire.h"
#include "DCThread.h"
#include "WireDlg.h"

static WireProxy *prox = WireProxy::get();
std::vector<WireData> WireProxy::p;
WireProxy *Wire::proxy() const { return prox; }
ConductanceDlg *WireProxy::createDialog(size_t condID, QWidget *parent) { return new WireDlg(condID, parent); }
Synapse *WireProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *DCT,
                                    inChannel *pre, inChannel *post, outChannel *out) {
    return new Wire(conductanceID, assignID, multiID, DCT, pre, post, out);
}

WireProxy::WireProxy()
{
    ConductanceManager::RegisterSynapse(this);
    addAP("Wire[#].active", &p, &WireData::active);
    addAP("Wire[#].activeSettling", &p, &WireData::activeSettling);
    addAP("Wire[#].label", &p, &WireData::label);
    addAP("Wire[#].assign[#].active", &p, &WireData::assign, &SynapseAssignment::active);
    addAP("Wire[#].assign[#].PreSynChannel", &p, &WireData::assign, &SynapseAssignment::PreSynChannel);
    addAP("Wire[#].assign[#].PostSynChannel", &p, &WireData::assign, &SynapseAssignment::PostSynChannel);
    addAP("Wire[#].assign[#].OutSynChannel", &p, &WireData::assign, &SynapseAssignment::OutSynChannel);
}


Wire::Wire(size_t condID, size_t assignID, size_t multiID, DCThread *DCT, inChannel *pre, inChannel *post, outChannel *out) :
    Synapse(condID, assignID, multiID, pre, nullptr, out),
    p(&params()),
    a(&assignment()),
    DCT(DCT),
    active(true)
{
}

void Wire::step(double, double, bool)
{
 // if ( !active || !p->active || !a->active || !pre->active || !out->active ) {
 //     m_conductance = 0;
 //     return;
 // }
  out->I += pre->V*1e-6;
}
