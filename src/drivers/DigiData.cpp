#include "DigiData.h"
#include "limits.h"
#include "AP.h"

// DigiData 1200 device driver

/// Construct a single self-registering proxy
static DigiDataProxy prox;
std::vector<DigiDataData> DigiDataProxy::p;
DAQProxy *DigiData::proxy() const { return &prox; }
DAQ *DigiDataProxy::createDAQ(size_t devID) { return new DigiData(devID); }
/* NYI: DAQDlg *DigiDataProxy::createDialog(size_t devID, QWidget *parent) { return new DigiDataDlg(devID, parent); } */

DigiDataProxy::DigiDataProxy()
{
    DeviceManager::RegisterDAQ(daqClass(), this);

    addAP("DigiDatap[#].active", &DigiDataProxy::p, &DigiDataData::active);
    addAP("DigiDatap[#].baseAddress", &DigiDataProxy::p, &DigiDataData::baseAddress);
    addAP("DigiDatap[#].syncIOMask", &DigiDataProxy::p, &DigiDataData::syncIOMask);
    addAP("DigiDatap[#].inChn[#].active", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::active);
    addAP("DigiDatap[#].inChn[#].gain", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::gain);
    addAP("DigiDatap[#].inChn[#].gainFac", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::gainFac);
    addAP("DigiDatap[#].inChn[#].spkDetect", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::spkDetect);
    addAP("DigiDatap[#].inChn[#].spkDetectThresh", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::spkDetectThresh);
    addAP("DigiDatap[#].inChn[#].bias", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::bias);
    addAP("DigiDatap[#].inChn[#].chnlSaving", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::chnlSaving);
    addAP("DigiDatap[#].outChn[#].active", &DigiDataProxy::p, &DigiDataData::outChn, &outChnData::active);
    addAP("DigiDatap[#].outChn[#].gain", &DigiDataProxy::p, &DigiDataData::outChn, &outChnData::gain);
    addAP("DigiDatap[#].outChn[#].gainFac", &DigiDataProxy::p, &DigiDataData::outChn, &outChnData::gainFac);
    addAP("DigiDatap[#].outChn[#].bias", &DigiDataProxy::p, &DigiDataData::outChn, &outChnData::bias);
    addAP("DigiDatap[#].outChn[#].chnlSaving", &DigiDataProxy::p, &DigiDataData::outChn, &outChnData::chnlSaving);

    addAP("DigiDatap[#].inChn[#].calib.copyChnOn", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::copyChnOn);
    addAP("DigiDatap[#].inChn[#].calib.copyChn", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::copyChn);
    addAP("DigiDatap[#].inChn[#].calib.samplingRate", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::samplingRate);
    addAP("DigiDatap[#].inChn[#].calib.outputChannelNumber", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::outputChannelNumber);
    addAP("DigiDatap[#].inChn[#].calib.iMaxElec", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMaxElec);
    addAP("DigiDatap[#].inChn[#].calib.iMinElec", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMinElec);
    addAP("DigiDatap[#].inChn[#].calib.numberOfLevels", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::numberOfLevels);
    addAP("DigiDatap[#].inChn[#].calib.injLenPerLevel", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injLenPerLevel);
    addAP("DigiDatap[#].inChn[#].calib.iMembStep", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::iMembStep);
    addAP("DigiDatap[#].inChn[#].calib.numberOfRepeats", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::numberOfRepeats);
    addAP("DigiDatap[#].inChn[#].calib.injLenPerRepeat", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injLenPerRepeat);
    addAP("DigiDatap[#].inChn[#].calib.hyperpolCurr", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::hyperpolCurr);
    addAP("DigiDatap[#].inChn[#].calib.injCalAmp", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injCalAmp);
    addAP("DigiDatap[#].inChn[#].calib.injCalLen", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::injCalLen);
    addAP("DigiDatap[#].inChn[#].calib.fullKernelLen", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::fullKernelLen);
    addAP("DigiDatap[#].inChn[#].calib.electrodeKernelLen", &DigiDataProxy::p, &DigiDataData::inChn, &inChnData::calib, &elecCalibParams::electrodeKernelLen);
}

//---------------------------------------------------------------------------

DigiData::DigiData(size_t devID) :
    DAQ(devID)
{
  inChnNo= 16;
  inIdx= new short int[inChnNo];
  inGainFac= new double[inChnNo];
  outChnNo= 2;       
  outIdx= new short int[outChnNo];
  outGainFac= new double[outChnNo];
  inGainNo= 4;
  inLow= QVector<double>(inGainNo);
  inLow[0]= -10.24;
  inLow[1]= -5.12;
  inLow[2]= -2.56;
  inLow[3]= -1.28;
  inHigh= QVector<double>(inGainNo);
  inHigh[0]= 10.24;
  inHigh[1]= 5.12;
  inHigh[2]= 2.56;
  inHigh[3]= 1.28;

  inGainText= new char*[inGainNo];
  inGainText[0]= new char[80];
  strcpy(inGainText[0], "-10.24 to +10.24 V");
  inGainText[1]= new char[80];
  strcpy(inGainText[1], "-5.12 to +5.12 V");
  inGainText[2]= new char[80];
  strcpy(inGainText[2], "-2.56 to +2.56 V");
  inGainText[3]= new char[80];
  strcpy(inGainText[3], "-1.28 to +1.28 V");
  outGainNo= 1;
  outLow= QVector<double>(outGainNo);
  outLow[0]= -10.24;
  outHigh= QVector<double>(outGainNo);
  outHigh[0]= 10.24;

  outGainText= new char*[outGainNo];
  outGainText[0]= new char[80];
  strcpy(outGainText[0], "-10.24 to +10.24 V");

  inChnGain= new short int[inGainNo];
  inChnGain[0]= 0x0000;
  inChnGain[1]= 0x2000;
  inChnGain[2]= 0x4000;
  inChnGain[3]= 0x6000;

  DACEnable= new short int[outChnNo];  
  DACEnable[0]= DACCHANNEL0ENABLE;
  DACEnable[1]= DACCHANNEL1ENABLE;
  portsOpen= false;
  init();
}

DigiData::~DigiData()
{
  //  inGain.~QVector();
  for (int i= 0; i < inGainNo; i++) {
    delete[] inGainText[i];
  }
  delete[] inGainText;
  //  outGain.~QVector();
  for (int i= 0; i < outGainNo; i++) {
    delete[] outGainText[i];
  }
  delete[] outGainText;
  delete[] inChnGain;
  delete[] DACEnable;
  delete[] inIdx;
  delete[] inGainFac;
  delete[] outIdx;
  delete[] outGainFac;
  ClosePortTalk();
}

void DigiData::init()
{
   short int base= DigiDataProxy::p[devID].baseAddress;
   base_address=       base;
   DAC_data=           base | DACDATA;
   ADC_data=           base | ADCDATA;
   board_ID=           base | BOARDID;
   timer_data=         base | TIMERDATA;
   timer_control=      base | TIMERCONTROL;
   timer_status=       base | TIMERSTATUS;
   ADCDAC_control=     base | ADCDACCONTROL;
   INTDMA_control=     base | INTDMACONTROL;
   digital_IO=         base | DIGITALIO;
   channel_scan_list=  base | CHANNELSCANLIST;
   real_time_data_0=   base | REALTIMEDATA0;
   real_time_data_1=   base | REALTIMEDATA1;
   real_time_data_2=   base | REALTIMEDATA2;
   real_time_control=  base | REALTIMECONTROL;
   ADCDAC_status=      base | ADCDACSTATUS;
   reset_control=      base | RESETCONTROL;
}

//---------------------------------------------------------------------------
bool DigiData::initialize_board(QString &name)
{
   unsigned char BoardType;
   bool success= false;

   if (!portsOpen) {
     OpenPortTalk();
     portsOpen= true;
   }
   BoardType= ReadByte(board_ID);
   if(BoardType == DIGIDATA1200ID)
   {
      WriteWord(ADCDAC_control,DD1200AMODE);
      BoardType= ReadByte(board_ID);
   }
   if(BoardType == DIGIDATA1200AID)
   {
      WriteWord(ADCDAC_control,DD1200AMODE);
      WriteWord(INTDMA_control,ZEROWORD);
      WriteWord(reset_control,RESETWHOLEBOARD);
      success= true;
   }

   // writing 0 to the DAC outputs
   WriteWord(ADCDAC_control,DACCHANNEL0ENABLE);
   WriteWord(DAC_data,0x0000);

   WriteWord(ADCDAC_control,DACCHANNEL1ENABLE);
   WriteWord(DAC_data,0x0000);

   name= QString("DigiData1200(A) ");
   
   initialized= success;
   return success;
}


//---------------------------------------------------------------------------
void DigiData::digital_out(unsigned char outbyte)
{
   WriteByte(digital_IO,outbyte);
}


//---------------------------------------------------------------------------
void DigiData::generate_scan_list(short int chnNo, short int *Chns)
{
  short int i, Chan_Gain_Code;
  DAQData *p = params();
  ChannelIndex dex(DAQClass::DD1200, devID, true);

  actInChnNo= chnNo;  
  WriteWord(ADCDAC_control, ADCSCANLISTENABLE);
  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= p->inChn[inIdx[i]].gainFac*inHigh[p->inChn[inIdx[i]].gain]/16.0/COUNTS; // divide by 16.0 b/c of 12 bit in chns
    Chan_Gain_Code= i + inChnGain[p->inChn[inIdx[i]].gain] + (inIdx[i] * CHANNELSHIFT);
    if(i == (actInChnNo -1)) Chan_Gain_Code+= LASTCHANNELFLAG;
    WriteWord(channel_scan_list, Chan_Gain_Code);
    dex.chanID = inIdx[i];
    inChnLabels[inIdx[i]] = dex.toString();
  }
  WriteWord(ADCDAC_control, ADCSCANLISTDISABLE);
}

//---------------------------------------------------------------------------
void DigiData::get_scan()
{
   short int i, scan;

   for(i= 0; i < actInChnNo; i++) WriteWord(reset_control, ADCSTARTCONVERT);

   for(i= 0; i < actInChnNo; i++){
      scan= ReadWord(ADC_data);
      in[inIdx[i]].V= inGainFac[i] * (double) scan;
   }
}

void DigiData::get_single_scan(inChannel *in)
{
   short int i, scan;

   for(i= 0; i < actInChnNo; i++) WriteWord(reset_control, ADCSTARTCONVERT);

   for(i= 0; i < actInChnNo; i++){
      scan= ReadWord(ADC_data);
      if (&(this->in[inIdx[i]]) == in) {
          in->V= inGainFac[i] * (double) scan;
      }
   }
}


//---------------------------------------------------------------------------
void DigiData::generate_analog_out_list(short int chnNo, short int *Chns)
{
  DAQData *p = params();
  ChannelIndex dex(DAQClass::DD1200, devID, false);

  // collect the active out channels
  actOutChnNo= chnNo;
  for (int i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= p->outChn[outIdx[i]].gainFac/outHigh[p->outChn[outIdx[i]].gain]*1e9*COUNTS;
    dex.chanID = outIdx[i];
    outChnLabels[outIdx[i]] = dex.toString();
  }
}


//---------------------------------------------------------------------------
void DigiData::write_analog_out()
{
  static short int int_I;
  for (int i= 0; i < actOutChnNo; i++) {
    int_I= ((short int) (out[outIdx[i]].I*outGainFac[i]))*16;
    int_I|= DigiDataProxy::p[devID].syncIOMask; // write synchronous digital IO
    WriteWord(ADCDAC_control,DACEnable[outIdx[i]]);
    WriteWord(DAC_data,int_I);
  } 
}


//---------------------------------------------------------------------------
void DigiData::reset_board() 
{
  for (int i= 0; i < outChnNo; i++) {
    WriteWord(ADCDAC_control,DACEnable[outIdx[i]]);
    WriteWord(DAC_data,0);
  } 
}

//---------------------------------------------------------------------------
QString DigiData::prefix()
{
    return QString("DigiData_%1").arg(devID);
}
