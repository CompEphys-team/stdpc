#ifndef SIMULDAQ_H
#define SIMULDAQ_H

#include <iostream>
#include <fstream>
#include <QList>
#include "DeviceManager.h"

class SDAQData : public DAQData {
  public:
    QuotedString inFileName;
    QuotedString outFileName;
    double inTFac;
    double outDt;
    SDAQData() : DAQData(),
        inFileName("StdpcIn1.dat"),
        outFileName("StdpcOut1.dat"),
        inTFac(1),
        outDt(0.001)
    {}
};

class SimulDAQProxy : public DAQProxy {
public:
    SimulDAQProxy();
    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "SimulDAQ"; }

    DAQ *createDAQ(size_t devID);
    DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr);

    std::vector<AP*> regAP;
    inline const std::vector<AP*> &coreAPs() const { return regAP; }

    static std::vector<SDAQData> p;
};

class SimulDAQ: public DAQ
{
  public:
    SimulDAQ(size_t devID, DAQProxy *proxy);
    virtual ~SimulDAQ();
    virtual bool initialize_board(QString &);
    virtual void start();
    virtual void generate_scan_list(short int, QVector<short int>);
    virtual void generate_analog_out_list(short int, QVector<short>);
    virtual void get_scan();
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out();
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
    double tOff;
    
    void flush_analog_out();
};
      
#endif
