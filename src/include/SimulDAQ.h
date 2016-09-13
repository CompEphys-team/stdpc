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
    SimulDAQ(SDAQData *p, int devID, Clock *clk);
    virtual ~SimulDAQ();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan();
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out();
    virtual void reset_board();

    virtual QString prefix();

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
    double tOff;
    
    void flush_analog_out();
};
      
#endif
