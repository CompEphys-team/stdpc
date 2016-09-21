#ifndef SIMULDAQDLG_H
#define SIMULDAQDLG_H


#include "ui_SimulDAQDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"
#include "ChannelListModel.h"

class SimulDAQDlg : public DAQDlg, private Ui::SimulDAQDlg
{
     Q_OBJECT

  public:
     SimulDAQDlg(int no, QWidget *parent= 0);
     bool exportData(bool forceInit = false);
     void importData();

     typedef SDAQData param_type;
     
  public slots:
     void accept();
     void reject();
     void open();

private slots:
     void on_inChannels_clicked();
     void on_outChannels_clicked();

     void on_InputFileB_clicked();

     void on_OutputFileB_clicked();

protected:
    DeviceStatus initDAQ();

     SDAQData backup;
};

#endif
