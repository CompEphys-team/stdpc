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

#ifndef ABSYN_H
#define ABSYN_H

#include "Global_func.h"
#include "Synapse.h"

struct abSynData : public SynapseData {
  bool LUTables;
  double gSyn;
  double Vrev;
  double aS;
  double bS;
  double aR;
  double VaR;
  double saR;
  double bR;
  int fixVpost;
  double Vpost;
  int Plasticity;
  STPlast ST;
  ODEPlast ODE;
};

class abSynProxy : public SynapseProxy {
private:
    abSynProxy();
public:
    abSynProxy(const abSynProxy &) = delete;
    void operator=(const abSynProxy &) = delete;
    static abSynProxy *get() { static abSynProxy proxy; return &proxy; }

    inline abSynData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "abSyn"; }
    inline QString prettyName() { return "a/b Syn"; }

    Synapse *createAssigned(size_t conductanceID, size_t assignID, size_t multiID, DCThread *,
                            inChannel *pre, inChannel *post, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<abSynData> p;
};

class abSyn : public Synapse {
  private:
    const abSynData *p;
    const SynapseAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;

    double P_f(double);
    double D_f(double);
    double invGFilter(double);
    double gFilter(double);

    void STlearn(double);
    double STDPFunc(double);
    void ODElearn(double);
    
  protected:
    double S;
    double R;
    double g;
    double graw;
    double P;  // "potentiation var" in ODE STDP
    double D;  // "depression var" in ODE STDP
    double Pslope; // slope of the sigmoid for P
    double Dslope; // slope of the sigmoid for D

  public:
    abSyn(size_t condID, size_t assignID, size_t multiID, DCThread *, inChannel *pre, inChannel *post, outChannel *out);
    inline const abSynData &params() const { return abSynProxy::p[condID]; }
    void step(double t, double dt, bool settling);

    abSynProxy *proxy() const;
};

#endif
