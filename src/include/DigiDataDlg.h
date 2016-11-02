#ifndef DIGIDATADLG_H
#define DIGIDATADLG_H

#include "ui_DigiDataDlg.h"
#include "DAQDlg.h"

class DigiDataDlg : public DAQDlg, private Ui::DigiDataDlg
{
public:
     DigiDataDlg(size_t idx, QWidget *parent= 0);
     void exportData(bool forceInit = false);
     void importData();
     void setIndex(size_t);

protected:
    void backup();
    void restoreBackup();

    DAQProxy *proxy() const;

    DigiDataData bak;
    QString label;
};

#endif
