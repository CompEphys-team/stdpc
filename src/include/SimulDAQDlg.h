#ifndef SIMULDAQDLG_H
#define SIMULDAQDLG_H

#include "ui_SimulDAQDlg.h"
#include "DAQDlg.h"
#include "SimulDAQ.h"

class SimulDAQDlg : public DAQDlg, private Ui::SimulDAQDlg
{
    Q_OBJECT

public:
     SimulDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

private slots:
     void on_InputFileB_clicked();
     void on_OutputFileB_clicked();

protected:
     bool change;
     void backup();
     void restoreBackup();

     SDAQData bak;
};

#endif
