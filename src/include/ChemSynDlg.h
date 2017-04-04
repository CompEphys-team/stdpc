#ifndef CHEMSYNDLG_H
#define CHEMSYNDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_ChemSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class ChemSynDlg : public ConductanceDlg, private Ui::ChemSynDlg
{
     Q_OBJECT

  public:
     ChemSynDlg(int, QWidget *parent= 0);
     void exportData();
     void importData();
     void setIndex(int);

     QString label;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;
     
  public slots:
     void PlastMethodChange();
     void STDComboChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
