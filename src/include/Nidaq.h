#ifndef NIDAQ_H
#define NIDAQ_H

#include "DeviceManager.h"
#include <NIDAQmx.h>

#define MAXRANGES 64
#define MAXCHANNELS 64

class NIDAQData : public DAQData
{
public:
    QString deviceName;
    NIDAQData() : DAQData(), deviceName("Dev1") {}
};

class NIDAQProxy : public DAQProxy {
public:
    NIDAQProxy();
    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "NIDAQ"; }
    inline QString prettyName() { return "Nat'l Instruments"; }

    DAQ *createDAQ(size_t devID);
    DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr);

    std::vector<AP*> regAP;
    inline const std::vector<AP*> &coreAPs() const { return regAP; }

    static std::vector<NIDAQData> p;
};

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
    NIDAQ(size_t devID, DAQProxy *proxy);
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
};

#endif
