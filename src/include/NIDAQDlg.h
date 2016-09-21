#ifndef NIDAQDLG_H
#define NIDAQDLG_H


#include "ui_NIDAQDlg.h"
#include "ObjectDataTypes.h"
#include "DAQDlg.h"
#include "Global.h"
#include "ChannelListModel.h"

class NIDAQDlg : public DAQDlg, private Ui::NIDAQDlg
{
     Q_OBJECT

  public:
     NIDAQDlg(int no, QWidget *parent= 0);
     bool exportData(bool forceInit = false);
     void importData();

     typedef NIDAQData param_type;
     
  public slots:
    void accept();
    void reject();
    void open();
    
private slots:
    void on_inChannels_clicked();
    void on_outChannels_clicked();

protected:
    DeviceStatus initDAQ();

    NIDAQData backup;
};

#endif
