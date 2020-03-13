#ifndef PIPEDAQDLG_H
#define PIPEDAQDLG_H

#include "ui_pipedaqdlg.h"
#include "DAQDlg.h"
#include "PipeDAQ.h"

class PipeDAQDlg : public DAQDlg, private Ui::PipeDAQDlg
{
    Q_OBJECT

public:
     PipeDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

protected:
     void backup();
     void restoreBackup();

     PipeDAQData bak;
};

#endif // PIPEDAQDLG_H
