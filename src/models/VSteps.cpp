/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "VSteps.h"
#include "DCThread.h"
#include "ModelManager.h"
#include "AP.h"
#include "VStepsDlg.h"

/// Construct a single self-registering proxy
static VStepsProxy *prox = VStepsProxy::get();
std::vector<VStepsData> VStepsProxy::p;
VStepsProxy *VSteps::proxy() const { return prox; }
Model *VStepsProxy::instantiate(size_t modelID, size_t instID, DCThread *DCT) { return new VSteps(modelID, instID, DCT); }
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
    addAP("VSteps[#].nSteps", &VStepsProxy::p, &VStepsData::nSteps);
    addAP("VSteps[#].deltaV", &VStepsProxy::p, &VStepsData::deltaV);
    addAP("VSteps[#].t0", &VStepsProxy::p, &VStepsData::t0);
    addAP("VSteps[#].tStep", &VStepsProxy::p, &VStepsData::tStep);
    addAP("VSteps[#].tHold", &VStepsProxy::p, &VStepsData::tHold);
    addAP("VSteps[#].inst.inChn.save", &VStepsProxy::p, &VStepsData::inst, &vInstData::inChn, &inChnData::chnlSaving);
}

VSteps::VSteps(size_t modelID, size_t instID, DCThread *DCT) :
    Model(modelID, instID, DCT),
    p(&params()),
    instp(&instance()),
    V(p->holdV),
    active(true)
{
    // intitialize vector of step times and step voltages
    cmdT.clear();
    cmdV.clear();
    cmdV.push_back(p->holdV);
    for (int i= 0; i < p->nSteps; i++) {
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
