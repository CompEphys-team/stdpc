#ifndef DAQ_H
#define DAQ_H

#include <windows.h>
#include "Channels.h"

class DAQ {
  private:
    LARGE_INTEGER intClock_frequency;
    double sysT;

  public:
    DAQ();
    virtual ~DAQ();
    virtual bool initialize_board(QString &)= 0;
    virtual void reset_RTC();
    virtual double get_RTC();
    virtual double wait_till_elapsed(double);
    virtual void generate_scan_list(short int, short int *)= 0;
    virtual void generate_analog_out_list(short int, short int *)= 0;
    virtual void get_scan(inChannel *)= 0;
    virtual void get_single_scan(inChannel *, int)= 0;
    virtual void write_analog_out(outChannel *)= 0;
    virtual void reset_board()= 0;
    
    double t;

    int inChnNo;
    int outChnNo;

    int inGainNo;
    QVector<double> inLow;
    QVector<double> inHigh;
    char **inGainText;

    int outGainNo;
    QVector<double> outLow;
    QVector<double> outHigh;
    char **outGainText;

// activated stuff
    short int actInChnNo;
    short int *inIdx;
    double *inGainFac;
    short int actOutChnNo;
    short int *outIdx;
    double *outGainFac;
    double clock_frequency;
    double clock_cycle;

    bool initialized;
};

#endif
