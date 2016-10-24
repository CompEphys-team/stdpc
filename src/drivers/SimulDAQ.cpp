#include "SimulDAQ.h"
#include "limits.h"
#include <QFileInfo>
#include <QDir>
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

SimulDAQ::SimulDAQ(int devID) :
    DAQ(devID),
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
  is.open(SDAQp[devID].inFileName.toLatin1());
  is >> t;
  t0= t;
  while (is.good()) {
    intq.append((t-t0) * SDAQp[devID].inTFac);
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
  QFileInfo fInfo(SDAQp[devID].outFileName.toLatin1());
  if ( !fInfo.dir().isReadable() || (fInfo.exists() && !fInfo.isWritable()) ) {
    name=QString("SimulDAQ output files ");
    success= false;
  }
  qt_ntfs_permission_lookup--;

  start();
  
  initialized= success;
  
  return success;
}

void SimulDAQ::generate_scan_list(short int chnNo, short int *Chns)
{
  short int i;
  actInChnNo= chnNo;

  ChannelIndex dex(DAQClass::Simul, devID, true);

  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= params()->inChn[inIdx[i]].gainFac;  // read V ... users take care of other units
    dex.chanID = inIdx[i];
    inChnLabels[inIdx[i]] = dex.toString();
  }
}

void SimulDAQ::get_scan()
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


void SimulDAQ::generate_analog_out_list(short int chnNo, short int *Chns) 
{
  ChannelIndex dex(DAQClass::Simul, devID, false);

  short int i;
  actOutChnNo= chnNo;
  for (i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= params()->outChn[outIdx[i]].gainFac*1.0e9;  // write nA
    dex.chanID = outIdx[i];
    outChnLabels[outIdx[i]] = dex.toString();
  }
  if (os.is_open()) os.close();
  os.open(SDAQp[devID].outFileName.toLatin1());
  lastWrite= 0.0;
}

void SimulDAQ::write_analog_out()
{
//  short int idx;
//  double dt;
  
//  dt= get_RTC();
  if (t > lastWrite + SDAQp[devID].outDt) {
    lastWrite= t;
    outtq.append(t);
    for (int i= 0; i < outChnNo; i++) {
      outq[i].append(out[outIdx[i]].I);
    }
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
    inT= *intIter;
  }
}


QString SimulDAQ::prefix()
{
    return QString("SimulDAQ_%1").arg(devID);
}
