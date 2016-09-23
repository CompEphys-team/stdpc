#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTime>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include "Global.h"
#include "ui_MainWin.h"
#include "SpikeTimeDlg.h"
#include "DataSavingDlg.h"
#include "ObjectDataTypes.h"

#include "DCThread.h"
#include "Graph.h"
#include "GraphDlg.h"



class MyMainWindow : public QMainWindow, private Ui::MainWindow
{
     Q_OBJECT

  protected:
    void closeEvent(QCloseEvent *event);
    void doExportLog(QString &);

  public:
     MyMainWindow(QWidget *parent= 0);
     virtual ~MyMainWindow();
     void exportData(bool ignoreDAQ = false);
     void importData();
     void exportSGData();
     void importSGData();

     DataSavingDlg *DSDlg;
     SpikeTimeDlg *SpkTDlg;
     GraphDlg *graphDlg[2];
     DCThread *DCT;

     ChannelListModel *inChnModel, *outChnModel, *SGbdChannelModel;

     Graph Graphs[2];
     
  public slots:
     void CloseToLimitWarning(QString, QString, double, double, double);
     void DisplayAbout();
     void DisplayMessage(QString);
     void updateDeviceStatus(DeviceStatus = DeviceStatus::Inactive, const QString & = QString());

  signals:
     void channelsChanged();
                 
  private slots:
     void SGMethodChanged();

     void StartButClicked();
     void StopButClicked();
     
     void SaveConfig();
     void LoadConfig();
     void ExportLog();
     void ClearLog();
     void SaveProtocol();
     void LoadProtocol();
     void LoadScript();
     void UnLoadScript();
          
 private:
    QFileDialog *ExportLogFileDlg;
    QFileDialog *LoadProtocolFileDlg;
    QFileDialog *SaveProtocolFileDlg;
    QFileDialog *LoadScriptFileDlg;    
    
    // adjustable parameter stuff

    void doLoadProtocol(QString &);
    void doSaveProtocol(QString &);

}; 

#endif
