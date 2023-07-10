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

#include "SpkGen.h"
#include "DCThread.h"
#include "ModelManager.h"
#include "AP.h"
#include "SpikeGenDlg.h"

/// Construct a single self-registering proxy
static SpkGenProxy *prox = SpkGenProxy::get();
std::vector<SGData> SpkGenProxy::p;
SpkGenProxy *SpkGen::proxy() const { return prox; }
Model *SpkGenProxy::instantiate(size_t modelID, size_t instID, DCThread *DCT) { return new SpkGen(modelID, instID, DCT); }
ModelDlg *SpkGenProxy::createDialog(size_t modelID, QWidget *parent) { return new SpikeGenDlg(modelID, parent); }

SpkGenProxy::SpkGenProxy()
{
    ModelManager::RegisterModel(modelClass(), this);
    AP *sgAc = addAP("SGp[#].active", &SpkGenProxy::p, &SGData::active);
               addAP("SGp[#].label", &SpkGenProxy::p, &SGData::label);
    AP *sgLU = addAP("SGp[#].LUTables", &SpkGenProxy::p, &SGData::LUTables);
    AP *sgVS = addAP("SGp[#].VSpike", &SpkGenProxy::p, &SGData::VSpike);
    AP *sgTS = addAP("SGp[#].spkTimeScaling", &SpkGenProxy::p, &SGData::spkTimeScaling);
    AP *sgVR = addAP("SGp[#].VRest", &SpkGenProxy::p, &SGData::VRest);
    AP *sgBT = addAP("SGp[#].bdType", &SpkGenProxy::p, &SGData::bdType);
               addAP("SGp[#].bdtUnder", &SpkGenProxy::p, &SGData::bdtUnder);
               addAP("SGp[#].bdtOver", &SpkGenProxy::p, &SGData::bdtOver);
               addAP("SGp[#].bdtUnderCont", &SpkGenProxy::p, &SGData::bdtUnderCont);
               addAP("SGp[#].bdtOverCont", &SpkGenProxy::p, &SGData::bdtOverCont);
               addAP("SGp[#].bdStrictlyCont", &SpkGenProxy::p, &SGData::bdStrictlyCont);
    AP *sgPd = addAP("SGp[#].period", &SpkGenProxy::p, &SGData::period);
               addAP("SGp[#].loopBursts", &SpkGenProxy::p, &SGData::loopBursts);
    AP *sgST = addAP("SGp[#].SpikeT[#][#]", &SpkGenProxy::p, &SGData::SpikeT);

               addAP("SGp[#].inst[#].active", &SpkGenProxy::p, &SGData::inst, &vInstData::active);
               addAP("SGp[#].inst[#].inChn.bias", &SpkGenProxy::p, &SGData::inst, &vInstData::inChn, &inChnData::bias);
    AP *sgSv = addAP("SGp[#].inst[#].inChn.chnlSaving", &SpkGenProxy::p, &SGData::inst, &vInstData::inChn, &inChnData::chnlSaving);
    AP *sgBC = addAP("SGp[#].inst[#].bdChannel", &SpkGenProxy::p, &SGData::inst, &SgInstData::bdChannel);
    AP *sgBH = addAP("SGp[#].inst[#].bdThresh", &SpkGenProxy::p, &SGData::inst, &SgInstData::bdThresh);

    addDeprecatedAP("SGp.active", sgAc);
    addDeprecatedAP("SGp.saving", sgSv);
    addDeprecatedAP("SGp.LUTables", sgLU);
    addDeprecatedAP("SGp.VSpike", sgVS);
    addDeprecatedAP("SGp.spkTimeScaling", sgTS);
    addDeprecatedAP("SGp.VRest", sgVR);
    addDeprecatedAP("SGp.bdType", sgBT);
    addDeprecatedAP("SGp.bdChannel", sgBC);
    addDeprecatedAP("SGp[#].bdChannel", sgBC);
    addDeprecatedAP("SGp.bdThresh", sgBH);
    addDeprecatedAP("SGp[#].bdThresh", sgBH);
    addDeprecatedAP("SGp.period", sgPd);
    addDeprecatedAP("SGp.SpikeT[#]", sgST, 2);
}


SpkGen::SpkGen(size_t modelID, size_t instID, DCThread *DCT) :
    Model(modelID, instID, DCT),
    p(&params()),
    instp(&instance()),
    V(p->VRest),
    bdChn(nullptr),
    SGactive(p->bdType == 0),
    burstDetected(false),
    tOverThresh(0.),
    tUnderThresh(0.),
    onThreshold(false),
    burstNo(0),
    period(p->period),
    epoch(0.),
    initial(true),
    active(true)
{
    if (p->LUTables) {
        theExp= &expLU;
        expLU.require(-50.0, 50.0, 0.02);
        theTanh= &tanhLU;
        tanhLU.require(-50.0, 50.0, 0.02);
    } else {
        theExp= &expFunc;
        theTanh= &tanhFunc;
    }
}

void SpkGen::post_init()
{
    Model::post_init();
    in.V = V + instp->inChn.bias;
    out.save = out.active = false;
    bdChn = DCT->getInChan(instp->bdChannel);
}

void SpkGen::RK4(double t, double dt, size_t n, bool settling)
{
    /// Cheapskate RK, because SG isn't really a candidate for it
    /// Note the 2*dt, which is the actual full step length for this RK cycle
    /// Note also that burst detection may be slightly inaccurate if the bd channel is another model
    if ( n == 0 && !settling )
        update(t, 2*dt);
    // Need neither further changes on `in` (nobody touches its voltage),
    // nor on `out` (it's inactive anyway)
}

void SpkGen::update(double t, double dt)
{
    if ( !active )
        return;

    if (initial) {
        epoch = t;
        spkIterator = p->SpikeT[burstNo].begin();
        spkOffsetCutoff = 100.0 / p->spkTimeScaling;
        initial = false;
    }

    if (p->bdType > 0 && !SGactive && bdChn ) { // taking care of the burst detection options
                                      // burst detection hasn't been completed yet ... spike generation inactive
        if (burstDetected) { // burst (first crossing) was detected, waiting for opposite crossing
            if (((p->bdType == 1) && (bdChn->V > instp->bdThresh)) ||
                ((p->bdType == 2) && (bdChn->V < instp->bdThresh))) {
                tOverThresh += dt;     // "under" and "over" only phrases for first and second cond. -- Note, this was reversed before v2017!
                if (tOverThresh > p->bdtOver) {
                    SGactive = true;
                    tUnderThresh = 0.;
                    burstDetected = false;
                    period = p->SpikeT[burstNo].back() + (20.0/p->spkTimeScaling);
                    epoch = t;
                }
            } else if ( p->bdtOverCont ) {
                if ( !onThreshold &&
                    (((p->bdType == 1) && (bdChn->V == instp->bdThresh)) ||
                     ((p->bdType == 2) && (bdChn->V == instp->bdThresh)))) {
                    onThreshold = true;
                } else {
                    onThreshold = false;
                    tOverThresh = 0.;
                    if ( p->bdStrictlyCont ) {
                        tUnderThresh = 0.;
                        burstDetected = false;
                    }
                }
            }
        } else {  // burst (first crossing) not yet detected ...
            if (((p->bdType == 1) && (bdChn->V < instp->bdThresh)) ||
                ((p->bdType == 2) && (bdChn->V > instp->bdThresh))) {
                tUnderThresh += dt;
                if (tUnderThresh > p->bdtUnder) {
                    tOverThresh = 0.;
                    burstDetected = true;
                }
            } else if ( p->bdtUnderCont ) {
                if ( !onThreshold &&
                    (((p->bdType == 1) && (bdChn->V == instp->bdThresh)) ||
                     ((p->bdType == 2) && (bdChn->V == instp->bdThresh)))) {
                    onThreshold = true;
                } else {
                    onThreshold = false;
                    tUnderThresh = 0.;
                }
            }
        }
    }

    // calculating SpkGen voltage for all cases:
    if (SGactive) {// the spike generation is active
        // routine that calculates the membrane potential of the CN taking
        // in account all the spikes (superimposing) the voltage due to each spike
        V = 0.0;

        // Advance spike iterator to only consider spikes starting after t-spkWidth
        while ( epoch + *spkIterator < t - spkOffsetCutoff && spkIterator != p->SpikeT[burstNo].end() )
            ++spkIterator;

        // Compute contributory voltage only from spikes in [t-spkWidth, t+spkWidth]
        for ( auto spkIt = spkIterator; spkIt != p->SpikeT[burstNo].end() && *spkIt < t + spkOffsetCutoff; ++spkIt )
            if ( *spkIt > 0 )
                V += VSpike((t - epoch - *spkIt) * p->spkTimeScaling);

        V *= p->VSpike;
        V += p->VRest;

        if (t > epoch + period) {
            if (p->bdType > 0) {
                SGactive = false;
            } else {
                epoch = t;
            }
            if ( ++burstNo == (int)p->SpikeT.size() ) {
                if ( p->loopBursts ) {
                    burstNo = 0;
                } else {
                    emit DCT->message(QString("Spike time data exhausted ... stopping spike Generator"));
                    V = p->VRest;
                    burstNo = 0;
                    active = false;
                }
            }
            spkIterator = p->SpikeT[burstNo].begin();
        }
    } else {
        V = p->VRest;
    }
    in.V = V + instp->inChn.bias;
}

// how to generate a spike
double SpkGen::VSpike(double t){
  #define A 1.0
  #define B 4.0
  #define TAU1 4.0
  #define TAU2 4.0
  #define T0 -0.576
  #define A1 2.0
  #define A2 2.0
  #define NORMALIZE 3.25394

  double f1, f2, f_spike;

  if ((t-T0)/TAU1 < 100.0)
    f1= (((*theTanh)(A1*(T0-t))+1.0)/2.0)*(*theExp)((t-T0)/TAU1);
  else f1= 0.0;

  if ((T0-t)/TAU2 < 100.0)
    f2= (((*theTanh)(A2*(t-T0))+1.0)/2.0)*(*theExp)((T0-t)/TAU2);
  else f2= 0.0;

  f_spike= (A*f1 + B*f2)/NORMALIZE;
  return(f_spike);

  #undef A
  #undef B
  #undef TAU1
  #undef TAU2
  #undef T0
  #undef A1
  #undef A2
#undef NORMALIZE
}
