#include "DigiData.h"
#include "limits.h"

// DigiData 1200 device driver

//---------------------------------------------------------------------------

DigiData::DigiData()
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
  inGainText[0]= "-10.24 to +10.24 V";
  inGainText[1]= "-5.12 to +5.12 V";
  inGainText[2]= "-2.56 to +2.56 V";
  inGainText[3]= "-1.28 to +1.28 V";          
  outGainNo= 1;
  outLow= QVector<double>(outGainNo);
  outLow[0]= -10.24;
  outHigh= QVector<double>(outGainNo);
  outHigh[0]= 10.24;

  outGainText= new char*[outGainNo];
  outGainText[0]= "-10.24 to +10.24 V";

  inChnGain= new short int[inGainNo];
  inChnGain[0]= 0x0000;
  inChnGain[1]= 0x2000;
  inChnGain[2]= 0x4000;
  inChnGain[3]= 0x6000;

  DACEnable= new short int[outChnNo];  
  DACEnable[0]= DACCHANNEL0ENABLE;
  DACEnable[1]= DACCHANNEL1ENABLE;
  
  clock_frequency= 4000000.0; // 4MHz clock
  clock_cycle= ((double) USHRT_MAX + 1.0)/clock_frequency;
  
  portsOpen= false;
  init();
};

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
   short int base= DigiDatap.baseAddress;
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
void DigiData::reset_RTC()
{
   WriteByte(real_time_control,C0MODE2);
   WriteByte(real_time_data_0,ZEROBYTE);
   WriteByte(real_time_data_0,ZEROBYTE);

   WriteByte(real_time_control,C1MODE2);
   WriteByte(real_time_data_1,FASTBYTE);  // very fast to generate clock to
   WriteByte(real_time_data_1,ZEROBYTE); // counter 2 soon --> reset C2

   WriteByte(real_time_control,C2MODE2);
   WriteByte(real_time_data_2,ZEROBYTE);
   WriteByte(real_time_data_2,ZEROBYTE);

   do{                                       // wait for counter 2 reset
      WriteByte(real_time_control,READCOUNT2);
   }while((ReadByte(real_time_data_2) & ReadByte(real_time_data_2)) != 0xff);

   WriteByte(real_time_control,C1MODE2);
   WriteByte(real_time_data_1,ZEROBYTE);
   WriteByte(real_time_data_1,ZEROBYTE);
}

//---------------------------------------------------------------------------
double DigiData::get_RTC(void)
{
//     short unsigned int hi, med;
   static short unsigned int lo;
   static double dt, lastT;
   /* I'm returning only the 16 bit low order register which makes the program
run more fast but put some limitation in the time required to make a cycle R-W
of ADC/DAC operations. The maximum time of the cycle should not be larger than
16 ms. One should use the program runing in realtime.
*/

   WriteByte(real_time_control,READALLCOUNTERS);

//   hi= ~(ReadByte(real_time_data_2)+(256*ReadByte(real_time_data_2)));
//   med= ~(ReadByte(real_time_data_1)+(256*ReadByte(real_time_data_1))-2);
   lo= ~(ReadByte(real_time_data_0)+(256*ReadByte(real_time_data_0))-2);
//   t= (double)lo + 65536.0*(double)med + 4294967296.0*(double)hi;
   lastT= t;
   t= ((double)lo)/clock_frequency;
   dt= t-lastT;
   if (dt < 0.0) dt+= clock_cycle;
   
   return dt;
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

  actInChnNo= chnNo;  
  WriteWord(ADCDAC_control, ADCSCANLISTENABLE);
  for(i= 0; i < actInChnNo; i++)
  {
    inIdx[i]= Chns[i];
    inGainFac[i]= inChnp[inIdx[i]].gainFac*inHigh[inChnp[inIdx[i]].gain]/16.0/COUNTS; // divide by 16.0 b/c of 12 bit in chns
    Chan_Gain_Code= i + inChnGain[inChnp[inIdx[i]].gain] + (inIdx[i] * CHANNELSHIFT);
    if(i == (actInChnNo -1)) Chan_Gain_Code+= LASTCHANNELFLAG;
    WriteWord(channel_scan_list, Chan_Gain_Code);
  }
  WriteWord(ADCDAC_control, ADCSCANLISTDISABLE);
}

//---------------------------------------------------------------------------
void DigiData::get_scan(inChannel *in)
{
   short int i, scan;

   for(i= 0; i < actInChnNo; i++) WriteWord(reset_control, ADCSTARTCONVERT);

   for(i= 0; i < actInChnNo; i++){
      scan= ReadWord(ADC_data);
      in[inIdx[i]].V= inGainFac[i] * (double) scan;
   }
}

//---------------------------------------------------------------------------
void DigiData::generate_analog_out_list(short int chnNo, short int *Chns)
{
  // collect the active out channels
  actOutChnNo= chnNo;
  for (int i= 0; i < actOutChnNo; i++) {
    outIdx[i]= Chns[i];
    outGainFac[i]= outChnp[outIdx[i]].gainFac/outHigh[outChnp[outIdx[i]].gain]*16.0*1e9*COUNTS;
  }
}


//---------------------------------------------------------------------------
void DigiData::write_analog_out(outChannel *out)
{
  static short int int_I;
  for (int i= 0; i < actOutChnNo; i++) {
    int_I= (short int) (out[outIdx[i]].I*outGainFac[i]);
    int_I|= DigiDatap.syncIOMask; // write synchronous digital IO
    WriteWord(ADCDAC_control,DACEnable[outIdx[i]]);
    WriteWord(DAC_data,int_I);
  } 
}


void DigiData::reset_board() 
{
  for (int i= 0; i < outChnNo; i++) {
    WriteWord(ADCDAC_control,DACEnable[outIdx[i]]);
    WriteWord(DAC_data,0);
  } 
}
