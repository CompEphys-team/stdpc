#ifndef HHDLG_H
#define HHDLG_H

#include "ui_HHDlg.h"
#include "ObjectDataTypes.h"
#include "CurrentAssignmentDlg.h"

class HHDlg : public QDialog, private Ui::HHDlg
{
     Q_OBJECT

  public:
     HHDlg(int, QWidget *parent= 0);
     void exportData(mhHHData &);
     void importData(mhHHData);

private:
    CurrentAssignmentDlg ca;
         
}; 

#endif
