#ifndef STDPDLG_H
#define STDPDLG_H


#include "ui_STDPDlg.h"
#include "ObjectDataTypes.h"

class STDPDlg : public QDialog, private Ui::STDPDlg
{
     Q_OBJECT

  public:
     STDPDlg(QWidget *parent= 0);
     void exportData(STPlast &);
     void importData(STPlast);
     void setLabel(const QString&);
}; 

#endif
