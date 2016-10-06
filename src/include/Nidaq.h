#ifndef NIDAQ_H
#define NIDAQ_H

using namespace std;

#include "Channels.h"
#include "Daq.h"

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
    NIDAQ(int devID);
    virtual ~NIDAQ();
    void init();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void digital_out(unsigned char);
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan();
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out();
    virtual void reset_board();

    virtual inline DAQData *params() { return &NIDAQp[devID]; }
    virtual QString prefix();
};

#endif
