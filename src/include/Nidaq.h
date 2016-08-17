#ifndef NIDAQ_H
#define NIDAQ_H

using namespace std;

#include "Channels.h"
#include "DAQ.h"

#include <NIDAQmx.h>

#define MAXRANGES 64
#define MAXCHANNELS 64

class NIDAQ: public DAQ
{
  private:
   char *devName;
   char *iChnNm[MAXCHANNELS];
   char *oChnNm[MAXCHANNELS];
          
   TaskHandle inTask;
   int inTaskActive;
   float64 *inBuf;
   TaskHandle outTask;
   int outTaskActive;
   float64 *outBuf;
   int DevicePresent;
             
  public:
    NIDAQ();
    virtual ~NIDAQ();
    void init();
    virtual bool initialize_board(QString &);
    virtual void reset_RTC();
    virtual void digital_out(unsigned char);
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan(inChannel *);
    virtual void get_single_scan(inChannel *, int);
    virtual void write_analog_out(outChannel *);
    virtual void reset_board();
    //short int* inChnGain;
    //short int* DACEnable;
    
    // actually activated stuff
};

#endif
