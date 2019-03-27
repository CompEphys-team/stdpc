#ifndef AlphaBetaHHDLG_H
#define AlphaBetaHHDLG_H

#include "ConductanceDlg.h"
#include "ui_AlphaBetaHHDlg.h"
#include "ObjectDataTypes.h"

class AlphaBetaHHDlg : public ConductanceDlg, private Ui::AlphaBetaHHDlg
{
     Q_OBJECT

  public:
     AlphaBetaHHDlg(size_t, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);
private slots:
     void on_buttonBox_clicked(QAbstractButton *button);
};

#endif
