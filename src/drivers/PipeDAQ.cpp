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
    regAP {
        addAP("PipeDAQp[#].active", &p, &PipeDAQData::active),
        addAP("PipeDAQp[#].label", &p, &PipeDAQData::label),
        addAP("PipeDAQp[#].pipeName", &p, &PipeDAQData::pipeName),
        addAP("PipeDAQp[#].hostName", &p, &PipeDAQData::hostName),
        addAP("PipeDAQp[#].read", &p, &PipeDAQData::read),
        addAP("PipeDAQp[#].server", &p, &PipeDAQData::server),
        addAP("PipeDAQp[#].connectLater", &p, &PipeDAQData::connectLater),
        addAP("PipeDAQp[#].wait", &p, &PipeDAQData::wait),
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
    DeviceManager::RegisterDAQ(daqClass(), this);
}

//---------------------------------------------------------------------------

PipeDAQ::PipeDAQ(size_t devID, DAQProxy *proxy) :
    DAQ(devID, proxy)
{
    if ( PipeDAQProxy::p[devID].read ) {
        inChnNo= params()->inChn.size();
        outChnNo = 0;
    } else {
        inChnNo = 0;
        outChnNo = params()->outChn.size();
    }
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
    sampleBufSz = inChnNo + outChnNo;
    sampleBuffer = new double[sampleBufSz];
    backupSamples = new double[sampleBufSz];
    sampleBufSz *= sizeof(double);
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
    delete[] sampleBuffer;
    delete[] backupSamples;
    delete[] pipeName;
    disconnectPipe();
    removePipe();
}

QString PipeDAQ::connectPipe()
{
    if ( connected )
        return QString();
    if ( PipeDAQProxy::p[devID].server && hPipe != INVALID_HANDLE_VALUE ) { // PipeDAQ is server; connect to client
        DWORD err;
        connected = ConnectNamedPipe(hPipe, NULL) || ((err = GetLastError()) == ERROR_PIPE_CONNECTED);
        if ( !connected )
            return QString("PipeDAQ (%1) failed to connect to client, error %2").arg(QString::fromWCharArray(pipeName)).arg(err);
    } else {
        hPipe = CreateFile(
                    pipeName,
                    PipeDAQProxy::p[devID].read ? GENERIC_READ : GENERIC_WRITE,
                    0,              // no sharing
                    NULL,           // default security attributes
                    OPEN_EXISTING,  // opens existing pipe
                    0,              // default attributes
                    NULL);          // no template file
        if ( hPipe == INVALID_HANDLE_VALUE ) {
            DWORD err = GetLastError();
            if ( err == ERROR_PIPE_BUSY ) {
                if ( !WaitNamedPipe(pipeName, 10000) ) {
                    return QString("PipeDAQ (%1) failed to connect to server, timed out.").arg(QString::fromWCharArray(pipeName));
                } else {
                    connected = true;
                }
            } else {
                return QString("PipeDAQ (%1) failed to connect to server, error %2").arg(QString::fromWCharArray(pipeName)).arg(err);
            }
        } else {
            connected = true;
        }

        DWORD mode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
        SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);
    }
    return QString();
}

void PipeDAQ::disconnectPipe()
{
    connected = false;
    if ( hPipe == INVALID_HANDLE_VALUE )
        return;
    if ( PipeDAQProxy::p[devID].server ) {
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
    } else {
        removePipe();
    }
}

void PipeDAQ::removePipe()
{
    initialized = false;
    if ( hPipe != INVALID_HANDLE_VALUE ) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }
}

void PipeDAQ::start()
{
    if ( !connected )
        std::cout << "connecting in start " << connectPipe() << std::endl;
    memset(backupSamples, 0, sampleBufSz);
}

bool PipeDAQ::initialize_board(QString &name)
{
    // Set up pipeName
    PipeDAQData const& p = PipeDAQProxy::p[devID];
    QString pname = QString("\\\\%1\\pipe\\%2").arg((p.server||p.hostName.isEmpty()) ? "." : p.hostName, p.pipeName);
    if ( pname.length() > pipeName_len+1 ) {
        delete[] pipeName;
        pipeName = new wchar_t[(pipeName_len = pname.length()) + 1]; // +1 for \0
        pipeName[pipeName_len] = '\0';
    }
    pname.toWCharArray(pipeName);

    if ( hPipe != INVALID_HANDLE_VALUE ) {
        disconnectPipe();
        removePipe();
    }

    if ( PipeDAQProxy::p[devID].server ) {
        hPipe = CreateNamedPipe(
                    pipeName,
                    p.read ? PIPE_ACCESS_INBOUND : PIPE_ACCESS_OUTBOUND,
                    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                    1,       // max. instances
                    512,     // output buffer size
                    512,     // input buffer size
                    0,       // client time-out (0 = default, 50 ms)
                    NULL);   // default security attribute
        if ( hPipe == INVALID_HANDLE_VALUE ) {
            name = QString("PipeDAQ (%1) failed to create pipe, error %2").arg(QString::fromWCharArray(pipeName)).arg(GetLastError());
            initialized = false;
        } else {
            initialized = true;
        }
    } else {
        initialized = true; // Client has nothing to do at config
    }

    if ( initialized )
        name = QString("PipeDAQ (%1)").arg(QString::fromWCharArray(pipeName));
    return initialized;
}

void PipeDAQ::generate_scan_list(short int chnNo, QVector<short> Chns)
{
    std::cout << "connecting in generate_scan_list " << connectPipe() << std::endl;
    if ( (!connected && !PipeDAQProxy::p[devID].connectLater) || !PipeDAQProxy::p[devID].read ) {
        actInChnNo = 0;
        in.clear();
    } else {
        actInChnNo = chnNo;
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
    if ( !PipeDAQProxy::p[devID].read )
        return;
    if ( !connected && PipeDAQProxy::p[devID].connectLater ) {
        QString ret = connectPipe();
        if ( debug_counter++ % 100000 == 0 )
            std::cout << "connecting in get_scan " << ret << std::endl;
        if ( connected )
            std::cout << "connected in get_scan" << std::endl;
        if ( !connected )
            return;
    }

    DWORD nBytesRead, nBytesAvailable = 0;
    bool success;
    // Read available data, discarding all but the latest message
    do {
        success = ReadFile(hPipe, sampleBuffer, sampleBufSz, &nBytesRead, NULL);
        PeekNamedPipe(hPipe, NULL, 0, NULL, &nBytesAvailable, NULL);
        if ( success )
            std::cout << nBytesRead << ' ' << nBytesAvailable << ' ' << sampleBuffer[0] << std::endl;
    } while ( nBytesAvailable >= sampleBufSz );

    if ( success ) {
        for( int i = 0; i < actInChnNo; i++ ) {
            in[inIdx[i]].V = sampleBuffer[inIdx[i]] * inGainFac[i];
        }
        memcpy(backupSamples, sampleBuffer, sampleBufSz);
    } else {
        if ( GetLastError() == ERROR_NO_DATA ) {
            for( int i = 0; i < actInChnNo; i++ ) {
                in[inIdx[i]].V = backupSamples[inIdx[i]] * inGainFac[i];
            }
        } else if ( PipeDAQProxy::p[devID].connectLater ) {
            disconnectPipe();
        }
    }
}

void PipeDAQ::get_single_scan(inChannel *)
{
    // Only called for sample&hold, ignore
}


void PipeDAQ::generate_analog_out_list(short int chnNo, QVector<short int> Chns)
{
    connectPipe();
    if ( (!connected && !PipeDAQProxy::p[devID].connectLater) || PipeDAQProxy::p[devID].read ) {
        actOutChnNo = 0;
        out.clear();
    } else {
        ChannelIndex dex(prox, devID, 0, false);
        actOutChnNo = chnNo;
        for (int i = 0; i < actOutChnNo; i++) {
            outIdx[i]= Chns[i];
            outGainFac[i]= params()->outChn[outIdx[i]].gainFac*1.0e9;  // write nA
            dex.chanID = outIdx[i];
            outChnLabels[outIdx[i]] = dex.toString();
        }
        memset(sampleBuffer, 0, sampleBufSz);
    }
}

void PipeDAQ::write_analog_out(bool)
{
    if ( PipeDAQProxy::p[devID].read )
        return;
    if ( !connected && PipeDAQProxy::p[devID].connectLater ) {
        QString ret = connectPipe();
        if ( debug_counter++ % 100000 == 0 )
            std::cout << "connecting in get_scan " << ret << std::endl;
        if ( connected )
            std::cout << "connected in get_scan" << std::endl;
        if ( !connected )
            return;
    }

    for ( int i = 0; i < actOutChnNo; i++ )
        sampleBuffer[outIdx[i]] = out[outIdx[i]].I * outGainFac[i];
    DWORD nBytesWritten;
    bool success = WriteFile(hPipe, sampleBuffer, sampleBufSz, &nBytesWritten, NULL);
    if ( !success && PipeDAQProxy::p[devID].connectLater )
        disconnectPipe();
}

void PipeDAQ::reset_board()
{
    disconnectPipe();
}
