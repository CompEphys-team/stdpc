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

#include "channeltransform.h"
#include "DCThread.h"

ChannelTransform::ChannelTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out):
    Conductance(condID, assignID, multiID),
    channel_active(in ? in->active : out->active),
    channel_value(in ? in->V : out->I)
{

}

void ChannelTransformProxy::instantiate(size_t condID, size_t assignID, DCThread *DCT, ConductanceManager *manager)
{
    const ChannelTransformAssignment &a = param(condID).assignment(assignID);
    size_t multi = 0;
    for ( ChannelIndex dex : DCT->getChanIndices(a.target) ) {
        if ( dex.isVirtual && !dex.isDirectional ) { // ensure consistent treatment of unsupported non-directional dex
            dex.isDirectional = dex.isInChn = true;
        }
        inChannel *inC = DCT->getInChan(dex);
        outChannel *outC = DCT->getOutChan(dex);
        ChannelTransform *assigned = createAssigned(condID, assignID, multi++, inC, outC);
        if ( dex.isVirtual ) {
            if ( dex.isInChn )
                manager->transform_modIn.push_back(assigned);
            else
                manager->transform_modOut.push_back(assigned);
        } else {
            if ( dex.isInChn )
                manager->transform_devIn.push_back(assigned);
            else
                manager->transform_devOut.push_back(assigned);
        }
    }
}
