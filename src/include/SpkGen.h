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

#ifndef SPKGEN_H
#define SPKGEN_H

#include "Global_func.h"
#include "ModelManager.h"

struct SgInstData : public vInstData {
    ChannelIndex bdChannel;
    double bdThresh;
};

struct SGData : public ModelData {
  bool LUTables;
  double VSpike;
  double spkTimeScaling;
  double VRest;

  int bdType;
  double bdtUnder;
  double bdtOver;
  bool bdtUnderCont;
  bool bdtOverCont;
  bool bdStrictlyCont;

  double period;
  bool loopBursts;
  std::vector<std::vector<double>> SpikeT;

  std::vector<SgInstData> inst;

  inline const SgInstData &instance(size_t i) const { return inst[i]; }
  inline size_t numInst() const { return inst.size(); }
};

class SpkGenProxy : public ModelProxy {
private:
    SpkGenProxy();
public:
    SpkGenProxy(const SpkGenProxy &) = delete;
    void operator=(const SpkGenProxy &) = delete;
    static SpkGenProxy *get() { static SpkGenProxy proxy; return &proxy; }

    inline SGData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString modelClass() { return "SG"; }
    inline QString prettyName() { return "Spike generator"; }

    Model *instantiate(size_t modelID, size_t instID, DCThread *);
    ModelDlg *createDialog(size_t modelID, QWidget *parent);

    static std::vector<SGData> p;
};

class SpkGen : public Model
{
public:
    SpkGen(size_t modelID, size_t instID, DCThread *);
    ~SpkGen() {}

    void post_init(DCThread *);

    // Override processing on unused out
    inline void updateOut(double) {}
    inline void retainCurrent(double) {}
    inline void restoreCurrent(double) {}

    void update(double t, double dt);
    void RK4(double, double, size_t, bool settling);

    inline SGData &params() const { return SpkGenProxy::p[modID]; }
    inline const SgInstData &instance() const { return params().instance(instID); }
    SpkGenProxy *proxy() const;

protected:
    const SGData * const p;
    const SgInstData * const instp;
    double V;

    stdpc::function *theExp;
    stdpc::function *theTanh;
    inChannel *bdChn;
    bool SGactive;
    bool burstDetected;
    double tOverThresh;
    double tUnderThresh;
    bool onThreshold;
    int burstNo;
    double period;
    double epoch;
    bool initial;
    bool active;

    double spkOffsetCutoff;
    std::vector<double>::const_iterator spkIterator;

    double VSpike(double t);
};

#endif
