#ifndef NIDAQDLG_H
#define NIDAQDLG_H


#include "ui_NIDAQDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"

class NIDAQDlg : public DAQDlg, private Ui::NIDAQDlg
{
     Q_OBJECT

  public:
     NIDAQDlg(QWidget *parent= 0);
     bool exportData(NIDAQData &);
     void importData(NIDAQData);
     
  public slots:
    void accept();
    void reject();
    
}; 

#endif
