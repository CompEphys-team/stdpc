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
     abSynDlg(size_t, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);

     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;
     
  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
