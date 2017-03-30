#ifndef CHEMSYNDLG_H
#define CHEMSYNDLG_H


#include <QAbstractButton>
#include "ui_ChemSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class ChemSynDlg : public QDialog, private Ui::ChemSynDlg
{
     Q_OBJECT

  public:
     ChemSynDlg(int, QWidget *parent= 0);
     void exportData(CSynData &);
     void importData(CSynData);
     void setIndex(int);

     QString label;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;

     typedef CSynData param_type;
     
  public slots:
     void PlastMethodChange();
     void STDComboChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
