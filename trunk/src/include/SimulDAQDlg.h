#ifndef SIMULDAQDLG_H
#define SIMULDAQDLG_H


#include "ui_SimulDAQDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"

class SimulDAQDlg : public DAQDlg, private Ui::SimulDAQDlg
{
     Q_OBJECT

  public:
     SimulDAQDlg(QWidget *parent= 0);
     bool exportData(SDAQData &);
     void importData(SDAQData);
     
  public slots:
     void accept();
     void reject();
     void appear();

}; 

#endif
