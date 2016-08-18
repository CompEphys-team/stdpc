#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMessageBox>
#include <QTime>
#include <QMainWindow>
#include <QCloseEvent>
#include <QFileDialog>
#include "Main.h"
#include "Global.h"
#include "ui_mainwin.h"
#include "InputChannelDlg.h"
#include "OutputChannelDlg.h"
#include "ChemSynDlg.h"
#include "AbSynDlg.h"
#include "GapJunctionDlg.h"
#include "DestexheSynDlg.h"
#include "HHDlg.h"
#include "AlphaBetaHHDlg.h"
#include "SpikeTimeDlg.h"
#include "DigiDataDlg.h"
#include "SimulDAQDlg.h"
#include "ElectrodeCompDlg.h"
#include "DataSavingDlg.h"
#include "AECChannel.h"


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
     ChemSynDlg *CSynDlg[MAX_SYN_NO];
     abSynDlg *abSDlg[MAX_SYN_NO];
     GapJunctionDlg *GJunctDlg[MAX_SYN_NO];
     DestexheSynDlg *DxheSynDlg[MAX_SYN_NO];
     HHDlg *nHHDlg[MAX_HH_NO];
     AlphaBetaHHDlg *abHHDlg[MAX_HH_NO];
     SpikeTimeDlg *SpkTDlg;
     GraphDlg *graphDlg[2];
     DCThread *DCT;
     int DAQtype;
     QString DAQName;
     DAQ *board;
     DigiDataDlg *DDataDlg;
     SimulDAQDlg *SDAQDlg;

#ifdef NATIONAL_INSTRUMENTS     
     NIDAQDlg *NDQDlg;
#endif
     
     DAQDlg *theDAQDlg;
     Graph Graphs[2];
     
  public slots:
     void DAQSetup();
     void updateSGInChn(int, int*);
     void CloseToLimitWarning(QString, int, double, double, double);
     void DisplayAbout();
                 
  private slots:
     void SGMethodChanged();
     void Syn0ComboChanged();
     void Syn1ComboChanged();
     void Syn2ComboChanged();
     void Syn3ComboChanged();
     void Syn4ComboChanged();
     void Syn5ComboChanged();

     void HH0ComboChanged();
     void HH1ComboChanged();
     void HH2ComboChanged();
     void HH3ComboChanged();
     void HH4ComboChanged();
     void HH5ComboChanged();

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
