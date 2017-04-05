#ifndef ABSYNDLG_H
#define ABSYNDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_AbSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class abSynDlg : public ConductanceDlg, private Ui::abSynDlg
{
     Q_OBJECT

  public:
     abSynDlg(int, QWidget *parent= 0);
     void exportData();
     void importData();
     void setIndex(int);

     QString label;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;
     
  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
