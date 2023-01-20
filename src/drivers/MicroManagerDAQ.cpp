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

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <string>

#include "MicroManagerDAQ.h"
#include "limits.h"
#include "AP.h"
#include "MicroManagerDAQDlg.h"

/// Construct a single self-registering proxy
static MicroManagerDAQProxy *prox = MicroManagerDAQProxy::get();
std::vector<MicroManagerDAQData> MicroManagerDAQProxy::p;
DAQ *MicroManagerDAQProxy::createDAQ(size_t devID) { return new MicroManagerDAQ(devID, prox); }
DAQDlg *MicroManagerDAQProxy::createDialog(size_t devID, QWidget *parent) { return new MicroManagerDAQDlg(devID, prox, parent); }

MicroManagerDAQProxy::MicroManagerDAQProxy() :
    regAP {
        addAP("MicroManagerDAQp[#].active", &p, &MicroManagerDAQData::active),
        addAP("MicroManagerDAQp[#].label", &p, &MicroManagerDAQData::label),
        addAP("MicroManagerDAQp[#].inChn[#].active", &p, &MicroManagerDAQData::inChn, &inChnData::active),
        addAP("MicroManagerDAQp[#].inChn[#].gain", &p, &MicroManagerDAQData::inChn, &inChnData::gain),
        addAP("MicroManagerDAQp[#].inChn[#].gainFac", &p, &MicroManagerDAQData::inChn, &inChnData::gainFac),
        addAP("MicroManagerDAQp[#].inChn[#].spkDetect", &p, &MicroManagerDAQData::inChn, &inChnData::spkDetect),
        addAP("MicroManagerDAQp[#].inChn[#].spkDetectThresh", &p, &MicroManagerDAQData::inChn, &inChnData::spkDetectThresh),
        addAP("MicroManagerDAQp[#].inChn[#].bias", &p, &MicroManagerDAQData::inChn, &inChnData::bias),
        addAP("MicroManagerDAQp[#].inChn[#].chnlSaving", &p, &MicroManagerDAQData::inChn, &inChnData::chnlSaving)
    }
{
    DeviceManager::RegisterDAQ(daqClass(), this);

    // Init WINSOCK
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0) {
        std::cerr << "MMDAQ: Failed on WSAStartup" << std::endl;
    }
}

MicroManagerDAQProxy::~MicroManagerDAQProxy()
{
    WSACleanup();
}

//---------------------------------------------------------------------------

MicroManagerDAQ::MicroManagerDAQ(size_t devID, DAQProxy *proxy) :
    DAQ(devID, proxy)
{
    inChnNo= params()->inChn.size();
    outChnNo = 0;
    outGainNo = 0;

    inIdx= new short int[inChnNo];
    inGainFac= new double[inChnNo];
    inGainNo= 1;
    inGainText= new char*[inGainNo];
    inGainText[0]= new char[80];
    strcpy(inGainText[0], "n/a");
    inLow = QVector<double>(inGainNo);
    inLow[0] = -1e10;
    inHigh = QVector<double>(inGainNo);
    inHigh[0] = 1e10;
}

MicroManagerDAQ::~MicroManagerDAQ()
{
    disconnect();
    delete[] inIdx;
    delete[] inGainFac;
    delete[] inGainText[0];
    delete[] inGainText;
}

bool MicroManagerDAQ::connect()
{
    if ( connected )
        return true;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "MMDAQ: Failed to create socket" << std::endl;
        return false;
    }

    // Get host info
    HOSTENT* host = gethostbyname("localhost");
    if (host == nullptr) {
        closesocket(sock);
        std::cerr << "MMDAQ: Failed to gethostbyname" << std::endl;
        return false;
    }

    // Define server info
    SOCKADDR_IN sin;
    ZeroMemory(&sin, sizeof(sin));
    sin.sin_port = htons(6666);
    sin.sin_family = AF_INET;
    memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

    // Connect to server
    if (::connect(sock, (const sockaddr*)&sin, sizeof(sin)) != 0) {
        closesocket(sock);
        std::cerr << "MMDAQ: Failed to connect" << std::endl;
        return false;
    }

    // Send greeting to server
    const char szMsg[] = "hello server\r\n";
    if (!send(sock, szMsg, strlen(szMsg), 0)) {
        closesocket(sock);
        std::cerr << "MMDAQ: Failed to send" << std::endl;
        return false;
    }

    // Receive greeting
    char szTemp[4096];
    if ( recv(sock, szTemp, 4096, 0) < 1 || strcmp(szTemp, "hello client") ) {
        closesocket(sock);
        std::cerr << "MMDAQ: Failed to receive (appropriate) greeting" << std::endl;
        return false;
    }

    connected = true;
    return true;
}

void MicroManagerDAQ::disconnect()
{
    if ( connected ) {
        connected = false;
        closesocket(sock);
    }
}

void MicroManagerDAQ::start()
{
    if ( !connected )
        connect();
}

bool MicroManagerDAQ::initialize_board(QString &name)
{
    return true;
}

void MicroManagerDAQ::generate_scan_list(short int chnNo, QVector<short> Chns)
{
    if ( !connected ) {
        if ( !connect() ) {
            return;
        }
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

void MicroManagerDAQ::get_scan(bool)
{
    if ( !connected ) {
        if ( !connect() ) {
            return;
        }
    }

    // Check for existing messages
    FD_SET readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    timeval t;
    t.tv_sec = 0;
    t.tv_usec = 0;
    if ( select(0, &readfds, 0, 0, &t) < 1 )
        // Nothing to read, bail.
        return;

    // Read everything
    char szTemp[4096];
    int bytes_received = recv(sock, szTemp, 4096, 0);
    if ( bytes_received < 1 )
        return;

    // Process line by line
    QString message(szTemp);
    QStringList lines(message.split('\n', Qt::SkipEmptyParts));
    for ( auto line : lines ) {
        if ( !line.startsWith("!!!S") || !line.endsWith("E!!!") )
            continue;
        QStringList values = line.split('\t');
        int roi = values[0].toInt();
        // values[1] is a timestamp, ignore.
        double v = values[2].toDouble();

        if ( roi < actInChnNo )
            in[inIdx[roi]].V = inGainFac[roi]*v;
    }
}

void MicroManagerDAQ::get_single_scan(inChannel *)
{
    // Only called for sample&hold, ignore
}


void MicroManagerDAQ::generate_analog_out_list(short int chnNo, QVector<short int> Chns)
{
    // No output channels, ignore
}

void MicroManagerDAQ::write_analog_out(bool)
{
    // No output, ignore
}

void MicroManagerDAQ::reset_board()
{
    disconnect();
}
