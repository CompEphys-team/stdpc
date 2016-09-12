#ifndef SIMULDAQ_H
#define SIMULDAQ_H

using namespace std;

#include <iostream>
#include <fstream>
#include <QList>
#include "Daq.h"

class SimulDAQ: public DAQ
{
  public:
    SimulDAQ();
    virtual ~SimulDAQ();
    virtual bool initialize_board(QString &);
    virtual void reset_RTC();
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan(inChannel *);
    virtual void get_single_scan(inChannel *, int);
    virtual void write_analog_out(outChannel *);
    virtual void reset_board();

    ifstream is;
    ofstream os;  

    QList<double> intq;
    QList<double> outtq;
    QList<double> *inq;
    QList<double> *outq;
    
    QList<double>::const_iterator intIter;
    QList<double>::const_iterator *inIter;
    
  private:
    void rewind();
    
    double dataT;
    double inT;
    double lastWrite;
    
    void flush_analog_out();
};
      
#endif
