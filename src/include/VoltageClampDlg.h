#pragma once

#include <QAbstractButton>
#include "ui_VoltageClampDlg.h"
#include "ObjectDataTypes.h"

class VoltageClampDlg : public QDialog, private Ui::VoltageClampDlg
{
     Q_OBJECT

  public:
     VoltageClampDlg(int, ChannelListModel *in, ChannelListModel *out, QWidget *parent= 0);
     void exportData(VoltageClampData &);
     void importData(VoltageClampData);
     void setIndex(int);

     QString label;

     typedef VoltageClampData param_type;
     
  public slots:
     void CloseClicked(QAbstractButton *);
}; 

