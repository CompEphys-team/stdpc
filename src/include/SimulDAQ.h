#ifndef SIMULDAQ_H
#define SIMULDAQ_H

using namespace std;

#include <iostream>
#include <fstream>
#include <QList>
#include "DeviceManager.h"

class SimulDAQProxy : public DAQProxy {
public:
    SimulDAQProxy();
    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "SimulDAQ"; }

    inline DAQ *createDAQ(size_t devID);
    /* NYI: inline DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr); */

    static std::vector<SDAQData> p;
};

class SimulDAQ: public DAQ
{
  public:
    SimulDAQ(size_t devID);
    virtual ~SimulDAQ();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan();
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out();
    virtual void reset_board();

    virtual inline DAQData *params() { return &SimulDAQProxy::p[devID]; }
    virtual DAQProxy *proxy() const;
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
