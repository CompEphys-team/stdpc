#ifndef GAPJUNCTIONDLG_H
#define GAPJUNCTIONDLG_H

#include "ConductanceDlg.h"
#include "ui_GapJunctionDlg.h"
#include "ObjectDataTypes.h"

class GapJunctionDlg : public ConductanceDlg, private Ui::GapJunctionDlg
{
     Q_OBJECT

  public:
     GapJunctionDlg(size_t, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);
private slots:
     void on_buttonBox_clicked(QAbstractButton *button);
};

#endif
