#ifndef SPIKETIMEDLG_H
#define SPIKETIMEDLG_H


#include <QAbstractButton>
#include "ui_SpikeTimeDlg.h"
#include "ObjectDataTypes.h"
#include "Global.h"

class SpikeTimeDlg : public QDialog, private Ui::SpikeTimeDlg
{
     Q_OBJECT

  public:
     SpikeTimeDlg(QWidget *parent= 0);
     void exportData();
     void importData();

  public slots:
     
  private:
}; 

#include "MainWin.h"

#endif
