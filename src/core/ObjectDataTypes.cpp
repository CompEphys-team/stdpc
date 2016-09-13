#include "ObjectDataTypes.h"

ostream &operator<<(ostream &os, SDAQData &p) 
{
  string buf;
  buf= p.inFileName.toStdString();
  os << buf << " ";
  buf= p.outFileName.toStdString();
  os << buf << " ";
  os << p.inChn.size() << " ";
  os << p.outChn.size() << " ";
  os << p.inTFac << " ";
  os << p.outDt << " ";
  return os;
} 

istream &operator>>(istream &is, SDAQData &p) 
{
  char cBuf[80];
  size_t inChnNo, outChnNo;
  is >> cBuf;
  p.inFileName= QString(cBuf);;
  is >> cBuf;
  p.outFileName= QString(cBuf);
  is >> inChnNo; p.inChn.resize(inChnNo);
  is >> outChnNo; p.outChn.resize(outChnNo);
  is >> p.inTFac;
  is >> p.outDt;
  return is;
} 

SDAQData &SDAQData::operator=(SDAQData old)
{
  inFileName= old.inFileName;
  outFileName= old.outFileName;
  inChn.resize(old.inChn.size());
  outChn.resize(old.outChn.size());
  inTFac= old.inTFac;
  outDt= old.outDt;
  return *this;
}

ostream &operator<<(ostream &os, DigiDataData &p)
{
  os << p.baseAddress << " ";
  return os;
}

istream &operator>>(istream &is, DigiDataData &p)
{
  is >> p.baseAddress;
  return is;
}

DigiDataData &DigiDataData::operator=(DigiDataData old)
{
  baseAddress= old.baseAddress;
  return *this;
}

#ifdef NATIONAL_INSTRUMENTS
ostream &operator<<(ostream &os, NIDAQData &p) 
{
  string buf;
  buf= p.deviceName.toStdString();
  os << buf << endl;
  return os;
} 

istream &operator>>(istream &is, NIDAQData &p) 
{
  char cBuf[80];
  is >> cBuf;
  p.deviceName= QString(cBuf);;
  return is;
} 
#endif
