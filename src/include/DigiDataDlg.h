#ifndef DIGIDATADLG_H
#define DIGIDATADLG_H

#include "ui_DigiDataDlg.h"
#include "DAQDlg.h"
#include "DigiData.h"

class DigiDataDlg : public DAQDlg, private Ui::DigiDataDlg
{
public:
     DigiDataDlg(size_t idx, DAQProxy *proxy, QWidget *parent= 0);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

protected:
    void backup();
    void restoreBackup();

    DigiDataData bak;
};

#endif
