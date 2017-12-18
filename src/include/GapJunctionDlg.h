#ifndef GAPJUNCTIONDLG_H
#define GAPJUNCTIONDLG_H

#include "ConductanceDlg.h"
#include "ui_GapJunctionDlg.h"
#include "ObjectDataTypes.h"

class GapJunctionDlg : public ConductanceDlg, private Ui::GapJunctionDlg
{
     Q_OBJECT

  public:
     GapJunctionDlg(int, QWidget *parent= 0);
     void exportData();
     void importData();
     void setIndex(int);
private slots:
     void on_buttonBox_clicked(QAbstractButton *button);
};

#endif
