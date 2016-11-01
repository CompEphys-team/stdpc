#ifndef DIGIDATA_H
#define DIGIDATA_H

#include "Pt_ioctl_tn.h"
#include "DeviceManager.h"

// we need to translate commands because we are using portTalkX instead of the Borland IOport

#define WriteWord outports
#define ReadWord inports
#define WriteByte outportc
#define ReadByte inportc


//-------------- Constants related to Digidata 1200A board ------------------
//  Addresses:
#define BASEADDRESS             0x320
//#define BASEADDRESS             0x340 (Attila)
#define DACDATA                 0x00
#define ADCDATA                 0x00
#define BOARDID                 0x02
#define TIMERDATA               0x04
#define TIMERCONTROL            0x06
#define TIMERSTATUS             0x06
#define ADCDACCONTROL           0x08
#define INTDMACONTROL           0x0A
#define DIGITALIO               0x0C
#define CHANNELSCANLIST         0x0E
#define REALTIMEDATA0           0x10
#define REALTIMEDATA1           0x12
#define REALTIMEDATA2           0x14
#define REALTIMECONTROL         0x16
#define ADCDACSTATUS            0x18
#define RESETCONTROL            0x1A
//   Board IDs:
#define DIGIDATA1200ID          2
#define DIGIDATA1200AID         3
//   ADCDAC Control
#define DD1200AMODE             0x0040
#define ENABLEDIGITALIO         0x0008
#define ADCASYNCDAC             0x0004
#define ADCGEARENABLE           0x0060
#define ADCSCANLISTENABLE       0x0050
#define ADCSCANLISTDISABLE      0x0040
#define DACCHANNEL0ENABLE       0x0001
#define DACCHANNEL1ENABLE       0x0002
//   ADC/DAC Status
#define ADCDATAREADY            0x0040
//   INT/DMA Control
#define ZEROWORD                0x0000
//   Reset Control
#define RESETWHOLEBOARD         0x007F
#define RESETSCANLIST           0x10
#define ADCSTARTCONVERT         0x0080
#define RESETADCFLAGS           0x0010
//   Real Time Clock
#define C0MODE2                 0x34
#define C1MODE2                 0x74
#define C2MODE2                 0xB4
#define ZEROBYTE                0x00
#define FASTBYTE                0x20
#define READCOUNT0              0x00
#define READCOUNT1              0x40
#define READCOUNT2              0x80
#define READCOUNT0AND1          0xD6
#define READALLCOUNTERS         0xDE

//   Channel Scan List
#define CHANNELSHIFT            0x0100
#define LASTCHANNELFLAG         0x8000

//   Conversion counts -> volts
#define COUNTS                  2048.0


class DigiDataProxy : public DAQProxy {
public:
    DigiDataProxy();
    inline DAQData &param(size_t i) { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString daqClass() { return "DigiData1200"; }
    inline QString prettyName() { return "DigiData 1200(A)"; }

    inline DAQ *createDAQ(size_t devID);
    /* NYI: inline DAQDlg *createDialog(size_t devID, QWidget *parent=nullptr); */

    static std::vector<DigiDataData> p;
};

class DigiData: public DAQ
{
  private:
   short int base_address;
   short int DAC_data;
   short int ADC_data;
   short int board_ID;
   short int timer_data;
   short int timer_control;
   short int timer_status;
   short int ADCDAC_control;
   short int INTDMA_control;
   short int digital_IO;
   short int channel_scan_list;
   short int real_time_data_0; 
   short int real_time_data_1;  
   short int real_time_data_2;  
   short int real_time_control; 
   short int ADCDAC_status; 
   short int reset_control;     
   bool portsOpen;
   double t;
            
  public:
    DigiData(size_t devID);
    virtual ~DigiData();
    void init();
    virtual bool initialize_board(QString &);
    virtual void start() {}
    virtual void digital_out(unsigned char);
    virtual void generate_scan_list(short int, short int *);
    virtual void generate_analog_out_list(short int, short int *);
    virtual void get_scan();
    virtual void get_single_scan(inChannel *);
    virtual void write_analog_out();
    virtual void reset_board();
    short int* inChnGain;
    short int* DACEnable;

    virtual inline DAQData *params() { return &DigiDataProxy::p[devID]; }
    virtual DAQProxy *proxy() const;
    virtual QString prefix();
    
    // actually activated stuff
};

#endif
