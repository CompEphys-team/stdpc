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

#ifndef IONICCURRENT_H
#define IONICCURRENT_H

#include "ConductanceManager.h"
#include "Channels.h"

class IonicCurrent : public Conductance
{
public:
    IonicCurrent(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    ~IonicCurrent() {}

    inline virtual const CurrentAssignment &assignment() const { return params().assignment(assignID); }
    virtual const CurrentData &params() const = 0;

    virtual void RK4(double t, double dt, size_t n, bool settling) = 0;
    virtual void step(double t, double dt, bool settling) = 0;

protected:
    inChannel *in;
    outChannel *out;
};

class IonicCurrentProxy : public ConductanceProxy
{
public:
    virtual CurrentData &param(size_t i) const = 0;
    void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                     std::vector<Conductance*> &preD,
                     std::vector<Conductance*> &inD,
                     std::vector<Conductance*> &postD);

    virtual IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                         inChannel *in, outChannel *out) = 0;
};

#endif // IONICCURRENT_H
