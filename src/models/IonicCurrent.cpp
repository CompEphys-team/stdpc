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

#include "IonicCurrent.h"
#include "DCThread.h"

IonicCurrent::IonicCurrent(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    Conductance(condID, assignID, multiID),
    in(in),
    out(out)
{
}

void IonicCurrentProxy::instantiate(size_t condID, size_t assignID, DCThread *DCT, std::vector<Conductance *> &preD, std::vector<Conductance *> &inD, std::vector<Conductance *> &postD)
{
    const CurrentAssignment &a = param(condID).assignment(assignID);
    size_t multi = 0;
    if ( a.VChannel == a.IChannel ) {
        // Input/Output on the same model => connect instances 1-to-1 rather than all-to-all
        for ( ChannelIndex VIChan : DCT->getChanIndices(a.VChannel) ) {
            inChannel *inC = DCT->getInChan(VIChan);
            outChannel *outC = DCT->getOutChan(VIChan);
            if ( inC && outC ) {
                inD.push_back(createAssigned(condID, assignID, multi++, inC, outC));
            }
        }
    } else {
        for ( ChannelIndex VChan : DCT->getChanIndices(a.VChannel) ) {
            for ( ChannelIndex IChan : DCT->getChanIndices(a.IChannel) ) {
                inChannel *inC = DCT->getInChan(VChan);
                outChannel *outC = DCT->getOutChan(IChan);
                if ( inC && outC ) {
                    if ( VChan.isVirtual && IChan.isAnalog )
                        postD.push_back(createAssigned(condID, assignID, multi++, inC, outC));
                    else
                        preD.push_back(createAssigned(condID, assignID, multi++, inC, outC));
                }
            }
        }
    }
}
