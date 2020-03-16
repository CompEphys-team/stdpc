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

#ifndef HH_H
#define HH_H

#include <cmath>
#include "IonicCurrent.h"
#include "Global_func.h"

struct mhHHData : public CurrentData {
  bool LUTables;
  double gMax;
  double Vrev;
  double mExpo;
  double hExpo;
  double Vm;
  double sm;
  double Cm;
  int taumType;
  double taum;
  double taumAmpl;
  double Vtaum;
  double staum;
  double Vh;
  double sh;
  double Ch;
  int tauhType;
  double tauh;
  double tauhAmpl;
  double Vtauh;
  double stauh;
};

class HHProxy : public IonicCurrentProxy
{
private:
    HHProxy();
public:
    HHProxy(const HHProxy &) = delete;
    void operator=(const HHProxy &) = delete;
    static HHProxy *get() { static HHProxy proxy; return &proxy; }

    inline mhHHData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "mhHH"; }
    inline QString prettyName() { return "m/h/tau HH"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<mhHHData> p;
};

class HH : public IonicCurrent
{
  private:
    const mhHHData *p;
    const CurrentAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    stdpc::function *theExpSigmoid;
    
  protected:
    double m, h;
    double km[4], kh[4], mi, hi;
    double minf, taum, hinf, tauh;

    bool mExpo_isInt, hExpo_isInt;
    int mExpo_int, hExpo_int;
    
  public:
    HH(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    void step(double t, double dt, bool settling);
    void RK4(double t, double dt, size_t n, bool settling);

    inline const mhHHData &params() const { return HHProxy::p[condID]; }
    HHProxy *proxy() const;
};

#endif
