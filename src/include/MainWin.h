#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTime>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include "Global.h"
#include "ui_MainWin.h"
#include "InputChannelDlg.h"
#include "OutputChannelDlg.h"
#include "SpikeTimeDlg.h"
#include "DigiDataDlg.h"
#include "SimulDAQDlg.h"
#include "ElectrodeCompDlg.h"
#include "DataSavingDlg.h"
#include "AECChannel.h"
#include "HHModelDlg.h"


#ifdef NATIONAL_INSTRUMENTS
#include "NIDAQDlg.h" 
#endif

#include "ObjectDataTypes.h"
#include "SimulDAQ.h"

#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
#endif

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
     void exportData();
     void importData();
     void exportSGData();
     void importSGData();
 
     InputChannelDlg *inChnDlg;
     OutputChannelDlg *outChnDlg;

     ElectrodeCompDlg *ECDlg;
     DataSavingDlg *DSDlg;
     SpikeTimeDlg *SpkTDlg;
     GraphDlg *graphDlg[2];
     DCThread *DCT;
     int DAQtype;
     QString DAQName;
     DAQ *board;
     DigiDataDlg *DDataDlg;
     SimulDAQDlg *SDAQDlg;
     HHModelDlg *hhModelDlg;

     ChannelListModel *inChnModel, *outChnModel, *SGbdChannelModel;

#ifdef NATIONAL_INSTRUMENTS     
     NIDAQDlg *NDQDlg;
#endif
     
     DAQDlg *theDAQDlg;
     Graph Graphs[2];
     
  public slots:
     void DAQSetup();
     void CloseToLimitWarning(QString, int, double, double, double);
     void DisplayAbout();

  signals:
     void channelsChanged();
                 
  private slots:
     void SGMethodChanged();

     void StartButClicked();
     void StopButClicked();
     
     void DisplayMessage(QString);
     void showDAQDlg();
     
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
