#include "SimulDaq.h"
#include "limits.h"


SimulDAQ::SimulDAQ()
{
  inChnNo= SDAQp.inChnNo;
  inIdx= new short int[inChnNo];
  inGainFac= new double[inChnNo];
  outChnNo= SDAQp.outChnNo;
  outIdx= new short int[outChnNo];
  outGainFac= new double[outChnNo];
  inGainNo= 1;
  inGainText= new char*[inGainNo];
  inGainText[0]= "n/a";
  inLow = QVector<double>(inChnNo);
  inLow[0] = -1.28;
  inHigh = QVector<double>(inChnNo);
  inHigh[0] = 1.28;
  outGainNo= 1;
  outGainText= new char*[outGainNo];
  outGainText[0]= "n/a";
  outLow = QVector<double>(outChnNo);
  outLow[0] = -10.24;
  outHigh = QVector<double>(outChnNo);
  outHigh[0] = +10.24;
  QueryPerformanceFrequency(&intClock_frequency);
  clock_frequency= (double) intClock_frequency.LowPart;
  clock_cycle= ((double) UINT_MAX + 1.0)/clock_frequency;
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
  //  delete[] inGainText[0];
  delete[] inGainText;
  //  delete[] outGainText[0];
  delete[] outGainText;
  delete[] inq;
  delete[] outq;
  delete[] inIter;
}

double SimulDAQ::get_RTC()
{
  static LARGE_INTEGER inT;
  static double dt, lastT;
  
  QueryPerformanceCounter(&inT);
  lastT= sysT;
  sysT= ((double) inT.LowPart)/clock_frequency;
  dt= sysT-lastT;
  if (dt < 0.0) dt+= clock_cycle;
  t+= dt;
 
  return dt;
}

void SimulDAQ::reset_RTC() {
  static LARGE_INTEGER inT;
   
  QueryPerformanceCounter(&inT);
  sysT= ((double) inT.LowPart)/clock_frequency;
  t= 0.0;
  lastWrite= 0.0;
  rewind();
}

bool SimulDAQ::initialize_board(QString &name)
{
  bool success= true;
  double t, data;
  QString lb;
  
  name= QString("SimulDAQ files ");

  intq.clear();
//  for (int i= 0; i < inChnNo; i++) inq[i].clear();
//  delete[] inq;
//  delete[] inIter;
  is.open(SDAQp.inFileName.toAscii());
  is >> t;
//  inChnNo= SDAQp.inChnNo;
//  inq= new QList<double>[inChnNo];
//  inIter= new QList<double>::const_iterator[inChnNo];
  while (is.good()) {
    intq.append(t);
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
    dataT= *(intq.end()-1)*SDAQp.inTFac;
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
  os.open(SDAQp.outFileName.toAscii());
  if (!os.good()) {
    name=QString("SimulDAQ output files ");
    success= false;
  }
//  delete[] outIdx;
//  outIdx= new short int[outChnNo];
//  delete[] outGainFac;
//  outGainFac= new double[outChnNo];
  reset_RTC();
  
  initialized= success;
  
  return success;
}

void SimulDAQ::generate_scan_list(short int chnNo, short int *Chns)
{
  short int i;
  actInChnNo= chnNo;  
  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= inChnp[inIdx[i]].gainFac;  // read V ... users take care of other units
  }
}

void SimulDAQ::get_scan(inChannel *in)
{
  short int i;
  short int idx;
  static double V;

  while (t > inT) {
    intIter++;
    for (i= 0; i < inChnNo; i++) {
      inIter[i]++;
    }
    if (intIter == intq.end()) {
      t-= dataT;
      lastWrite-= dataT;
      rewind();
    }
    inT= *intIter*SDAQp.inTFac;
  }
  for (i= 0; i < inChnNo; i++) {
    idx= 0;
    V= *inIter[i];
    if ((idx < actInChnNo) && (i == inIdx[idx])) {
      in[i].V= V*inGainFac[idx];
      idx++;
    }
  }
}

void SimulDAQ::generate_analog_out_list(short int chnNo, short int *Chns) 
{
  short int i;
  actOutChnNo= chnNo;
  for (i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= outChnp[outIdx[i]].gainFac*1.0e9;  // write nA
  }
  if (os.is_open()) os.close();
  os.open(SDAQp.outFileName.toAscii());
  lastWrite= 0.0;
}

void SimulDAQ::write_analog_out(outChannel *out)
{
//  short int idx;
  double dt;
  
  dt= get_RTC();
  if (t > lastWrite+SDAQp.outDt) {
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
    inT= *intIter*SDAQp.inTFac;
  }
}
