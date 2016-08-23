#ifndef GAPJUNCTIONDLG_H
#define GAPJUNCTIONDLG_H


#include "ui_GapJunctionDlg.h"
#include "ObjectDataTypes.h"
#include "GapJunctionAssignmentDlg.h"

class GapJunctionDlg : public QDialog, private Ui::GapJunctionDlg
{
     Q_OBJECT

  public:
     GapJunctionDlg(int, QWidget *parent= 0);
     void exportData(GJunctData &);
     void importData(GJunctData);
     
     int No;
     
  public slots:
    void updateOutChn(int, int*);
    void updateInChn(int, int*);

private:
    GapJunctionAssignmentDlg gja;
}; 

#endif
