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

#pragma once

#include "ModelManager.h"

struct VStepsData : public ModelData
{
  double holdV;
  double startV;
  int nSteps;
  double deltaV;
  double t0;
  double tStep;
  double tHold;
  vInstData inst;

  inline const vInstData &instance(size_t) const { return inst; }
  inline size_t numInst() const { return 1; }

  VStepsData();
};

class VStepsProxy : public ModelProxy {
private:
    VStepsProxy();
public:
    VStepsProxy(const VStepsProxy &) = delete;
    void operator=(const VStepsProxy &) = delete;
    static VStepsProxy *get() { static VStepsProxy proxy; return &proxy; }

    inline VStepsData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString modelClass() { return "VStep"; }
    inline QString prettyName() { return "Voltage stepper"; }

    Model *instantiate(size_t modelID, size_t instID, DCThread *);
    ModelDlg *createDialog(size_t modelID, QWidget *parent);

    static std::vector<VStepsData> p;
};

class VSteps : public Model
{
public:
    VSteps(size_t modelID, size_t instID, DCThread *);
    ~VSteps() {}

    // Override processing on unused out
    inline void updateOut(double) {}
    inline void retainCurrent(double) {}
    inline void restoreCurrent(double) {}

    void update(double t, double dt);
    virtual void RK4(double t, double dt, size_t n, bool settling);

    inline VStepsData &params() const { return VStepsProxy::p[modID]; }
    VStepsProxy *proxy() const;

protected:
    const VStepsData * const p;
    const vInstData * const instp;
    double V;
    std::vector<double> cmdT, cmdV;
    std::vector<double>::iterator cmdTI, cmdVI;
};
