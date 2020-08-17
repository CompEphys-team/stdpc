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

#include "channelsinusoid.h"
#include "DCThread.h"
#include "channelsinusoiddlg.h"
#include "AP.h"

static ChannelSinusoidProxy *prox = ChannelSinusoidProxy::get();
std::vector<ChannelSinusoidData> ChannelSinusoidProxy::p;
ChannelSinusoidProxy *ChannelSinusoid::proxy() const { return prox; }
ConductanceDlg *ChannelSinusoidProxy::createDialog(size_t condID, QWidget *parent) { return new ChannelSinusoidDlg(condID, parent); }
ChannelTransform *ChannelSinusoidProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new ChannelSinusoid(conductanceID, assignID, multiID, in, out);
}

ChannelSinusoidProxy::ChannelSinusoidProxy()
{
    ConductanceManager::RegisterTool(this);

    addAP("ChannelSinusoid[#].active", &p, &ChannelSinusoidData::active);
    addAP("ChannelSinusoid[#].activeSettling", &p, &ChannelSinusoidData::activeSettling);
    addAP("ChannelSinusoid[#].label", &p, &ChannelSinusoidData::label);
    addAP("ChannelSinusoid[#].seq", &p, &ChannelSinusoidData::seq);
    addAP("ChannelSinusoid[#].slope", &p, &ChannelSinusoidData::slope);
    addAP("ChannelSinusoid[#].offset", &p, &ChannelSinusoidData::offset);
    addAP("ChannelSinusoid[#].assign[#].active", &p, &ChannelSinusoidData::assign, &ChannelTransformAssignment::active);
    addAP("ChannelSinusoid[#].assign[#].target", &p, &ChannelSinusoidData::assign, &ChannelTransformAssignment::target);
    addAP("ChannelSinusoid[#].assign[#].save", &p, &ChannelSinusoidData::assign, &ChannelTransformAssignment::save);
}


ChannelSinusoid::ChannelSinusoid(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    ChannelTransform(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{

}

void ChannelSinusoid::step(double t, double dt, bool settling)
{
    if ( p->active && a->active && channel_active && (p->activeSettling || !settling) ) {
        m_conductance = channel_value;
        channel_value = 1. / (1. + exp((p->offset - channel_value) / p->slope));
    } else {
        m_conductance = 0;
    }
}

