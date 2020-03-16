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

#ifndef NIDAQ_H
#define NIDAQ_H

#include "DeviceManager.h"
#include <NIDAQmx.h>

#define MAXRANGES 64
#define MAXCHANNELS 64

class NIDAQData : public DAQData
{
public:
    QString deviceName;
    NIDAQData() : DAQData(), deviceName("Dev1") {}
};

class NIDAQProxy : public DAQProxy {
private:
    NIDAQProxy();
public:
    NIDAQProxy(const NIDAQProxy &) = delete;
    void operator=(const NIDAQProxy &) = delete;
    static NIDAQProxy *get() { static NIDAQProxy proxy; return &proxy; }

    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "NIDAQ"; }
    inline QString prettyName() { return "Nat'l Instruments"; }

    DAQ *createDAQ(size_t devID);
    DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr);

    std::vector<AP*> regAP;
    inline const std::vector<AP*> &coreAPs() const { return regAP; }

    static std::vector<NIDAQData> p;
};

class NIDAQ: public DAQ
{
  private:
   char *devName;
   char *iChnNm[MAXCHANNELS];
   char *oChnNm[MAXCHANNELS];
   char *diChnNm[MAXCHANNELS];
          
   TaskHandle inTask;
   int inTaskActive;
   float64 *inBuf;

   TaskHandle outTask;
   int outTaskActive;
   float64 *outBuf;

   TaskHandle digInTask;
   int digInTaskActive;

   int DevicePresent;
             
  public:
    NIDAQ(size_t devID, DAQProxy *proxy);
    virtual ~NIDAQ();
    void init();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void digital_out(unsigned char outbyte);
    virtual void generate_scan_list(short int, QVector<short>);
    virtual void generate_analog_out_list(short int, QVector<short>);
    virtual void get_scan(bool settling = false);
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out(bool settling = false);
    virtual void reset_board();

    virtual void armTrigger(ChannelIndex trigChn);
    virtual bool triggerFired();
};

#endif
