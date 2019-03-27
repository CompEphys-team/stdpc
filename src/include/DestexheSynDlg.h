#ifndef DESTEXHESYNDLG_H
#define DESTEXHESYNDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_DestexheSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class DestexheSynDlg : public ConductanceDlg, private Ui::DestexheSynDlg
{
     Q_OBJECT

  public:
     DestexheSynDlg(size_t, QWidget *parent= nullptr);
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
