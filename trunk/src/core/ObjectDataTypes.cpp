
#include "ObjectDataTypes.h"

ostream &operator<<(ostream &os, SDAQData &p) 
{
  string buf;
  buf= p.inFileName.toStdString();
  os << buf << " ";
  buf= p.outFileName.toStdString();
  os << buf << " ";
  os << p.inChnNo << " ";
  os << p.outChnNo << " ";
  os << p.inTFac << " ";
  os << p.outDt << " ";
  return os;
} 

istream &operator>>(istream &is, SDAQData &p) 
{
  char cBuf[80];
  is >> cBuf;
  p.inFileName= QString(cBuf);;
  is >> cBuf;
  p.outFileName= QString(cBuf);
  is >> p.inChnNo;
  is >> p.outChnNo;
  is >> p.inTFac;
  is >> p.outDt;
  return is;
} 

SDAQData &SDAQData::operator=(SDAQData old)
{
  inFileName= old.inFileName;
  outFileName= old.outFileName;
  inChnNo= old.inChnNo;
  outChnNo= old.outChnNo;
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
