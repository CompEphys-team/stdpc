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

#include "HHNeuron.h"
#include "ModelManager.h"
#include "AP.h"
#include "HHModelDlg.h"

/// Construct a single self-registering proxy
static HHNeuronProxy *prox = HHNeuronProxy::get();
std::vector<HHNeuronData> HHNeuronProxy::p;
ModelProxy *HHNeuronModel::proxy() const { return prox; }
ModelPrototype *HHNeuronProxy::createPrototype(size_t modelID) { return new HHNeuronModel(modelID); }
ModelDlg *HHNeuronProxy::createDialog(size_t modelID, QWidget *parent) { return new HHModelDlg(modelID, parent); }

HHNeuronProxy::HHNeuronProxy()
{
    ModelManager::RegisterModel(modelClass(), this);
    addAP("HHNeuronp[#].active", &HHNeuronProxy::p, &HHNeuronData::active);
    addAP("HHNeuronp[#].label", &HHNeuronProxy::p, &HHNeuronData::label);
    addAP("HHNeuronp[#].C", &HHNeuronProxy::p, &HHNeuronData::C);
    addAP("HHNeuronp[#].gLeak", &HHNeuronProxy::p, &HHNeuronData::gLeak);
    addAP("HHNeuronp[#].ELeak", &HHNeuronProxy::p, &HHNeuronData::ELeak);
    addAP("HHNeuronp[#].Vlimit", &HHNeuronProxy::p, &HHNeuronData::Vlimit);
    addAP("HHNeuronp[#].Vmin", &HHNeuronProxy::p, &HHNeuronData::Vmin);
    addAP("HHNeuronp[#].Vmax", &HHNeuronProxy::p, &HHNeuronData::Vmax);
    addAP("HHNeuronp[#].inst[#].active", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::active);
    addAP("HHNeuronp[#].inst[#].inChn.spkDetect", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::inChn, &inChnData::spkDetect);
    addAP("HHNeuronp[#].inst[#].inChn.spkDetectThresh", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::inChn, &inChnData::spkDetectThresh);
    addAP("HHNeuronp[#].inst[#].inChn.bias", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::inChn, &inChnData::bias);
    addAP("HHNeuronp[#].inst[#].inChn.chnlSaving", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::inChn, &inChnData::chnlSaving);
    addAP("HHNeuronp[#].inst[#].outChn.bias", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::outChn, &outChnData::bias);
    addAP("HHNeuronp[#].inst[#].outChn.chnlSaving", &HHNeuronProxy::p, &HHNeuronData::inst, &vInstData::outChn, &outChnData::chnlSaving);
}


HHNeuron::HHNeuron(ModelPrototype *parent, size_t instID, DCThread *DCT) :
    Model(parent, instID, DCT),
    V(-0.06)
{
    Vi = V;
    in.V = V + params().inChn.bias;
}

void HHNeuron::RK4(double, double dt, size_t n, bool)
{
    // instance update
    const HHNeuronData &p = static_cast<HHNeuronData const&>(parent->params());
    kV[n] = (out.I + p.gLeak * (p.ELeak - Vi)) / p.C;
    if ( n < 3 ) {
        Vi = V + kV[n]*dt;
    } else {
        Vi = V + (kV[0] + 2*kV[1] + 2*kV[2] + kV[3]) * dt / 6;
    }

    if ( p.Vlimit ) {
        if ( Vi < p.Vmin ) Vi = p.Vmin;
        else if ( Vi > p.Vmax ) Vi = p.Vmax;
    }

    if ( n == 3 )
        V = Vi;

    in.V = Vi + params().inChn.bias;

    // restore current (except on last step, where it's better to keep the final value for output/graphing)
    if ( n < 3 ) {
        out.I = retainedI;
    }
}



void HHNeuronModel::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new HHNeuron(this, i, DCT));
}
