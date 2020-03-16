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

#include "SimulDAQ.h"
#include "limits.h"
#include <QFileInfo>
#include <QDir>
#include "AP.h"
#include "SimulDAQDlg.h"
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

/// Construct a single self-registering proxy
static SimulDAQProxy *prox = SimulDAQProxy::get();
std::vector<SDAQData> SimulDAQProxy::p;
DAQ *SimulDAQProxy::createDAQ(size_t devID) { return new SimulDAQ(devID, prox); }
DAQDlg *SimulDAQProxy::createDialog(size_t devID, QWidget *parent) { return new SimulDAQDlg(devID, prox, parent); }

SimulDAQProxy::SimulDAQProxy() :
    regAP {
        addAP("SDAQp[#].active", &p, &SDAQData::active),
        addAP("SDAQp[#].label", &p, &SDAQData::label),
        addAP("SDAQp[#].inFileName", &p, &SDAQData::inFileName),
        addAP("SDAQp[#].outFileName", &p, &SDAQData::outFileName),
        addAP("SDAQp[#].inTFac", &p, &SDAQData::inTFac),
        addAP("SDAQp[#].outDt", &p, &SDAQData::outDt),
        addAP("SDAQp[#].rewindAfterSettling", &p, &SDAQData::rewindAfterSettling),
        addAP("SDAQp[#].inChn[#].active", &p, &SDAQData::inChn, &inChnData::active),
        addAP("SDAQp[#].inChn[#].gain", &p, &SDAQData::inChn, &inChnData::gain),
        addAP("SDAQp[#].inChn[#].gainFac", &p, &SDAQData::inChn, &inChnData::gainFac),
        addAP("SDAQp[#].inChn[#].spkDetect", &p, &SDAQData::inChn, &inChnData::spkDetect),
        addAP("SDAQp[#].inChn[#].spkDetectThresh", &p, &SDAQData::inChn, &inChnData::spkDetectThresh),
        addAP("SDAQp[#].inChn[#].bias", &p, &SDAQData::inChn, &inChnData::bias),
        addAP("SDAQp[#].inChn[#].chnlSaving", &p, &SDAQData::inChn, &inChnData::chnlSaving),
        addAP("SDAQp[#].outChn[#].active", &p, &SDAQData::outChn, &outChnData::active),
        addAP("SDAQp[#].outChn[#].gain", &p, &SDAQData::outChn, &outChnData::gain),
        addAP("SDAQp[#].outChn[#].gainFac", &p, &SDAQData::outChn, &outChnData::gainFac),
        addAP("SDAQp[#].outChn[#].bias", &p, &SDAQData::outChn, &outChnData::bias),
        addAP("SDAQp[#].outChn[#].chnlSaving", &p, &SDAQData::outChn, &outChnData::chnlSaving)
    }
{
    DeviceManager::RegisterDAQ(daqClass(), this);
}

//---------------------------------------------------------------------------

SimulDAQ::SimulDAQ(size_t devID, DAQProxy *proxy) :
    DAQ(devID, proxy),
    tOff(0.0)
{
  inChnNo= params()->inChn.size();
  inIdx= new short int[inChnNo];
  inGainFac= new double[inChnNo];
  outChnNo= params()->outChn.size();
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
  inq= new QList<double>[inChnNo];
  outq= new QList<double>[outChnNo];
  inIter= new QList<double>::const_iterator[inChnNo];
}

SimulDAQ::~SimulDAQ()
{
  delete[] inIdx;
  delete[] inGainFac;
  delete[] outIdx;
  delete[] outGainFac;
  delete[] inGainText[0];
  delete[] inGainText;
  delete[] outGainText[0];
  delete[] outGainText;
  delete[] inq;
  delete[] outq;
  delete[] inIter;
}

void SimulDAQ::start() {
  tOff = 0.0;
  lastWrite= 0.0;
  rewind();
}

bool SimulDAQ::initialize_board(QString &name)
{
  bool success= true;
  double t, t0, data;
  QString lb;
  
  name= QString("SimulDAQ files ");

  intq.clear();
  is.open(SimulDAQProxy::p[devID].inFileName.toLatin1());
  is >> t;
  t0= t;
  while (is.good()) {
    intq.append((t-t0) * SimulDAQProxy::p[devID].inTFac);
    for (int i= 0; i < inChnNo; i++) {
      is >> data;
      inq[i].append(data);
    }
    is >> t;
  }
  if (intq.size() == 0) {
    name=QString("SimulDAQ input files ");
    success= false;
  }
  else {
    lb.setNum(intq.size());
    name=QString("SimulDAQ input ")+lb+QString(" points ");
    intIter= intq.begin();
    inT= *intIter;
    dataT= *(intq.end()-1);
    for (int i= 0; i < inChnNo; i++) inIter[i]= inq[i].begin();
  }       
  is.close();
  is.clear();

  outtq.clear();
  for (int i= 0; i < outChnNo; i++) outq[i].clear();
  if (os.is_open()) os.close();

  qt_ntfs_permission_lookup++;
  QFileInfo fInfo(SimulDAQProxy::p[devID].outFileName.toLatin1());
  if ( !fInfo.dir().isReadable() || (fInfo.exists() && !fInfo.isWritable()) ) {
    name=QString("SimulDAQ output files ");
    success= false;
  }
  qt_ntfs_permission_lookup--;

  start();
  
  initialized= success;
  
  return success;
}

void SimulDAQ::generate_scan_list(short int chnNo, QVector<short> Chns)
{
  short int i;
  actInChnNo= chnNo;

  ChannelIndex dex(prox, devID, 0, true);

  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= params()->inChn[inIdx[i]].gainFac;  // read V ... users take care of other units
    dex.chanID = inIdx[i];
    inChnLabels[inIdx[i]] = dex.toString();
  }
}

void SimulDAQ::get_scan(bool)
{
  short int i;
  short int idx;
  static double V;

  //os << t << endl;
  if (t >= inT + tOff) {
    intIter++;
    for (i= 0; i < inChnNo; i++) {
      inIter[i]++;
    }
    if (intIter == intq.end()) {
      tOff+= dataT;
      rewind();
    }
    inT= *intIter;

    idx= 0;
    for (i= 0; i < inChnNo; i++) {
      V= *inIter[i];
      //os << idx << " " << actInChnNo << " " << i << " " << inIdx[idx] << " " << V << endl;
      if ((idx < actInChnNo) && (i == inIdx[idx])) {
        in[i].V= V*inGainFac[idx];
        idx++;
      }
    }
  }
}

void SimulDAQ::get_single_scan(inChannel *in)
{
   short int i;
   short int idx;
   static double V;

   //os << t << endl;
   if (t >= inT + tOff) {
      intIter++;
      for (i= 0; i < inChnNo; i++) {
        inIter[i]++;
      }
      if (intIter == intq.end()) {
        tOff+= dataT;
        rewind();
      }
      inT= *intIter;

      idx= 0;
      for (i= 0; i < inChnNo; i++) {
        V= *inIter[i];
        if ((idx < actInChnNo) && (i == inIdx[idx])) {
          if (&(this->in[i]) == in) {
            in->V= V*inGainFac[idx];
          }
          idx++;
        }
     }
   }
 }


void SimulDAQ::generate_analog_out_list(short int chnNo, QVector<short int> Chns)
{
  ChannelIndex dex(prox, devID, 0, false);

  short int i;
  actOutChnNo= chnNo;
  for (i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= params()->outChn[outIdx[i]].gainFac;  // write A
    dex.chanID = outIdx[i];
    outChnLabels[outIdx[i]] = dex.toString();
  }
  if (os.is_open()) os.close();
  os.open(SimulDAQProxy::p[devID].outFileName.toLatin1());
  lastWrite= 0.0;
}

void SimulDAQ::write_analog_out(bool settling)
{
  if ( settling ) // Can't write during settling without messing up time stamps... so don't write.
    return;
  if (t > lastWrite + SimulDAQProxy::p[devID].outDt) {
    lastWrite= t;
    outtq.append(t);
    for (int i= 0; i < outChnNo; i++) {
      outq[i].append(out[outIdx[i]].I * outGainFac[i]);
    }
  }
}

void SimulDAQ::settling_complete()
{
    if ( SimulDAQProxy::p[devID].rewindAfterSettling ) {
        rewind();
        tOff = t;
    }
}


void SimulDAQ::reset_board()
{
  flush_analog_out();
  rewind();
}


void SimulDAQ::flush_analog_out() 
{
  while (outtq.size() > 0) {
    os << outtq.front() << " ";
    outtq.pop_front();
    for (int i= 0; i < outChnNo; i++) {
      os << outq[i].front() << " ";
      outq[i].pop_front();
    }
    os << endl;
  }
}


void SimulDAQ::rewind() 
{
  if (intq.size() > 0) {
    intIter= intq.begin();
    for (int i= 0; i < inChnNo; i++) {
      inIter[i]= inq[i].begin();
    }
    intIter++;   // the t iterator needs to be one step ahead so that channel values of the current step are applied
    inT= *intIter;
  }
}
