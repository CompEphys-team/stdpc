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

#include "SynapticNoise.h"
#include "SynapticNoiseDlg.h"
#include "AP.h"

static SynapticNoiseProxy *prox = SynapticNoiseProxy::get();
std::vector<SynapticNoiseData> SynapticNoiseProxy::p;
SynapticNoiseProxy *SynapticNoise::proxy() const { return prox; }
ConductanceDlg *SynapticNoiseProxy::createDialog(size_t condID, QWidget *parent) { return new SynapticNoiseDlg(condID, parent); }
IonicCurrent *SynapticNoiseProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new SynapticNoise(conductanceID, assignID, multiID, in, out);
}

SynapticNoiseProxy::SynapticNoiseProxy()
{
    ConductanceManager::RegisterTool(this);

    addAP("SynapticNoisep[#].active", &p, &SynapticNoiseData::active);
    addAP("SynapticNoisep[#].activeSettling", &p, &SynapticNoiseData::activeSettling);
    addAP("SynapticNoisep[#].label", &p, &SynapticNoiseData::label);
    addAP("SynapticNoisep[#].Vrev", &p, &SynapticNoiseData::Vrev);
    addAP("SynapticNoisep[#].tau", &p, &SynapticNoiseData::tau);
    addAP("SynapticNoisep[#].g0", &p, &SynapticNoiseData::g0);
    addAP("SynapticNoisep[#].D", &p, &SynapticNoiseData::D__deprecated)->setReadOnly();
    addAP("SynapticNoisep[#].std", &p, &SynapticNoiseData::std);
    addAP("SynapticNoisep[#].assign[#].active", &p, &SynapticNoiseData::assign, &CurrentAssignment::active);
    addAP("SynapticNoisep[#].assign[#].VChannel", &p, &SynapticNoiseData::assign, &CurrentAssignment::VChannel);
    addAP("SynapticNoisep[#].assign[#].IChannel", &p, &SynapticNoiseData::assign, &CurrentAssignment::IChannel);
    addAP("SynapticNoisep[#].assign[#].save", &p, &SynapticNoiseData::assign, &CurrentAssignment::save);

    AP::hooks_post_load().push_back([](){
        for ( SynapticNoiseData &data : p ) {
            if ( data.D__deprecated >= 0 ) {
                // D was deprecated on 2019-10-03.
                // D = 2 std^2 / tau
                // => std = sqrt(D*tau/2)
                data.std = std::sqrt(data.D__deprecated * data.tau / 2);
            }
        }
    });
}

SynapticNoise::SynapticNoise(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    IonicCurrent(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{
    m_conductance = params().g0;
}

void SynapticNoise::step(double, double dt, bool settling)
{
    if ( p->active && a->active && in->active && out->active ) {
        m_conductance = next(dt);
        if ( ( !p->ignoreNegative || m_conductance > 0 ) && ( !settling || p->activeSettling ) )
            out->I += m_conductance * (p->Vrev - in->V);
    } else {
        m_conductance = 0;
    }
}

void SynapticNoise::RK4(double, double dt, size_t n, bool settling)
{
    if ( p->active && a->active && in->active && out->active ) {
        if ( n == 0 ) // RK0: Estimate gradient at the step's starting point, m_conductance unchanged from previous
            gNext = next(2*dt); // 2*dt, because dt on RK step 0 is half the full step length
        else if ( n == 1 ) // RK1, RK2: Estimate gradient at (temporal) midpoint
            m_conductance = 0.5 * (m_conductance + gNext);
        else if ( n == 3 ) // RK3: Estimate gradient at endpoint
            m_conductance = gNext;

        if ( ( !p->ignoreNegative || m_conductance > 0 ) && ( !settling || p->activeSettling ) )
            out->I += m_conductance * (p->Vrev - in->V);
    } else {
        m_conductance = 0;
    }
}

double SynapticNoise::next(double dt)
{
    double tmp = std::exp(-dt/p->tau);
    return p->g0 + (m_conductance - p->g0) * tmp + p->std * std::sqrt(1 - tmp*tmp) * RNG.variate<double>(0, 1);
}
