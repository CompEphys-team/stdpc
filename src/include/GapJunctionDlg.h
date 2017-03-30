#ifndef GAPJUNCTIONDLG_H
#define GAPJUNCTIONDLG_H


#include "ui_GapJunctionDlg.h"
#include "ObjectDataTypes.h"

class GapJunctionDlg : public QDialog, private Ui::GapJunctionDlg
{
     Q_OBJECT

  public:
     GapJunctionDlg(int, QWidget *parent= 0);
     void exportData(GJunctData &);
     void importData(GJunctData);
     void setIndex(int);

     typedef GJunctData param_type;
     
     QString label;
}; 

#endif
