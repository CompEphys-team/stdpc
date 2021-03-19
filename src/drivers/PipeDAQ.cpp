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

#include "zmq.h"
#include "PipeDAQ.h"
#include "limits.h"
#include "AP.h"
#include "pipedaqdlg.h"

/// Construct a single self-registering proxy
static PipeDAQProxy *prox = PipeDAQProxy::get();
std::vector<PipeDAQData> PipeDAQProxy::p;
DAQ *PipeDAQProxy::createDAQ(size_t devID) { return new PipeDAQ(devID, prox); }
DAQDlg *PipeDAQProxy::createDialog(size_t devID, QWidget *parent) { return new PipeDAQDlg(devID, prox, parent); }

PipeDAQProxy::PipeDAQProxy() :
    m_context { zmq_ctx_new() },
    regAP {
        addAP("PipeDAQp[#].active", &p, &PipeDAQData::active),
        addAP("PipeDAQp[#].label", &p, &PipeDAQData::label),
        addAP("PipeDAQp[#].read", &p, &PipeDAQData::read),
        addAP("PipeDAQp[#].write", &p, &PipeDAQData::write),
        addAP("PipeDAQp[#].readAddr", &p, &PipeDAQData::readAddr),
        addAP("PipeDAQp[#].writeAddr", &p, &PipeDAQData::writeAddr),
        addAP("PipeDAQp[#].bind_read", &p, &PipeDAQData::bind_read),
        addAP("PipeDAQp[#].bind_write", &p, &PipeDAQData::bind_write),
        addAP("PipeDAQp[#].inChn[#].active", &p, &PipeDAQData::inChn, &inChnData::active),
        addAP("PipeDAQp[#].inChn[#].gain", &p, &PipeDAQData::inChn, &inChnData::gain),
        addAP("PipeDAQp[#].inChn[#].gainFac", &p, &PipeDAQData::inChn, &inChnData::gainFac),
        addAP("PipeDAQp[#].inChn[#].spkDetect", &p, &PipeDAQData::inChn, &inChnData::spkDetect),
        addAP("PipeDAQp[#].inChn[#].spkDetectThresh", &p, &PipeDAQData::inChn, &inChnData::spkDetectThresh),
        addAP("PipeDAQp[#].inChn[#].bias", &p, &PipeDAQData::inChn, &inChnData::bias),
        addAP("PipeDAQp[#].inChn[#].chnlSaving", &p, &PipeDAQData::inChn, &inChnData::chnlSaving),
        addAP("PipeDAQp[#].outChn[#].active", &p, &PipeDAQData::outChn, &outChnData::active),
        addAP("PipeDAQp[#].outChn[#].gain", &p, &PipeDAQData::outChn, &outChnData::gain),
        addAP("PipeDAQp[#].outChn[#].gainFac", &p, &PipeDAQData::outChn, &outChnData::gainFac),
        addAP("PipeDAQp[#].outChn[#].bias", &p, &PipeDAQData::outChn, &outChnData::bias),
        addAP("PipeDAQp[#].outChn[#].chnlSaving", &p, &PipeDAQData::outChn, &outChnData::chnlSaving)
    }
{
    if ( m_context == nullptr ) {
        std::cerr << "PipeDAQ: Error setting up ZMQ context: " << zmq_strerror(zmq_errno()) << std::endl;
    } else {
        DeviceManager::RegisterDAQ(daqClass(), this);
    }
}

PipeDAQProxy::~PipeDAQProxy() {
    zmq_ctx_term(m_context);
}

//---------------------------------------------------------------------------

PipeDAQ::PipeDAQ(size_t devID, DAQProxy *proxy) :
    DAQ(devID, proxy)
{
    inChnNo = PipeDAQProxy::p[devID].read ? params()->inChn.size() : 0;
    outChnNo = PipeDAQProxy::p[devID].write ? params()->outChn.size() : 0;
    inIdx= new short int[inChnNo];
    inGainFac= new double[inChnNo];
    outIdx= new short int[outChnNo];
    outGainFac= new double[outChnNo];
    inGainNo= 1;
    inGainText= new char*[inGainNo];
    inGainText[0]= new char[80];
    strcpy(inGainText[0], "n/a");
    inLow = QVector<double>(inGainNo);
    inLow[0] = -1e10;
    inHigh = QVector<double>(inGainNo);
    inHigh[0] = 1e10;
    outGainNo= 1;
    outGainText= new char*[outGainNo];
    outGainText[0]= new char[80];
    strcpy(outGainText[0], "n/a");
    outLow = QVector<double>(outGainNo);
    outLow[0] = -1e10;
    outHigh = QVector<double>(outGainNo);
    outHigh[0] = +1e10;

    readBuffer = new double[inChnNo];
    writeBuffer = new double[outChnNo];
}

PipeDAQ::~PipeDAQ()
{
    delete[] inIdx;
    delete[] inGainFac;
    delete[] outIdx;
    delete[] outGainFac;
    delete[] inGainText[0];
    delete[] inGainText;
    delete[] outGainText[0];
    delete[] outGainText;

    delete[] readBuffer;
    delete[] writeBuffer;

    disconnect();
}

bool PipeDAQ::connect()
{
    if ( connected )
        return true;
    PipeDAQData &p = PipeDAQProxy::p[devID];
    int ret, opt, sz = sizeof(int);
    if ( p.read ) {
        subscriber = zmq_socket(prox->context(), ZMQ_SUB);
        opt = 1; zmq_setsockopt(subscriber, ZMQ_CONFLATE, &opt, sz); // Discard anything but the latest message. Note, not multipart compatible.
        opt = inChnNo*sizeof(double); zmq_setsockopt(subscriber, ZMQ_MAXMSGSIZE, &opt, sz); // Accept #inchans doubles per message.
        if ( p.bind_read )
            ret = zmq_bind(subscriber, p.readAddr.toStdString().c_str());
        else
            ret = zmq_connect(subscriber, p.readAddr.toStdString().data());
        if ( ret != 0 ) {
            std::cerr << "PipeDAQ " << PipeDAQProxy::p[devID].label << ": Error "
                      << (p.bind_read ? "binding" : "connecting") << " to read socket "
                      << p.readAddr << ": " << zmq_strerror(zmq_errno()) << std::endl;
            disconnect();
            return false;
        }
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0); // Subscribe to everything.
        connected = true;
    }
    if ( p.write ) {
        publisher = zmq_socket(prox->context(), ZMQ_PUB);
        opt = 0; zmq_setsockopt(publisher, ZMQ_LINGER, &opt, sz); // Discard messages on close.
        if ( p.bind_write )
            ret = zmq_bind(publisher, p.writeAddr.toStdString().c_str());
        else
            ret = zmq_connect(publisher, p.writeAddr.toStdString().data());
        if ( ret != 0 ) {
            std::cerr << "PipeDAQ " << PipeDAQProxy::p[devID].label << ": Error "
                      << (p.bind_write ? "binding" : "connecting") << " to write socket "
                      << p.writeAddr << ": " << zmq_strerror(ret) << std::endl;
            disconnect();
            return false;
        }
        connected = true;
    }
    return true;
}

void PipeDAQ::disconnect()
{
    zmq_close(subscriber);
    zmq_close(publisher);
    subscriber = nullptr;
    publisher = nullptr;
    connected = false;
}

void PipeDAQ::start()
{
    if ( !connected && !connect() )
        return;
}

bool PipeDAQ::initialize_board(QString &name)
{
    bool contd = connected;
    PipeDAQData &p = PipeDAQProxy::p[devID];
    if ( !connect() ) {
        name = p.label;
        initialized = false;
    } else {
        if ( !connected ) {
            name = QString("mute/blind %1").arg(p.label);
        } else {
            QString read_name = (subscriber==nullptr) ? QString() : p.readAddr;
            QString write_name = (publisher==nullptr) ? QString() : p.writeAddr;
            QString separator = (read_name.isEmpty() || write_name.isEmpty()) ? QString() : QString(", ");
            QString continued = contd ? QString("Maintained existing connection to ") : QString();
            name = QString("%1%2%3%4").arg(continued, read_name, separator, write_name);
        }
        initialized = true;
    }
    return initialized;
}

void PipeDAQ::generate_scan_list(short int chnNo, QVector<short> Chns)
{
    connect();
    if ( subscriber == nullptr ) {
        actInChnNo = 0;
        in.clear();
    } else {
        actInChnNo = chnNo;
        readBufSz = actInChnNo * sizeof(double);
        ChannelIndex dex(prox, devID, 0, true);
        for(int i = 0; i < actInChnNo; i++) {
            inIdx[i]= Chns[i];
            inGainFac[i]= params()->inChn[inIdx[i]].gainFac;
            dex.chanID = inIdx[i];
            inChnLabels[inIdx[i]] = dex.toString();
        }
    }
}

void PipeDAQ::get_scan(bool)
{
    if ( subscriber == nullptr )
        return;

    int size = 0;
    size = zmq_recv(subscriber, readBuffer, readBufSz, ZMQ_DONTWAIT);
    if ( size == -1 && zmq_errno() != EAGAIN ) {
        std::cerr << "PipeDAQ " << PipeDAQProxy::p[devID].label << ": Error reading from socket: "
                  << zmq_strerror(zmq_errno()) << std::endl;
        return;
    }
    for ( int i = 0; i < actInChnNo; i++) {
        in[inIdx[i]].V = readBuffer[inIdx[i]] * inGainFac[i];
    }
}

void PipeDAQ::get_single_scan(inChannel *)
{
    // Only called for sample&hold, ignore
}


void PipeDAQ::generate_analog_out_list(short int chnNo, QVector<short int> Chns)
{
    connect();
    if ( publisher == nullptr ) {
        actOutChnNo = 0;
        out.clear();
    } else {
        ChannelIndex dex(prox, devID, 0, false);
        actOutChnNo = chnNo;
        writeBufSz = actOutChnNo * sizeof(double);
        for (int i = 0; i < actOutChnNo; i++) {
            outIdx[i]= Chns[i];
            outGainFac[i]= params()->outChn[outIdx[i]].gainFac*1.0e9;  // write nA
            dex.chanID = outIdx[i];
            outChnLabels[outIdx[i]] = dex.toString();
        }
    }
}

void PipeDAQ::write_analog_out(bool)
{
    if ( publisher == nullptr )
        return;
    for ( int i = 0; i < actOutChnNo; i++ )
        writeBuffer[outIdx[i]] = out[outIdx[i]].I * outGainFac[i];
    int ret = zmq_send(publisher, writeBuffer, writeBufSz, 0);
    if ( ret == -1 ) {
        std::cerr << "PipeDAQ " << PipeDAQProxy::p[devID].label << ": Error writing to socket: "
                  << zmq_strerror(zmq_errno()) << std::endl;
    }
}

void PipeDAQ::reset_board()
{
    disconnect();
}
