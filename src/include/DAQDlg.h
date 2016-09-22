#ifndef DAQDLG_H
#define DAQDLG_H

#include <QDialog>
#include "InputChannelDlg.h"
#include "OutputChannelDlg.h"
#include "DeviceManager.h"

class DAQDlg : public QDialog
{
     Q_OBJECT

  public:
     DAQDlg(int no, QWidget *parent= 0);
    
  signals:
    void message(QString);
    void deviceStatusChanged(DeviceStatus, const QString&);
    void channelsChanged();
    void CloseToLimit(QString,QString,double,double,double);

protected:
    int idx;
    InputChannelDlg *inDlg;
    OutputChannelDlg *outDlg;
    DAQ *board;
    bool didInit;
}; 

#endif




