#ifndef AlphaBetaHHDLG_H
#define AlphaBetaHHDLG_H

#include "ConductanceDlg.h"
#include "ui_AlphaBetaHHDlg.h"
#include "ObjectDataTypes.h"

class AlphaBetaHHDlg : public ConductanceDlg, private Ui::AlphaBetaHHDlg
{
     Q_OBJECT

  public:
     AlphaBetaHHDlg(int, QWidget *parent= 0);
     void exportData();
     void importData();
     void setIndex(int);
}; 

#endif
