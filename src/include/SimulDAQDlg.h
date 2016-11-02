#ifndef SIMULDAQDLG_H
#define SIMULDAQDLG_H

#include "ui_SimulDAQDlg.h"
#include "DAQDlg.h"

class SimulDAQDlg : public DAQDlg, private Ui::SimulDAQDlg
{
    Q_OBJECT

public:
     SimulDAQDlg(size_t idx, QWidget *parent= 0);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

private slots:
     void on_InputFileB_clicked();
     void on_OutputFileB_clicked();

protected:
     void backup();
     void restoreBackup();

     DAQProxy *proxy() const;

     SDAQData bak;
     QString label;
};

#endif
