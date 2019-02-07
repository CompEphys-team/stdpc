#include "VSteps.h"
#include "DCThread.h"
#include "ModelManager.h"
#include "AP.h"
#include "VStepsDlg.h"

/// Construct a single self-registering proxy
static VStepsProxy *prox = VStepsProxy::get();
std::vector<VStepsData> VStepsProxy::p;
ModelProxy *VStepsPrototype::proxy() const { return prox; }
ModelPrototype *VStepsProxy::createPrototype(size_t modelID) { return new VStepsPrototype(modelID); }
ModelDlg *VStepsProxy::createDialog(size_t modelID, QWidget *parent) { return new VStepsDlg(modelID, parent); }

VStepsData::VStepsData()
{
    // Ensure the single instance is correctly set up. None of these values have any reason to change at runtime.
    inst.active = true;
    inst.inChn.active = true;
    inst.outChn.active = false;
    inst.outChn.chnlSaving = false;
}

VStepsProxy::VStepsProxy()
{
    ModelManager::RegisterModel(modelClass(), this);
    addAP("VSteps[#].active", &VStepsProxy::p, &VStepsData::active);
    addAP("VSteps[#].label", &VStepsProxy::p, &VStepsData::label);
    addAP("VSteps[#].holdV", &VStepsProxy::p, &VStepsData::holdV);
    addAP("VSteps[#].startV", &VStepsProxy::p, &VStepsData::startV);
    addAP("VSteps[#].endV", &VStepsProxy::p, &VStepsData::endV);
    addAP("VSteps[#].deltaV", &VStepsProxy::p, &VStepsData::deltaV);
    addAP("VSteps[#].t0", &VStepsProxy::p, &VStepsData::t0);
    addAP("VSteps[#].tStep", &VStepsProxy::p, &VStepsData::tStep);
    addAP("VSteps[#].tHold", &VStepsProxy::p, &VStepsData::tHold);
    addAP("VSteps[#].inst.inChn.save", &VStepsProxy::p, &VStepsData::inst, &vInstData::inChn, &inChnData::chnlSaving);
}

VSteps::VSteps(ModelPrototype *parent, size_t instID, DCThread *DCT) :
    Model(parent, instID, DCT),
    p(static_cast<const VStepsData *>(&(parent->params()))),
    instp(static_cast<const vInstData *>(&params())),
    V(p->holdV),
    active(true)
{
    // intitialize vector of step times and step voltages
    cmdT.clear();
    cmdV.clear();
    int stepNo= static_cast<int>(floor((p->endV-p->startV+1e-6)/p->deltaV));
    cmdV.push_back(p->holdV);
    for (int i= 0; i < stepNo; i++) {
        cmdT.push_back(p->t0+i*(p->tStep+p->tHold));
        cmdV.push_back(p->startV+i*p->deltaV);
        cmdT.push_back(p->t0+i*(p->tStep+p->tHold)+p->tStep);
        cmdV.push_back(p->holdV);
    }
    cmdTI= cmdT.begin();
    cmdVI= cmdV.begin();
}

void VSteps::RK4(double t, double dt, size_t n, bool settling)
{
    /// Cheapskate RK, because VSteps isn't really a candidate for it
    /// Note the 2*dt, which is the actual full step length for this RK cycle
    if ( n == 0 ) {
        if ( settling )
            update(0,0);
        else
            update(t, 2*dt);
    }
    // Need neither further changes on `in` (nobody touches its voltage),
    // nor on `out` (it's inactive anyway)
}

void VSteps::update(double t, double dt)
{
    if ( !active )
        return;
    while ((cmdTI != cmdT.end()) && (*cmdTI < t + dt)) {
        cmdTI++;
        cmdVI++;
    }
    V= *cmdVI;
    in.V= V;
}

void VStepsPrototype::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new VSteps(this, i, DCT));
}
