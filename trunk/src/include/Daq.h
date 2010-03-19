#ifndef DAQ_H
#define DAQ_H

#include "Channels.h"

class DAQ {
  public:
    DAQ();
    virtual ~DAQ();
    virtual bool initialize_board(QString &)= 0;
    virtual void reset_RTC()= 0;
    virtual double get_RTC()= 0;
    virtual void generate_scan_list(short int, short int *)= 0;
    virtual void generate_analog_out_list(short int, short int *)= 0;
    virtual void get_scan(inChannel *)= 0;
    virtual void write_analog_out(outChannel *)= 0;
    virtual void reset_board()= 0;
    
    int inChnNo;
    int outChnNo;

    int inGainNo;
    QVector<double> inGain;
    char **inGainText;

    int outGainNo;
    QVector<double> outGain;
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
