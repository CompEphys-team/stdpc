#ifndef DESTEXHESYNDLG_H
#define DESTEXHESYNDLG_H


#include <QAbstractButton>
#include "ui_DestexheSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class DestexheSynDlg : public QDialog, private Ui::DestexheSynDlg
{
     Q_OBJECT

  public:
     DestexheSynDlg(int, QWidget *parent= 0);
     void exportData(DestexheSynData &);
     void importData(DestexheSynData);
     void setIndex(int);

     QString label;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;

     typedef DestexheSynData param_type;

  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
};

#endif
