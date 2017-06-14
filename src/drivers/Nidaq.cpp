#include "AP.h"
#include "Nidaq.h"
#include "limits.h"
#include <sstream>
#include <QMessageBox>
#include <QString>
#include <cmath>
#include "NIDAQDlg.h"

// NIDAQmx device driver

/// Construct a single self-registering proxy
static NIDAQProxy prox;
std::vector<NIDAQData> NIDAQProxy::p;
DAQ *NIDAQProxy::createDAQ(size_t devID) { return new NIDAQ(devID, &prox); }
DAQDlg *NIDAQProxy::createDialog(size_t devID, QWidget *parent) { return new NIDAQDlg(devID, &prox, parent); }

NIDAQProxy::NIDAQProxy() :
    regAP {
        addAP("NIDAQp[#].active", &p, &NIDAQData::active),
        addAP("NIDAQp[#].deviceName", &p, &NIDAQData::deviceName),
        addAP("NIDAQp[#].inChn[#].active", &p, &NIDAQData::inChn, &inChnData::active),
        addAP("NIDAQp[#].inChn[#].gain", &p, &NIDAQData::inChn, &inChnData::gain),
        addAP("NIDAQp[#].inChn[#].gainFac", &p, &NIDAQData::inChn, &inChnData::gainFac),
        addAP("NIDAQp[#].inChn[#].spkDetect", &p, &NIDAQData::inChn, &inChnData::spkDetect),
        addAP("NIDAQp[#].inChn[#].spkDetectThresh", &p, &NIDAQData::inChn, &inChnData::spkDetectThresh),
        addAP("NIDAQp[#].inChn[#].bias", &p, &NIDAQData::inChn, &inChnData::bias),
        addAP("NIDAQp[#].inChn[#].chnlSaving", &p, &NIDAQData::inChn, &inChnData::chnlSaving),
        addAP("NIDAQp[#].outChn[#].active", &p, &NIDAQData::outChn, &outChnData::active),
        addAP("NIDAQp[#].outChn[#].gain", &p, &NIDAQData::outChn, &outChnData::gain),
        addAP("NIDAQp[#].outChn[#].gainFac", &p, &NIDAQData::outChn, &outChnData::gainFac),
        addAP("NIDAQp[#].outChn[#].bias", &p, &NIDAQData::outChn, &outChnData::bias),
        addAP("NIDAQp[#].outChn[#].chnlSaving", &p, &NIDAQData::outChn, &outChnData::chnlSaving),

        addAP("NIDAQp[#].inChn[#].calib.copyChnOn", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::copyChnOn),
        addAP("NIDAQp[#].inChn[#].calib.copyChn", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::copyChn),
        addAP("NIDAQp[#].inChn[#].calib.samplingRate", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::samplingRate),
        addAP("NIDAQp[#].inChn[#].calib.outputChannelNumber", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::outputChannelNumber),
        addAP("NIDAQp[#].inChn[#].calib.iMaxElec", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMaxElec),
        addAP("NIDAQp[#].inChn[#].calib.iMinElec", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMinElec),
        addAP("NIDAQp[#].inChn[#].calib.numberOfLevels", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::numberOfLevels),
        addAP("NIDAQp[#].inChn[#].calib.injLenPerLevel", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injLenPerLevel),
        addAP("NIDAQp[#].inChn[#].calib.iMembStep", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::iMembStep),
        addAP("NIDAQp[#].inChn[#].calib.numberOfRepeats", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::numberOfRepeats),
        addAP("NIDAQp[#].inChn[#].calib.injLenPerRepeat", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injLenPerRepeat),
        addAP("NIDAQp[#].inChn[#].calib.hyperpolCurr", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::hyperpolCurr),
        addAP("NIDAQp[#].inChn[#].calib.injCalAmp", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injCalAmp),
        addAP("NIDAQp[#].inChn[#].calib.injCalLen", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::injCalLen),
        addAP("NIDAQp[#].inChn[#].calib.fullKernelLen", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::fullKernelLen),
        addAP("NIDAQp[#].inChn[#].calib.electrodeKernelLen", &p, &NIDAQData::inChn, &inChnData::calib, &elecCalibParams::electrodeKernelLen)
    }
{
    DeviceManager::RegisterDAQ(daqClass(), this);
}

//---------------------------------------------------------------------------

NIDAQ::NIDAQ(size_t devID, DAQProxy *proxy) :
    DAQ(devID, proxy)
{
  char data[1024];
  float64 fdata[128];
  istringstream *istr;
  stringstream ostr;
  char cbuf;
  
  devName= new char[80];
  strcpy(devName, (const char *) NIDAQProxy::p[devID].deviceName.toLatin1());
  
  DevicePresent= (DAQmxGetDevAIPhysicalChans (devName, data, 1024) == 0);
  //if (!DevicePresent)
//  QMessageBox::warning(NULL, QString("My Application"),
//                QString(data),
//                QMessageBox::Ok);
    
  if (DevicePresent) {
    istr=new istringstream(data);
    int k= 0;
    while (istr->good()) {
      iChnNm[k]= new char[80];
      istr->getline(iChnNm[k++], 80, ',');
      istr->get(cbuf);
    }
    delete istr;
    inChnNo= k;
    inIdx= new short int[inChnNo];
    inGainFac= new double[inChnNo];
  
    for (int i= 0; i < 128; i++) fdata[i]= 0.0;
    DAQmxGetDevAIVoltageRngs(devName, fdata, 128);
    k= 0;
    while ((abs(fdata[k]) > 1e-10) || (abs(fdata[k+1]) > 1e-10)) k+=2;
    inGainNo= k/2;
    inGainText= new char*[inGainNo];
    inLow= QVector<double>(inGainNo);
    inHigh= QVector<double>(inGainNo);
    for (int i= 0; i < inGainNo; i++) {
      ostr.clear();
      ostr << fdata[2*i] << " to " << fdata[2*i+1] << " V" << ends;
      inGainText[i]= new char[80];
      ostr.getline(inGainText[i], 80);
      inLow[i]= fdata[2*i];
      inHigh[i]= fdata[2*i+1];
    }
  
    DAQmxGetDevAOPhysicalChans (devName, data, 1024);
    istr=new istringstream(data);
    k= 0;
    while (istr->good()) {
      oChnNm[k]= new char[80];
      istr->getline(oChnNm[k++], 80, ',');
      istr->get(cbuf);
    }
    delete istr;
    outChnNo= k;
    outIdx= new short int[outChnNo];
    outGainFac= new double[outChnNo];

    for (int i= 0; i < 128; i++) fdata[i]= 0.0;
    DAQmxGetDevAOVoltageRngs(devName, fdata, 128);
    k= 0;
    while ((fdata[k] != 0.0) || (fdata[k+1] != 0.0)) k+=2;
    outGainNo= k/2;
    outGainText= new char*[outGainNo];
    outLow= QVector<double>(outGainNo);
    outHigh= QVector<double>(outGainNo);
    for (int i= 0; i < outGainNo; i++) {
      ostr.clear();
      ostr << fdata[2*i] << " to " << fdata[2*i+1] << " V" << ends;
      outGainText[i]= new char[80];
      ostr.getline(outGainText[i], 80);
      outLow[i]= fdata[2*i];
      outHigh[i]= fdata[2*i+1];
    }

    DAQmxGetDevDILines(devName, data, 1024);
    istr= new istringstream(data);
    k= 0;
    while (istr->good()) {
      diChnNm[k]= new char[80];
      istr->getline(diChnNm[k++], 80, ',');
      istr->get(cbuf);
    }
    delete istr;
    digInChnNo= k;
  }
  else {
    inChnNo= 0;
    inGainNo= 0;
    outChnNo= 0;
    outGainNo= 0;
    digInChnNo= 0;
  }

  inTaskActive= 0;
  outTaskActive= 0;  
  digInTaskActive= 0;
  
  
//   QMessageBox::warning(NULL, QString("My Application"),
//                QString("Finished scanning ouput ranges, exiting NIDAQ constructor"),
//               QMessageBox::Close); 

//  init();
}

NIDAQ::~NIDAQ()
{
  if (DevicePresent) {
    for (int i= 0; i < inChnNo; i++) {
      delete[] iChnNm[i];
    }
    for (int i= 0; i < inGainNo; i++) {
      delete[] inGainText[i];
    }
    delete[] inGainText;
    for (int i= 0; i < outChnNo; i++) {
      delete[] oChnNm[i];
    }
    for (int i= 0; i < outGainNo; i++) {
      delete[] outGainText[i];
    }
    delete[] outGainText;
    for (int i= 0; i < digInChnNo; i++) {
      delete[] diChnNm[i];
    }
    //delete[] inChnGain;
    delete[] inIdx;
    delete[] inGainFac;
    delete[] outIdx;
    delete[] outGainFac;
    DAQmxResetDevice(devName);
    delete[] devName;
  }
}

void NIDAQ::init()
{
}

//---------------------------------------------------------------------------
bool NIDAQ::initialize_board(QString &name)
{
  char cname[1024];
  bool success= false;

  if (DevicePresent) {
    success= (DAQmxGetDevProductType(devName, cname, 1024) == 0);
    if (success) name= QString(cname);
    else name= QString(devName);
    DAQmxResetDevice(devName);
    initialized= success;
  } else name = QString(devName);
  return success;
}

void NIDAQ::start()
{
  // this is called when the dynamic clamp starts ... start tasks
  if (DevicePresent) {
    DAQmxStartTask(inTask);
    DAQmxStartTask(outTask);
  }
}

//---------------------------------------------------------------------------
void NIDAQ::digital_out(unsigned char)
{
// not supported for the NIDAQ driver
}


//---------------------------------------------------------------------------
void NIDAQ::generate_scan_list(short int chnNo, QVector<short int> Chns)
{
  if (DevicePresent) {
    actInChnNo= chnNo;
    if (inTaskActive != 0) {
       DAQmxClearTask(inTask);
       delete[] inBuf;
    }
    inBuf= new float64[chnNo];
    DAQmxCreateTask ("", &inTask);

    DAQData *p = params();
    ChannelIndex dex(&prox, devID, 0, true);
  
    for (int i= 0; i < chnNo; i++) {
      inIdx[i]= Chns[i];
      DAQmxCreateAIVoltageChan (inTask, iChnNm[inIdx[i]], "", DAQmx_Val_RSE, inLow[p->inChn[inIdx[i]].gain],
                                inHigh[p->inChn[inIdx[i]].gain], DAQmx_Val_Volts, "");
      inGainFac[i]= p->inChn[inIdx[i]].gainFac;
      dex.chanID = inIdx[i];
      inChnLabels[inIdx[i]] = dex.toString();
    }
    DAQmxSetSampTimingType(inTask, DAQmx_Val_OnDemand);
    DAQmxSetReadOverWrite(inTask, DAQmx_Val_OverwriteUnreadSamps);
//  DAQmxSetBufInputBufSize(inTask, actInChnNo);
    inTaskActive= 1;
  }
}

//---------------------------------------------------------------------------
void NIDAQ::get_scan()
{
  // assume device is present if this is called (responsibility of calling code!)
  static int i;
  static int32 spr;
//  QString temp;

  DAQmxReadAnalogF64(inTask, 1, 1.0e-4, DAQmx_Val_GroupByScanNumber, inBuf, actInChnNo, &spr, NULL);
//  temp.setNum(inBuf[0]);
//  QMessageBox::warning(NULL, QString("My Application"),
//                temp,
//                QMessageBox::Ok);
  for (i= 0; i < actInChnNo; i++) in[inIdx[i]].V= inGainFac[i]*inBuf[i];
}

//---------------------------------------------------------------------------
void NIDAQ::get_single_scan(inChannel *in)
{
  // assume device is present if this is called (responsibility of calling code!)
  static int i;
  static int32 spr;
//  QString temp;

  DAQmxReadAnalogF64(inTask, 1, 1.0e-4, DAQmx_Val_GroupByScanNumber, inBuf, actInChnNo, &spr, NULL);
//  temp.setNum(inBuf[0]);
//  QMessageBox::warning(NULL, QString("My Application"),
//                temp,
//                QMessageBox::Ok);
  for (i= 0; i < actInChnNo; i++) {
      if (&(this->in[inIdx[i]]) == in) {
          in->V= inGainFac[i]*inBuf[i];
      }
  }
}


//---------------------------------------------------------------------------
void NIDAQ::generate_analog_out_list(short int chnNo, QVector<short int> Chns)
{
  if (DevicePresent) {
    actOutChnNo= chnNo;
    if (outTaskActive != 0) {
      DAQmxClearTask(outTask);
      delete[] outBuf;
    }
    outBuf= new float64[actOutChnNo];
    DAQmxCreateTask("", &outTask);

    DAQData *p = params();
    ChannelIndex dex(&prox, devID, 0, false);
   
    for (int i= 0; i < chnNo; i++) {
      outIdx[i]= Chns[i];
      DAQmxCreateAOVoltageChan (outTask, oChnNm[outIdx[i]], "", outLow[p->outChn[outIdx[i]].gain],
                                outHigh[p->outChn[outIdx[i]].gain], DAQmx_Val_Volts, "");
      outGainFac[i]= p->outChn[outIdx[i]].gainFac*1e9;
      dex.chanID = outIdx[i];
      outChnLabels[outIdx[i]] = dex.toString();
    }
    DAQmxSetSampTimingType(outTask, DAQmx_Val_OnDemand);
    //DAQmxSetBufOutputBufSize(outTask, actOutChnNo*10);
    outTaskActive= 1;
  }
}


//---------------------------------------------------------------------------
void NIDAQ::write_analog_out()
{
  static int i;
  static int32 spw;
  for (i= 0; i < actOutChnNo; i++) outBuf[i]= out[outIdx[i]].I*outGainFac[i];
  DAQmxWriteAnalogF64(outTask, 1, 1, 1.0e-4, DAQmx_Val_GroupByScanNumber, outBuf, &spw, NULL);
  //outBuf[0]= sin(t);
  //DAQmxWriteAnalogF64(outTask, 1, 1, 1.0e-4, DAQmx_Val_GroupByScanNumber, outBuf, &spw, NULL);
}


void NIDAQ::reset_board()
{
 static int32 spw;
  // stop tasks first
 if (DevicePresent) {
   if (outTaskActive) {
      for (int i= 0; i < actOutChnNo; i++) outBuf[i]= 0.0;
      DAQmxWriteAnalogF64(outTask, 1, 1, 1.0e-4, DAQmx_Val_GroupByScanNumber, outBuf, &spw, NULL);
      DAQmxStopTask(outTask);
      DAQmxClearTask(outTask);
      delete[] outBuf;
      outTaskActive= 0;
    }
    if (inTaskActive) {
      DAQmxStopTask(inTask);
      DAQmxClearTask(inTask);
      delete[] inBuf;
      inTaskActive= 0;
    }
    if ( digInTaskActive ) {
        DAQmxStopTask(digInTask);
        DAQmxClearTask(digInTask);
        digInTaskActive= 0;
    }
    DAQmxResetDevice(devName);
 }
}

void NIDAQ::armTrigger(ChannelIndex trigChn)
{
    if ( DevicePresent ) {
        if (digInTaskActive != 0) {
            DAQmxClearTask(digInTask);
        }
        DAQmxCreateTask ("", &digInTask);
        DAQmxCreateDIChan (digInTask, diChnNm[trigChn.chanID], "", DAQmx_Val_ChanPerLine);
        DAQmxSetSampTimingType(digInTask, DAQmx_Val_OnDemand);
        DAQmxSetReadOverWrite(digInTask, DAQmx_Val_OverwriteUnreadSamps);
        digInTaskActive= 1;
        DAQmxStartTask(digInTask);
    }
}

bool NIDAQ::triggerFired()
{
    if ( DevicePresent && digInTaskActive ) {
        uInt32 buf;
        DAQmxReadDigitalScalarU32(digInTask, 1e-3, &buf, NULL);
        return buf;
    }
    return true;
}
