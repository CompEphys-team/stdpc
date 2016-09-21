#include "SimulDAQ.h"
#include "limits.h"


SimulDAQ::SimulDAQ(SDAQData *p, int devID) :
    DAQ(p, devID),
    tOff(0.0)
{
  inChnNo= p->inChn.size();
  inIdx= new short int[inChnNo];
  inGainFac= new double[inChnNo];
  outChnNo= p->outChn.size();
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
//  for (int i= 0; i < inChnNo; i++) inq[i].clear();
//  delete[] inq;
//  delete[] inIter;
  is.open(static_cast<SDAQData*>(p)->inFileName.toLatin1());
  is >> t;
  t0= t;
//  inChnNo= SDAQp.inChnNo;
//  inq= new QList<double>[inChnNo];
//  inIter= new QList<double>::const_iterator[inChnNo];
  while (is.good()) {
    intq.append((t-t0) * static_cast<SDAQData*>(p)->inTFac);
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
//  inChnNo= SDAQp.inChnNo;
//  delete[] inIdx;
//  inIdx= new short int[inChnNo];
//  delete[] inGainFac;
//  inGainFac= new double[inChnNo];
  outtq.clear();
  for (int i= 0; i < outChnNo; i++) outq[i].clear();
//  delete[] outq;

//  outChnNo= SDAQp.outChnNo;
//  outq= new QList<double>[outChnNo];
  if (os.is_open()) os.close();
  os.open(static_cast<SDAQData*>(p)->outFileName.toLatin1());
  if (!os.good()) {
    name=QString("SimulDAQ output files ");
    success= false;
  }
//  delete[] outIdx;
//  outIdx= new short int[outChnNo];
//  delete[] outGainFac;
//  outGainFac= new double[outChnNo];
  start();
  
  initialized= success;
  
  return success;
}

void SimulDAQ::generate_scan_list(short int chnNo, short int *Chns)
{
  short int i;
  actInChnNo= chnNo;

  ChannelIndex dex;
  dex.isValid = true;
  dex.isAnalog = true;
  dex.daqClass = DAQClass::NI;
  dex.devID = devID;
  dex.isInChn = true;

  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= p->inChn[inIdx[i]].gainFac;  // read V ... users take care of other units
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
  if (t+tOff >= inT) {
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
   if (t+tOff >= inT) {
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
  ChannelIndex dex;
  dex.isValid = true;
  dex.isAnalog = true;
  dex.daqClass = DAQClass::NI;
  dex.devID = devID;
  dex.isInChn = false;

  short int i;
  actOutChnNo= chnNo;
  for (i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= p->outChn[outIdx[i]].gainFac*1.0e9;  // write nA
    dex.chanID = outIdx[i];
    outChnLabels[outIdx[i]] = dex.toString();
  }
  if (os.is_open()) os.close();
  os.open(static_cast<SDAQData*>(p)->outFileName.toLatin1());
  lastWrite= 0.0;
}

void SimulDAQ::write_analog_out()
{
//  short int idx;
//  double dt;
  
//  dt= get_RTC();
  if (t+tOff > lastWrite + static_cast<SDAQData*>(p)->outDt) {
    lastWrite= t+tOff;
    outtq.append(t+tOff);
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
    return QString("sim");
}
