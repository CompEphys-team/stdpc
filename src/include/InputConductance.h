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

#ifndef INPUTCONDUCTANCE_H
#define INPUTCONDUCTANCE_H

#include "Synapse.h"

struct InputConductanceData : public SynapseData
{
    double Vrev; //!< Reversal potential
    double gStatic; //!< Constant g factor
    bool rectify;
};


class InputConductanceProxy : public SynapseProxy {
private:
    InputConductanceProxy();
public:
    InputConductanceProxy(const InputConductanceProxy &) = delete;
    void operator=(const InputConductanceProxy &) = delete;
    static InputConductanceProxy *get() { static InputConductanceProxy proxy; return &proxy; }

    inline InputConductanceData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "InputConductance"; }
    inline QString prettyName() { return "Input conductance"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<InputConductanceData> p;
};


class InputConductance : public Synapse
{
private:
    const InputConductanceData *p;
    const SynapseAssignment *a;

public:
  InputConductance(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
  inline const InputConductanceData &params() const { return InputConductanceProxy::p[condID]; }
  void step(double t, double dt, bool settling);

  InputConductanceProxy *proxy() const;
};

#endif // INPUTCONDUCTANCE_H
