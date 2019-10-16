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

#ifndef ABHH_H
#define ABHH_H

#include "IonicCurrent.h"
#include "Global_func.h"

struct abHHData : public CurrentData {
  bool LUTables;
  double gMax;
  double Vrev;
  int mExpo;
  int hExpo;
  int maFunc;
  double mka;
  double mVa;
  double msa;
  int mbFunc;
  double mkb;
  double mVb;
  double msb;
  int haFunc;
  double hka;
  double hVa;
  double hsa;
  int hbFunc;
  double hkb;
  double hVb;
  double hsb;
};

class abHHProxy : public IonicCurrentProxy
{
private:
    abHHProxy();
public:
    abHHProxy(const abHHProxy &) = delete;
    void operator=(const abHHProxy &) = delete;
    static abHHProxy *get() { static abHHProxy proxy; return &proxy; }

    inline abHHData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "abHH"; }
    inline QString prettyName() { return "a/b HH"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<abHHData> p;
};

class abHH : public IonicCurrent
{
  private:
    const abHHData *p;
    const CurrentAssignment *a;

    stdpc::function *theExp;
    stdpc::function *theExpSigmoid;
    stdpc::function *theTanh;

    double mhFunc(double, int);
    
  protected:
    double m, h;
    double ma, mb, ha, hb;
    double km[4], kh[4], mi, hi;
    
  public:
    abHH(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);
    void step(double t, double dt, bool settling);
    void RK4(double t, double dt, size_t n, bool settling);

    inline const abHHData &params() const { return abHHProxy::p[condID]; }
    abHHProxy *proxy() const;
};

#endif
