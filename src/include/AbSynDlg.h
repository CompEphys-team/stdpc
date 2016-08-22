#ifndef ABSYNDLG_H
#define ABSYNDLG_H


#include <QAbstractButton>
#include "ui_abSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"
#include "SynapseAssignmentDlg.h"

class abSynDlg : public QDialog, private Ui::abSynDlg
{
     Q_OBJECT

  public:
     abSynDlg(int, QWidget *parent= 0);
     void exportData(abSynData &);
     void importData(abSynData);

     int No;   
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;
     
  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
     void updateOutChn(int, int*);
     void updateInChn(int, int*);
     
  private:
     SynapseAssignmentDlg sa;
}; 

#endif
