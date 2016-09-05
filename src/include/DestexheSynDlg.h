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
     DestexheSynDlg(int, ChannelListModel *in, ChannelListModel *out, QWidget *parent= 0);
     void exportData(DestexheSynData &);
     void importData(DestexheSynData);

     int No;
     STDPDlg *STDP;
     ODESTDPDlg *ODESTDP;

  public slots:
     void PlastMethodChange();
     void ResCloseClicked(QAbstractButton *);
};

#endif
