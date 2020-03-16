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

#include "InputConductance.h"
#include "InputConductanceDlg.h"
#include "AP.h"

static InputConductanceProxy *prox = InputConductanceProxy::get();
std::vector<InputConductanceData> InputConductanceProxy::p;
InputConductanceProxy *InputConductance::proxy() const { return prox; }
ConductanceDlg *InputConductanceProxy::createDialog(size_t condID, QWidget *parent) { return new InputConductanceDlg(condID, parent); }
Synapse *InputConductanceProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *DCT,
                                      inChannel *pre, inChannel *post, outChannel *out) {
    return new InputConductance(conductanceID, assignID, multiID, DCT, pre, post, out);
}

InputConductanceProxy::InputConductanceProxy()
{
    ConductanceManager::RegisterTool(this);
    addAP("InputCond[#].active", &p, &InputConductanceData::active);
    addAP("InputCond[#].activeSettling", &p, &InputConductanceData::activeSettling);
    addAP("InputCond[#].label", &p, &InputConductanceData::label);

    addAP("InputCond[#].Vrev", &p, &InputConductanceData::Vrev);
    addAP("InputCond[#].gStatic", &p, &InputConductanceData::gStatic);
    addAP("InputCond[#].rectify", &p, &InputConductanceData::rectify);
    addAP("InputCond[#].assign[#].active", &p, &InputConductanceData::assign, &SynapseAssignment::active);
    addAP("InputCond[#].assign[#].GChannel", &p, &InputConductanceData::assign, &SynapseAssignment::PreSynChannel);
    addAP("InputCond[#].assign[#].VChannel", &p, &InputConductanceData::assign, &SynapseAssignment::PostSynChannel);
    addAP("InputCond[#].assign[#].IChannel", &p, &InputConductanceData::assign, &SynapseAssignment::OutSynChannel);
}

InputConductance::InputConductance(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out):
    Synapse(condID, assignID, multiID, pre, post, out),
    p(&params()),
    a(&assignment())
{
    m_conductance = p->gStatic;
}

void InputConductance::step(double, double, bool settling)
{
    if ( !p->active || !a->active || !pre->active || !post->active || !out->active ) {
        m_conductance = 0;
        return;
    }

    if ( p->rectify && pre->V < 0 )
        m_conductance = 0;
    else
        m_conductance = p->gStatic * pre->V;

    if ( !settling || p->activeSettling )
        out->I += m_conductance * (p->Vrev - post->V);
}
