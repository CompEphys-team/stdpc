#ifndef HHDLG_H
#define HHDLG_H

#include "ui_HHDlg.h"
#include "ObjectDataTypes.h"
#include "ConductanceDlg.h"

class HHDlg : public ConductanceDlg, private Ui::HHDlg
{
     Q_OBJECT

  public:
     HHDlg(int, QWidget *parent= 0);
     void exportData();
     void importData();
     void setIndex(int);
private slots:
     void on_buttonBox_clicked(QAbstractButton *button);
};

#endif
