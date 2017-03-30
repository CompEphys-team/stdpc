#ifndef HHDLG_H
#define HHDLG_H

#include "ui_HHDlg.h"
#include "ObjectDataTypes.h"

class HHDlg : public QDialog, private Ui::HHDlg
{
     Q_OBJECT

  public:
     HHDlg(int, QWidget *parent= 0);
     void exportData(mhHHData &);
     void importData(mhHHData);
     void setIndex(int);
     typedef mhHHData param_type;

     QString label;
}; 

#endif
