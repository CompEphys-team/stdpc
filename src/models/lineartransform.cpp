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

#include "lineartransform.h"
#include "DCThread.h"
#include "lineartransformdlg.h"
#include "AP.h"

static LinearTransformProxy *prox = LinearTransformProxy::get();
std::vector<LinearTransformData> LinearTransformProxy::p;
LinearTransformProxy *LinearTransform::proxy() const { return prox; }
ConductanceDlg *LinearTransformProxy::createDialog(size_t condID, QWidget *parent) { return new LinearTransformDlg(condID, parent); }
ChannelTransform *LinearTransformProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new LinearTransform(conductanceID, assignID, multiID, in, out);
}

LinearTransformProxy::LinearTransformProxy()
{
    ConductanceManager::RegisterTool(this);

    addAP("LinearTransform[#].active", &p, &LinearTransformData::active);
    addAP("LinearTransform[#].activeSettling", &p, &LinearTransformData::activeSettling);
    addAP("LinearTransform[#].label", &p, &LinearTransformData::label);
    addAP("LinearTransform[#].seq", &p, &LinearTransformData::seq);
    addAP("LinearTransform[#].factor", &p, &LinearTransformData::factor);
    addAP("LinearTransform[#].offset", &p, &LinearTransformData::offset);
    addAP("LinearTransform[#].assign[#].active", &p, &LinearTransformData::assign, &ChannelTransformAssignment::active);
    addAP("LinearTransform[#].assign[#].target", &p, &LinearTransformData::assign, &ChannelTransformAssignment::target);
    addAP("LinearTransform[#].assign[#].save", &p, &LinearTransformData::assign, &ChannelTransformAssignment::save);
}


LinearTransform::LinearTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    ChannelTransform(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{

}

void LinearTransform::step(double t, double dt, bool settling)
{
    if ( p->active && a->active && channel_active && (p->activeSettling || !settling) ) {
        m_conductance = channel_value;
        channel_value = p->offset + p->factor*channel_value;
    } else {
        m_conductance = 0;
    }
}
