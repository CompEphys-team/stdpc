#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTime>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include "Global.h"
#include "DataSavingDlg.h"
#include "TriggerDlg.h"
#include "ObjectDataTypes.h"
#include "DeviceManager.h"
#include "DCThread.h"

namespace Ui {
class MainWindow;
}

class MyMainWindow : public QMainWindow
{
     Q_OBJECT

  protected:
    void closeEvent(QCloseEvent *event);

  public:
     MyMainWindow(QWidget *parent= nullptr);
     virtual ~MyMainWindow();
     void exportData(bool ignoreDAQ = false);
     void importData();

     DataSavingDlg *DSDlg;
     TriggerDlg *TrigDlg;
     DCThread *DCT;
     
  public slots:
     void CloseToLimitWarning(QString, QString, double, double, double);
     void DisplayAbout();
     void DisplayMessage(QString);
     void updateDeviceStatus(DeviceStatus = DeviceStatus::Inactive, const QString & = QString());
     void updateStartButton();

  signals:
     void channelsChanged();
     void modelRemoved(ChannelIndex);
                 
  private slots:
     void StartButClicked();
     void StopButClicked();
     void TabChanged(int idx);
     
     void SaveConfig();
     void LoadConfig();
     void ExportLog();
     void doExportLog(QString);
     void ClearLog();
     void SaveProtocol();
     void LoadProtocol();
     void LoadScript();
     void UnLoadScript();
          
 private:
    QLabel *loadedProtocolStatus;
    QLabel *rateIndicator;

    Ui::MainWindow *ui;

    void DoSaveProtocol(ofstream&);
    bool DoLoadProtocol(ifstream&);
}; 

#endif
