#ifndef NIDAQ_H
#define NIDAQ_H

#include "Channels.h"
#include "DAQ.h"

#include <nidaqmx.h>
#include <windows.h>

//-------------- Constants related to Digidata 1200A board ------------------

#define MAXRANGES 64
#define MAXCHANNELS 64

class NIDAQ: public DAQ
{
  private:
   LARGE_INTEGER intClock_frequency;
   double sysT;
   double t;
   char *devName;
   double *inLow;
   double *inHigh;
   double *outLow;
   double *outHigh;
   char *iChnNm[MAXCHANNELS];
   char *oChnNm[MAXCHANNELS];
          
   TaskHandle inTask;
   int inTaskActive;
   float64 *inBuf;
   TaskHandle outTask;
   int outTaskActive;
   float64 *outBuf;
            
  public:
    NIDAQ();
    virtual ~NIDAQ();
    void init();
    virtual bool initialize_board(QString &);
    virtual void reset_RTC();
    virtual double get_RTC();
    virtual void digital_out(unsigned char);
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan(inChannel *);
    virtual void write_analog_out(outChannel *);
    virtual void reset_board();
    short int* inChnGain;
    short int* DACEnable;
    
    // actually activated stuff
};

#endif
