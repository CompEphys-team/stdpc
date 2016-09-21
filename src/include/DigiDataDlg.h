#ifndef DIGIDATADLG_H
#define DIGIDATADLG_H

#include "ui_DigiDataDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"
#include "ChannelListModel.h"

class DigiDataDlg : public DAQDlg, private Ui::DigiDataDlg
{
     Q_OBJECT

  public:
     DigiDataDlg(int no, QWidget *parent= 0);
     bool exportData(bool forceInit = false);
     void importData();

     typedef DigiDataData param_type;
     
  public slots:
    void accept();
    void reject();
    void open();
    
private slots:
    void on_inChannels_clicked();
    void on_outChannels_clicked();

protected:
    DeviceStatus initDAQ();

    DigiDataData backup;
};

#endif
