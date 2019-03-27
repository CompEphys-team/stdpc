#ifndef HHDLG_H
#define HHDLG_H

#include "ui_HHDlg.h"
#include "ObjectDataTypes.h"
#include "ConductanceDlg.h"

class HHDlg : public ConductanceDlg, private Ui::HHDlg
{
     Q_OBJECT

  public:
     HHDlg(size_t, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);
private slots:
     void on_buttonBox_clicked(QAbstractButton *button);
};

#endif
