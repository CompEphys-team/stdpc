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

#ifndef CONDUCTANCE_H
#define CONDUCTANCE_H

#include "ObjectDataTypes.h"

class ConductanceProxy;

class Conductance
{
public:
    Conductance(size_t condID, size_t assignID, size_t multiID) :
        condID(condID), assignID(assignID), multiID(multiID), m_conductance(0) {}
    virtual ~Conductance() {}

    /// Compute Runke-Kutta step n (n = 0..3), in this order:
    ///  - Compute k_n based on the previous intermediate state (= null state, when n=0)
    ///  - Provide output to channels based on the previous intermediate state
    ///  - Update intermediate state based on k_n and null state
    ///  - at n=3, update null state based on k_{0..3} (nullstate += (k0 + 2k1 + 2k2 + k3)/6 * dt)
    /// Note, out.I must be reset to the retained value at the end of every step, except for the last one (n=3).
    virtual void RK4(double t, double dt, size_t n, bool settling) = 0;

    /// Compute a forward-integrating time step, updating m_conductance and adding current to the output channel(s)
    virtual void step(double t, double dt, bool settling) = 0;

    inline const AssignmentData &assignment() const { return params().assignment(assignID); }
    inline size_t assignmentID() const { return assignID; }

    virtual const ConductanceData &params() const = 0;
    inline size_t conductanceID() const { return condID; }

    inline size_t multiplexID() const { return multiID; }

    virtual ConductanceProxy *proxy() const = 0;

    inline const double &conductance() const { return m_conductance; }

protected:
    const size_t condID;
    const size_t assignID;
    const size_t multiID;

    double m_conductance;
};

#endif // CONDUCTANCE_H
