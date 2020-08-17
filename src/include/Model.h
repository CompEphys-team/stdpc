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

#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include <QPair>
#include <QVector>
#include <QString>
#include <QObject>
#include "Channels.h"

class ModelProxy;

class Model : public QObject
{
    Q_OBJECT
public:
    Model(size_t modelID, size_t instID, DCThread *DCT);
    virtual ~Model() {}

    /// Post-init call for any additional setup that requires other models to be present
    /// Derived classes should call Model::post_init in their overridden versions to ensure complete channel setup.
    virtual void post_init(DCThread *);

    /// Process channels:
    /// Applies bias to out.I and calls in.process(t).
    virtual void updateChannels(double t);
    virtual inline void updateIn(double t) { in.process(t); }
    virtual inline void updateOut(double /*t*/) { out.I = instance().outChn.bias; }

    /// Retain out.I (which includes bias + any a2d contributions) prior to entering RK4 iterations
    virtual inline void retainCurrent(double /*t*/) { retainedI = out.I; }

    /// Reset out.I to the retained value at the start of each new RK4 iteration (note: full iteration, not step)
    virtual inline void restoreCurrent(double /*t*/) { out.I = retainedI; }

    /// Compute Runke-Kutta step n (n = 0..3), in this order:
    ///  - Compute k_n based on the previous intermediate state (= null state, when n=0)
    ///  - Provide output to channels based on the previous intermediate state
    ///  - Update intermediate state based on k_n and null state
    ///  - at n=3, update null state based on k_{0..3} (nullstate += (k0 + 2k1 + 2k2 + k3)/6 * dt)
    /// Note, out.I must be reset to the retained value at the end of every step, except for the last one (n=3).
    virtual void RK4(double t, double dt, size_t n, bool settling) = 0;

    inline virtual const vInstData &instance() const { return params().instance(instID); }
    inline size_t instanceID() const { return instID; }

    virtual const ModelData &params() const = 0;
    inline size_t modelID() const { return modID; }

    virtual ModelProxy *proxy() const = 0;

    inChannel in;
    outChannel out;

signals:
    void message(const QString &);

protected:
    const size_t modID;
    const size_t instID;

    double retainedI;
};

#endif // MODEL_H
