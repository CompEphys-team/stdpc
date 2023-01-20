#ifndef MICROMANAGERDAQDLG_H
#define MICROMANAGERDAQDLG_H

#include "ui_MicroManagerDAQDlg.h"
#include "DAQDlg.h"
#include "MicroManagerDAQ.h"

class MicroManagerDAQDlg : public DAQDlg, private Ui::MicroManagerDAQDlg
{
    Q_OBJECT

public:
     MicroManagerDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

protected:
     void backup();
     void restoreBackup();

     MicroManagerDAQData bak;
};

#endif // MICROMANAGERDAQDLG_H
