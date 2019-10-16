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

#ifndef INCLUDESYNAPTICNOISE_H
#define INCLUDESYNAPTICNOISE_H

#include "IonicCurrent.h"

/// Note: SynapticNoise follows the formalism laid out in:
///  Destexhe, A, M Rudolph, J. -M Fellous, and T. J Sejnowski.
///  ‘Fluctuating Synaptic Conductances Recreate in Vivo-like Activity in Neocortical Neurons’.
///  Neuroscience 107, no. 1 (9 November 2001): 13–24. doi:10.1016/S0306-4522(01)00344-X.
/// However, any one SynapticNoise instance only covers one synapse type (excitatory or inhibitory), not both.

struct SynapticNoiseData : public CurrentData
{
    double Vrev; //!< Reversal potential
    double tau; //!< Time constant
    double g0; //!< Average conductance
    double D__deprecated = -1; //!< Noise diffusion coefficient; deprecated in favour of std
    double std; //!< Standard deviation \sigma
    bool ignoreNegative;
};

class SynapticNoiseProxy : public IonicCurrentProxy
{
private:
    SynapticNoiseProxy();
public:
    SynapticNoiseProxy(const SynapticNoiseProxy &) = delete;
    void operator=(const SynapticNoiseProxy &) = delete;
    static SynapticNoiseProxy *get() { static SynapticNoiseProxy proxy; return &proxy; }

    inline SynapticNoiseData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "SynapticNoise"; }
    inline QString prettyName() { return "Syn. noise"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<SynapticNoiseData> p;
};

class SynapticNoise : public IonicCurrent
{
public:
    SynapticNoise(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);

    void step(double t, double dt, bool settling);
    void RK4(double t, double dt, size_t n, bool settling);
    
    inline const SynapticNoiseData &params() const { return SynapticNoiseProxy::p[condID]; }
    SynapticNoiseProxy *proxy() const;

protected:
    const SynapticNoiseData *p;
    const CurrentAssignment *a;
    double gNext;
    
    double next(double dt);
};

#endif // INCLUDESYNAPTICNOISE_H
