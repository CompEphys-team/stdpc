#ifndef DIGIDATADLG_H
#define DIGIDATADLG_H


#include "ui_DigiDataDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"

class DigiDataDlg : public DAQDlg, private Ui::DigiDataDlg
{
     Q_OBJECT

  public:
     DigiDataDlg(QWidget *parent= 0);
     bool exportData(DigiDataData &);
     void importData(DigiDataData);
     
  public slots:
    void accept();
    void reject();
    void appear();
    
}; 

#endif
