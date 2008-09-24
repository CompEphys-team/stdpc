#ifndef HHDLG_H
#define HHDLG_H

#include "ui_HHDlg.h"
#include "ObjectDataTypes.h"

class HHDlg : public QDialog, private Ui::HHDlg
{
     Q_OBJECT

  public:
     int x;
     HHDlg(int, QWidget *parent= 0);
     void exportData(mhHHData &);
     void importData(mhHHData);
     
  public slots:
    void updateOutChn(int, int*);
    void updateInChn(int, int*);
         
}; 

#endif
