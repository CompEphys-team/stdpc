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

#ifndef DAQ_H
#define DAQ_H

#include "Clock.h"
#include "Channels.h"
#include <QPair>

class DAQProxy;

class DAQ {
  private:
    LARGE_INTEGER intClock_frequency;
    double sysT;

  protected:
    static constexpr double upTolFac = 0.99;
    static constexpr double loTolFac = 0.01;
    QVector<double> iHiLim;
    QVector<double> iLoLim;
    QVector<double> vHiLim;
    QVector<double> vLoLim;
    QVector<QString> inChnLabels;
    QVector<QString> outChnLabels;

  public:
    DAQ(size_t devID, DAQProxy *proxy);
    virtual ~DAQ();
    virtual bool initialize_board(QString &)= 0;
    virtual void start() = 0;
    virtual void generate_scan_list(short int, QVector<short int>)= 0;
    virtual void generate_analog_out_list(short int, QVector<short int> )= 0;
    virtual void get_scan(bool settling = false)= 0;
    virtual void get_single_scan(inChannel *)= 0;
    virtual void write_analog_out(bool settling = false)= 0;
    virtual void settling_complete() {} //!< Called when DC exits settling loop and enters standard clamp mode.
    virtual void reset_board()= 0;

    virtual inline void armTrigger(ChannelIndex) {}
    virtual inline bool triggerFired() { return true; }

    DAQData *params();

    void init_chans(); // Sets up scan list, analog out list
    void reset_chans();
    void process_scan(double t); // detects spikes, updates channel buffer
    QPair<QVector<ChannelIndex>, QVector<const double *>> valuesToSave();
    QVector<AECChannel*> aecChans();

    struct ChannelLimitWarning {QString what; QString chan_label; double hiLim; double loLim; double value;};
    bool check_limits(bool checkV_and_warn, ChannelLimitWarning &w);

    size_t devID;
    DAQProxy *proxy;

    const double &t;

    int inChnNo;
    int outChnNo;
    int digInChnNo;

    int inGainNo;
    QVector<double> inLow;
    QVector<double> inHigh;
    char **inGainText;

    int outGainNo;
    QVector<double> outLow;
    QVector<double> outHigh;
    char **outGainText;

// activated stuff
    short int actInChnNo;
    short int *inIdx;
    double *inGainFac;
    short int actOutChnNo;
    short int *outIdx;
    double *outGainFac;

    QVector<inChannel> in;
    QVector<outChannel> out;

    bool initialized;
};

#endif
