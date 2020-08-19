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

#include "sinusoidaltransform.h"
#include "DCThread.h"
#include "sinusoidaltransformdlg.h"
#include "AP.h"

static SinusoidalTransformProxy *prox = SinusoidalTransformProxy::get();
std::vector<SinusoidalTransformData> SinusoidalTransformProxy::p;
SinusoidalTransformProxy *SinusoidalTransform::proxy() const { return prox; }
ConductanceDlg *SinusoidalTransformProxy::createDialog(size_t condID, QWidget *parent) { return new SinusoidalTransformDlg(condID, parent); }
ChannelTransform *SinusoidalTransformProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new SinusoidalTransform(conductanceID, assignID, multiID, in, out);
}

SinusoidalTransformProxy::SinusoidalTransformProxy()
{
    ConductanceManager::RegisterTool(this);

    addAP("SinusoidalTransform[#].active", &p, &SinusoidalTransformData::active);
    addAP("SinusoidalTransform[#].activeSettling", &p, &SinusoidalTransformData::activeSettling);
    addAP("SinusoidalTransform[#].label", &p, &SinusoidalTransformData::label);
    addAP("SinusoidalTransform[#].seq", &p, &SinusoidalTransformData::seq);
    addAP("SinusoidalTransform[#].slope", &p, &SinusoidalTransformData::slope);
    addAP("SinusoidalTransform[#].offset", &p, &SinusoidalTransformData::offset);
    addAP("SinusoidalTransform[#].assign[#].active", &p, &SinusoidalTransformData::assign, &ChannelTransformAssignment::active);
    addAP("SinusoidalTransform[#].assign[#].target", &p, &SinusoidalTransformData::assign, &ChannelTransformAssignment::target);
    addAP("SinusoidalTransform[#].assign[#].save", &p, &SinusoidalTransformData::assign, &ChannelTransformAssignment::save);
}


SinusoidalTransform::SinusoidalTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    ChannelTransform(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{

}

void SinusoidalTransform::step(double t, double dt, bool settling)
{
    if ( p->active && a->active && channel_active && (p->activeSettling || !settling) ) {
        m_conductance = channel_value;
        channel_value = 1. / (1. + exp((p->offset - channel_value) / p->slope));
    } else {
        m_conductance = 0;
    }
}

