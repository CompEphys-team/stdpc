#ifndef NIDAQDLG_H
#define NIDAQDLG_H

#include "ui_NIDAQDlg.h"
#include "DAQDlg.h"
#include "Nidaq.h"

class NIDAQDlg : public DAQDlg, private Ui::NIDAQDlg
{
public:
     NIDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

protected:
    void backup();
    void restoreBackup();

    NIDAQData bak;
};

#endif
