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

#ifndef PIPEDAQ_H
#define PIPEDAQ_H

#include "DeviceManager.h"

class PipeDAQData : public DAQData {
  public:
    bool read, write;
    QuotedString readAddr, writeAddr; // Full socket addresses for PUB/SUB sockets, including protocol.
    bool bind_read, bind_write; // If true, use bind() instead of connect() on the sockets.

    PipeDAQData() : DAQData(),
        read(true),
        write(false),
        readAddr("tcp://localhost:5555"),
        writeAddr("tcp://localhost:5556"),
        bind_read(false),
        bind_write(false)
    {}
};

class PipeDAQProxy : public DAQProxy {
private:
    PipeDAQProxy();
    ~PipeDAQProxy();
    void *m_context;
public:
    PipeDAQProxy(const PipeDAQProxy &) = delete;
    void operator=(const PipeDAQProxy &) = delete;
    static PipeDAQProxy *get() { static PipeDAQProxy proxy; return &proxy; }

    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "PipeDAQ"; }

    DAQ *createDAQ(size_t devID);
    DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr);

    std::vector<AP*> regAP;
    inline const std::vector<AP*> &coreAPs() const { return regAP; }

    static std::vector<PipeDAQData> p;
    void *context() { return m_context; }
};

class PipeDAQ: public DAQ
{
  public:
    PipeDAQ(size_t devID, DAQProxy *proxy);
    virtual ~PipeDAQ();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void generate_scan_list(short int, QVector<short int>);
    virtual void generate_analog_out_list(short int, QVector<short>);
    virtual void get_scan(bool settling = false);
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out(bool settling = false);
    virtual void reset_board();

  private:
    int readBufSz = 0, writeBufSz = 0;
    double *readBuffer = nullptr, *writeBuffer = nullptr;

    bool connected = false;
    void *subscriber = nullptr;
    void *publisher = nullptr;

    bool connect();
    void disconnect();
};

#endif
