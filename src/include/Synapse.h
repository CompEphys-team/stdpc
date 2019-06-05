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

#ifndef SYN_H
#define SYN_H

#include "ConductanceManager.h"
#include "Channels.h"

class Synapse : public Conductance
{
public:
    Synapse(size_t condID, size_t assignID, size_t multiID, inChannel *pre, inChannel *post, outChannel *out);
    ~Synapse() {}

    virtual const SynapseAssignment &assignment() const { return params().assignment(assignID); }
    virtual const SynapseData &params() const = 0;

    virtual void RK4(double, double, size_t, bool) {}
    virtual void step(double t, double dt, bool settling) = 0;

protected:
    inChannel *pre;
    inChannel *post;
    outChannel *out;

    void setupBuffer(DCThread *DCT);
    size_t bufferHandle;
    bool buffered;
};

class SynapseProxy : public ConductanceProxy
{
public:
    virtual SynapseData &param(size_t i) const = 0;
    void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                    std::vector<Conductance*> &preD,
                    std::vector<Conductance*> &inD,
                    std::vector<Conductance*> &postD);

    virtual Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                                    inChannel *pre, inChannel *post, outChannel *out) = 0;
};

#endif // SYN_H
