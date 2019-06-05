/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#include "Wire.h"
#include "DCThread.h"
#include "WireDlg.h"

static WireProxy *prox = WireProxy::get();
std::vector<WireData> WireProxy::p;
WireProxy *Wire::proxy() const { return prox; }
ConductanceDlg *WireProxy::createDialog(size_t condID, QWidget *parent) { return new WireDlg(condID, parent); }
IonicCurrent *WireProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new Wire(conductanceID, assignID, multiID, in, out);
}

WireProxy::WireProxy()
{
    ConductanceManager::RegisterTool(this);
    addAP("Wire[#].active", &p, &WireData::active);
    addAP("Wire[#].activeSettling", &p, &WireData::activeSettling);
    addAP("Wire[#].label", &p, &WireData::label);
    addAP("Wire[#].assign[#].active", &p, &WireData::assign, &CurrentAssignment::active);
    addAP("Wire[#].assign[#].VChannel", &p, &WireData::assign, &CurrentAssignment::VChannel);
    addAP("Wire[#].assign[#].IChannel", &p, &WireData::assign, &CurrentAssignment::IChannel);
    addAP("Wire[#].factor", &p, &WireData::factor);
}


Wire::Wire(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    IonicCurrent(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{
}

void Wire::step(double, double, bool settling)
{
    if ( p->active && a->active && in->active && out->active && (!settling || p->activeSettling) ) {
        out->I += in->V * p->factor;
    }
}
