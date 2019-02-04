#include "VSteps.h"
#include "DCThread.h"
#include "ModelManager.h"
#include "AP.h"
#include "VStepsDlg.h"

/// Construct a single self-registering proxy
static VStepsProxy prox;
std::vector<VStepsData> VStepsProxy::p;
ModelProxy *VStepsPrototype::proxy() const { return &prox; }
ModelPrototype *VStepsProxy::createPrototype(size_t modelID) { return new VStepsPrototype(modelID); }
ModelDlg *VStepsProxy::createDialog(size_t modelID, QWidget *parent) { return new VStepsDlg(modelID, parent); }

VStepsProxy::VStepsProxy()
{
    ModelManager::RegisterModel(modelClass(), this);
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
    for (int i= 0; i < stepNo; i++) {
        cmdT.push_back(p->t0+i*(p->tStep+p->tHold));
        cmdV.push_back(p->startV+i*p->deltaV);
        cmdT.push_back(p->t0+i*(p->tStep+p->tHold)+p->tStep);
        cmdV.push_back(p->holdV);
    }
    cmdTI= cmdT.begin();
    cmdVI= cmdV.begin();
}

void VSteps::RK4(double t, double dt, size_t n)
{
    /// Cheapskate RK, because SG isn't really a candidate for it
    /// Note the 2*dt, which is the actual full step length for this RK cycle
    /// Note also that burst detection may be slightly inaccurate if the bd channel is another model
    if ( n == 0 )
        update(t, 2*dt);
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
}

void VStepsPrototype::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new VSteps(this, i, DCT));
}
