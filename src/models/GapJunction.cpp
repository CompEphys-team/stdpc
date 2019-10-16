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

#include "GapJunction.h"
#include "DCThread.h"
#include "GapJunctionDlg.h"

static GapJunctionProxy *prox = GapJunctionProxy::get();
std::vector<GJunctData> GapJunctionProxy::p;
GapJunctionProxy *GapJunction::proxy() const { return prox; }
ConductanceDlg *GapJunctionProxy::createDialog(size_t condID, QWidget *parent) { return new GapJunctionDlg(condID, parent); }

GapJunctionProxy::GapJunctionProxy()
{
    ConductanceManager::RegisterSynapse(this);

    addAP("ESynp[#].active", &p, &GJunctData::active);
    addAP("ESynp[#].activeSettling", &p, &GJunctData::activeSettling);
    addAP("ESynp[#].label", &p, &GJunctData::label);
    addAP("ESynp[#].type", &p, &GJunctData::type);
    addAP("ESynp[#].gSyn", &p, &GJunctData::gSyn);
    addAP("ESynp[#].assign[#].active", &p, &GJunctData::assign, &GapJunctionAssignment::active);
    addAP("ESynp[#].assign[#].preInChannel", &p, &GJunctData::assign, &GapJunctionAssignment::preInChannel);
    addAP("ESynp[#].assign[#].postInChannel", &p, &GJunctData::assign, &GapJunctionAssignment::postInChannel);
    addAP("ESynp[#].assign[#].preOutChannel", &p, &GJunctData::assign, &GapJunctionAssignment::preOutChannel);
    addAP("ESynp[#].assign[#].postOutChannel", &p, &GJunctData::assign, &GapJunctionAssignment::postOutChannel);
    addAP("ESynp[#].assign[#].save", &p, &GJunctData::assign, &GapJunctionAssignment::save);

    addAP("ESynp[#].preInChannel", &p, &GJunctData::legacy_PreIn);
    addAP("ESynp[#].postInChannel", &p, &GJunctData::legacy_PostIn);
    addAP("ESynp[#].preOutChannel", &p, &GJunctData::legacy_PreOut);
    addAP("ESynp[#].postOutChannel", &p, &GJunctData::legacy_PostOut);
}

void GapJunctionProxy::instantiate(size_t condID, size_t assignID, DCThread *DCT, std::vector<Conductance *> &preD, std::vector<Conductance *> &, std::vector<Conductance *> &)
{
    const GapJunctionAssignment &a = param(condID).assignment(assignID);
    struct postChanPointers {
        inChannel *inC;
        outChannel *outC;
    };
    std::vector<postChanPointers> postChans;

    if ( a.postInChannel == a.postOutChannel ) {
        for ( ChannelIndex post : DCT->getChanIndices(a.postInChannel) ) {
            inChannel *inC = DCT->getInChan(post);
            outChannel *outC = DCT->getOutChan(post);
            if ( inC && outC ) {
                postChans.push_back({inC, outC});
            }
        }
    } else {
        for ( ChannelIndex in : DCT->getChanIndices(a.postInChannel) ) {
            for ( ChannelIndex out : DCT->getChanIndices(a.postOutChannel) ) {
                inChannel *inC = DCT->getInChan(in);
                outChannel *outC = DCT->getOutChan(out);
                if ( inC && outC ) {
                    postChans.push_back({inC, outC});
                }
            }
        }
    }

    size_t multi = 0;
    if ( a.preInChannel == a.preOutChannel ) {
        for ( ChannelIndex pre : DCT->getChanIndices(a.preInChannel) ) {
            inChannel *inC = DCT->getInChan(pre);
            outChannel *outC = DCT->getOutChan(pre);
            if ( inC && outC ) {
                for ( postChanPointers &post : postChans ) {
                    preD.push_back(new GapJunction(condID, assignID, multi++, inC, outC, post.inC, post.outC));
                }
            }
        }
    } else {
        for ( ChannelIndex in : DCT->getChanIndices(a.preInChannel) ) {
            for ( ChannelIndex out : DCT->getChanIndices(a.preOutChannel) ) {
                inChannel *inC = DCT->getInChan(in);
                outChannel *outC = DCT->getOutChan(out);
                if ( inC && outC ) {
                    for ( postChanPointers &post : postChans ) {
                        preD.push_back(new GapJunction(condID, assignID, multi++, inC, outC, post.inC, post.outC));
                    }
                }
            }
        }
    }
}

GapJunction::GapJunction(size_t condID, size_t assignID, size_t multiID, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost) :
    Conductance(condID, assignID, multiID),
    p(&params()),
    a(&assignment()),
    pre(pre),
    post(post),
    outpre(outpre),
    outpost(outpost)
{
}

void GapJunction::step(double, double, bool settling)
{
  if ( p->active && a->active && pre->active && post->active && outpre->active && outpost->active ) {
      // calculate synaptic current
      I= p->gSyn * (pre->V - post->V);
      if ((p->type == 1) && (I < 0.0)) {
           I = m_conductance = 0.0;
           return;
      } else {
          m_conductance = p->gSyn;
      }
      if ( !settling || p->activeSettling ) {
          outpost->I+= I;
          outpre->I-= I;
      }
  } else {
      m_conductance = 0;
  }
}

