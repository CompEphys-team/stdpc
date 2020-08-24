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

#include "Synapse.h"
#include "DCThread.h"

void SynapseProxy::instantiate(size_t conductanceID, size_t assignID, DCThread *DCT, ConductanceManager *manager)
{
    const SynapseAssignment &a = param(conductanceID).assignment(assignID);
    std::vector<ChannelIndex> preSynInst = DCT->getChanIndices(a.PreSynChannel);
    std::vector<ChannelIndex> postSynInst = DCT->getChanIndices(a.PostSynChannel);
    std::vector<ChannelIndex> outSynInst = DCT->getChanIndices(a.OutSynChannel);
    inChannel *preC, *postC;
    outChannel *outC;

    size_t multi = 0;
    if ( a.PostSynChannel == a.OutSynChannel ) {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex pre : preSynInst ) {
                if ( (preC=DCT->getInChan(pre)) && (postC=DCT->getInChan(post)) && (outC=DCT->getOutChan(post)) ) {
                    Synapse *tmp = createAssigned(conductanceID, assignID, multi++, DCT, preC, postC, outC);
                    if ( pre.isVirtual && post.isAnalog )
                        manager->postD.push_back(tmp);
                    else
                        manager->preD.push_back(tmp);
                }
            }
        }
    } else {
        for ( ChannelIndex post : postSynInst ) {
            for ( ChannelIndex out : outSynInst ) {
                for ( ChannelIndex pre : preSynInst ) {
                    if ( (preC=DCT->getInChan(pre)) && ((postC=DCT->getInChan(post)) || post.isNone) && (outC=DCT->getOutChan(out)) ) {
                        Synapse *tmp = createAssigned(conductanceID, assignID, multi++, DCT, preC, postC, outC);
                        if ( (pre.isVirtual || post.isVirtual) && out.isAnalog )
                            manager->postD.push_back(tmp);
                        else
                            manager->preD.push_back(tmp);
                    }
                }
            }
        }
    }
}


Synapse::Synapse(size_t condID, size_t assignID, size_t multiID, inChannel *pre, inChannel *post, outChannel *out) :
    Conductance(condID, assignID, multiID),
    pre(pre),
    post(post),
    out(out),
    buffered(false)
{

}

void Synapse::setupBuffer(DCThread *DCT)
{
    if ( assignment().delay > 0. ) {
        bufferHandle = pre->getBufferHandle(assignment().delay, DCT->bufferHelper);
        buffered = true;
    }
}
