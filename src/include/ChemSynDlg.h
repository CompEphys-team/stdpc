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
     ChemSynDlg(size_t, QWidget *parent= nullptr);
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
