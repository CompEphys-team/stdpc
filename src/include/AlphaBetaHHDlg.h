#ifndef AlphaBetaHHDLG_H
#define AlphaBetaHHDLG_H


#include "ui_AlphaBetaHHDlg.h"
#include "ObjectDataTypes.h"

class AlphaBetaHHDlg : public QDialog, private Ui::AlphaBetaHHDlg
{
     Q_OBJECT

  public:
     AlphaBetaHHDlg(int, QWidget *parent= 0);
     void exportData(abHHData &);
     void importData(abHHData);
     void setIndex(int);
     typedef abHHData param_type;

     QString label;
}; 

#endif
