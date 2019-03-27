#ifndef ODESTDPDLG_H
#define ODESTDPDLG_H


#include "ui_ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class ODESTDPDlg : public QDialog, private Ui::ODESTDPDlg
{
     Q_OBJECT

  public:
     ODESTDPDlg(QWidget *parent= nullptr);
     void exportData(ODEPlast &);
     void importData(ODEPlast);
     void setLabel(const QString&);
}; 

#endif
