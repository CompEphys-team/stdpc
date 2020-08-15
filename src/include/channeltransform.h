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

#ifndef CHANNELTRANSFORM_H
#define CHANNELTRANSFORM_H

#include "ConductanceManager.h"
#include "Channels.h"

class ChannelTransform : public Conductance
{
public:
    ChannelTransform(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    ~ChannelTransform() {}

    inline virtual const ChannelTransformAssignment &assignment() const { return params().assignment(assignID); }
    virtual const ChannelTransformData &params() const = 0;

    virtual void RK4(double, double, size_t, bool) {}
    virtual void step(double t, double dt, bool settling) = 0;

protected:
    bool &channel_active;
    double &channel_value;
};

class ChannelTransformProxy : public ConductanceProxy
{
public:
    virtual ChannelTransformData &param(size_t i) const = 0;
    void instantiate(size_t conductanceID, size_t assignID, DCThread *, ConductanceManager *manager);

    virtual ChannelTransform *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                             inChannel *in, outChannel *out) = 0;
};

#endif // CHANNELTRANSFORM_H
