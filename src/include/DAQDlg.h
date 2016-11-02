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
     DAQDlg(size_t idx, DAQ *board, QWidget *parent = nullptr);

     virtual void importData();
     virtual void exportData(bool forceInit = false);

     virtual void setIndex(size_t i);

public slots:
    inline void accept() { exportData(); QDialog::accept(); }
    inline void reject() { restoreBackup(); if ( didInit ) initDAQ(); importData(); QDialog::reject(); }
    inline void open() { backup(); didInit = false; QDialog::open(); }
    
signals:
    void message(QString);
    void deviceStatusChanged(DeviceStatus, const QString&);
    void channelsChanged();
    void CloseToLimit(QString,QString,double,double,double);

protected slots:
    inline void openInChnDlg() { exportData(); inDlg->open(); }
    inline void openOutChnDlg() { exportData(); outDlg->open(); }

protected:
    size_t idx;
    InputChannelDlg *inDlg;
    OutputChannelDlg *outDlg;
    bool didInit;

    virtual void backup() = 0;
    virtual void restoreBackup() = 0;

    void initDAQ();

    virtual DAQProxy *proxy() const = 0;
}; 

#endif




