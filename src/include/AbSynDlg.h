#ifndef ABSYNDLG_H
#define ABSYNDLG_H


#include <QAbstractButton>
#include "ui_abSynDlg.h"
#include "STDPDlg.h"
#include "ODESTDPDlg.h"
#include "ObjectDataTypes.h"

class abSynDlg : public QDialog, private Ui::abSynDlg
{
     Q_OBJECT

  public:
     abSynDlg(int, ChannelListModel *in, ChannelListModel *out, QWidget *parent= 0);
     void exportData(abSynData &);
     void importData(abSynData);

     int No;   
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;

     typedef abSynData param_type;
     
  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
}; 

#endif
