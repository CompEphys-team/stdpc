#ifndef AlphaBetaHHDLG_H
#define AlphaBetaHHDLG_H


#include "ui_AlphaBetaHHDlg.h"
#include "ObjectDataTypes.h"
#include "CurrentAssignmentDlg.h"

class AlphaBetaHHDlg : public QDialog, private Ui::AlphaBetaHHDlg
{
     Q_OBJECT

  public:
     AlphaBetaHHDlg(int, QWidget *parent= 0);
     void exportData(abHHData &);
     void importData(abHHData);
     
  public slots:
    void updateOutChn(int, int*);
    void updateInChn(int, int*);

  private:
    CurrentAssignmentDlg ca;
   
}; 

#endif
