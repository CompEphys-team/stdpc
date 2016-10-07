#include "MainWin.h"
#include "ui_MainWin.h"
#include "LUtables.h"
#include "AP.h"
#include <windows.h>
#include <QScrollBar>

MyMainWindow::MyMainWindow(QWidget *parent)
     : QMainWindow(parent),
       ui(new Ui::MainWindow)
 {
     ui->setupUi(this);

     inChnModel = new ChannelListModel(ChannelListModel::In | ChannelListModel::Blank, this);
     outChnModel = new ChannelListModel(ChannelListModel::Out | ChannelListModel::Blank, this);

     DSDlg= new DataSavingDlg(this);
     SpkTDlg= new SpikeTimeDlg;
     graphDlg[0]= new GraphDlg(0, this);
     graphDlg[1]= new GraphDlg(1, this);

     QVector<ComponentPrototypeBase *> prototypes;
     prototypes.push_back(new ComponentPrototype<HHDlg>("m/h/tau HH", &mhHHp));
     prototypes.push_back(new ComponentPrototype<AlphaBetaHHDlg>("a/b HH", &abHHp));
     ui->currentTable->init(prototypes, inChnModel, outChnModel);

     prototypes.clear();
     prototypes.push_back(new ComponentPrototype<ChemSynDlg>("ChemSyn", &CSynp));
     prototypes.push_back(new ComponentPrototype<abSynDlg>("a/b Syn", &abSynp));
     prototypes.push_back(new ComponentPrototype<GapJunctionDlg>("Gap Junction", &ESynp));
     prototypes.push_back(new ComponentPrototype<DestexheSynDlg>("DestexheSyn", &DxheSynp));
     ui->synapseTable->init(prototypes, inChnModel, outChnModel);

     QVector<DaqOptsPrototypeBase*> dprot;
     dprot.push_back(new DaqOptsPrototype<SimulDAQDlg>("SimulDAQ", &SDAQp));
     dprot.push_back(new DaqOptsPrototype<DigiDataDlg>("DigiData 1200(A)", &DigiDatap));
#ifdef NATIONAL_INSTRUMENTS
     dprot.push_back(new DaqOptsPrototype<NIDAQDlg>("Nat'l Instruments", &NIDAQp));
#endif
     dprot.push_back(new DaqOptsPrototype<HHModelDlg>("HH Model", &HHNeuronp));
     ui->DAQTable->init(dprot, this);
     
     ExportLogFileDlg= new QFileDialog(this, QString("Export Log File Dialog"), QString("."), 
               QString("*.log"));
     ExportLogFileDlg->setAcceptMode(QFileDialog::AcceptSave);
     
     LoadProtocolFileDlg= new QFileDialog(this, QString("Load Protocol File Dialog"), QString("."), 
               QString("*.cpr"));
     LoadProtocolFileDlg->setAcceptMode(QFileDialog::AcceptOpen);
     SaveProtocolFileDlg= new QFileDialog(this, QString("Save Protocol File Dialog"), QString("."), 
               QString("*.cpr"));
     SaveProtocolFileDlg->setAcceptMode(QFileDialog::AcceptSave);
     LoadScriptFileDlg= new QFileDialog(this, QString("Load Script File Dialog"), QString("."), 
               QString("*.scr"));
     LoadScriptFileDlg->setAcceptMode(QFileDialog::AcceptOpen);
         
     DCT= new DCThread();

     SGbdChannelModel = new ChannelListModel(ChannelListModel::AnalogIn | ChannelListModel::Virtual, this);
     ui->SGbdChannelCombo->setModel(SGbdChannelModel);
     connect(ui->SGMethodCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     connect(ui->BurstDetectionCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     
     connect(ui->SpikeTimesBut, SIGNAL(clicked()), SpkTDlg, SLOT(show()));
     connect(ui->StartBut, SIGNAL(clicked()), SLOT(StartButClicked()));
     connect(ui->StopBut, SIGNAL(clicked()), SLOT(StopButClicked()));
     
     connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
     connect(ui->actionData_saving, SIGNAL(triggered()), DSDlg, SLOT(open()));
     connect(ui->actionSave_config, SIGNAL(triggered()), SLOT(SaveConfig()));
     connect(ui->actionExport_Log, SIGNAL(triggered()), ExportLogFileDlg, SLOT(show()));
     connect(ExportLogFileDlg, SIGNAL(accepted()), SLOT(ExportLog()));
     connect(ui->actionClear_Log, SIGNAL(triggered()), SLOT(ClearLog()));
     connect(ui->actionLoad_Protocol, SIGNAL(triggered()), LoadProtocolFileDlg, SLOT(show()));
     connect(LoadProtocolFileDlg, SIGNAL(accepted()), SLOT(LoadProtocol()));
     connect(ui->actionSave_Protocol, SIGNAL(triggered()), SaveProtocolFileDlg, SLOT(show()));
     connect(SaveProtocolFileDlg, SIGNAL(accepted()), SLOT(SaveProtocol()));
     connect(ui->actionLoad_Script, SIGNAL(triggered()), LoadScriptFileDlg, SLOT(show()));
     connect(LoadScriptFileDlg, SIGNAL(accepted()), SLOT(LoadScript()));
     connect(ui->actionUnload_Script, SIGNAL(triggered()), SLOT(UnLoadScript()));
     connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(DisplayAbout()));
     connect(this, SIGNAL(destroyed()), SLOT(close()));

     connect(this, SIGNAL(channelsChanged()), inChnModel, SLOT(updateChns()));
     connect(this, SIGNAL(channelsChanged()), outChnModel, SLOT(updateChns()));
     connect(this, SIGNAL(channelsChanged()), SGbdChannelModel, SLOT(updateChns()));
     connect(this, SIGNAL(modelRemoved(ChannelIndex)), inChnModel, SLOT(updateChns(ChannelIndex)));
     connect(this, SIGNAL(modelRemoved(ChannelIndex)), outChnModel, SLOT(updateChns(ChannelIndex)));
     connect(this, SIGNAL(modelRemoved(ChannelIndex)), SGbdChannelModel, SLOT(updateChns(ChannelIndex)));
     
     connect(ui->Graph1SetBut, SIGNAL(clicked()),graphDlg[0], SLOT(show()));
     connect(ui->Graph2SetBut, SIGNAL(clicked()),graphDlg[1], SLOT(show()));

     connect(ui->HHActivate, SIGNAL(clicked(bool)), ui->currentTable, SLOT(activateAll()));
     connect(ui->HHDeactivate, SIGNAL(clicked(bool)), ui->currentTable, SLOT(deactivateAll()));
     connect(ui->HHClear, &QPushButton::clicked, [=](){ui->currentTable->importData(true);});
     connect(ui->HHReset, SIGNAL(clicked(bool)), ui->currentTable, SLOT(importData()));

     connect(ui->SynActivate, SIGNAL(clicked(bool)), ui->synapseTable, SLOT(activateAll()));
     connect(ui->SynDeactivate, SIGNAL(clicked(bool)), ui->synapseTable, SLOT(deactivateAll()));
     connect(ui->SynClear, &QPushButton::clicked, [=](){ui->synapseTable->importData(true);});
     connect(ui->SynReset, SIGNAL(clicked(bool)), ui->synapseTable, SLOT(importData()));

     connect(ui->DAQClear, &QPushButton::clicked, [=](){ui->DAQTable->importData(true);});
     connect(ui->DAQReset, SIGNAL(clicked(bool)), ui->DAQTable, SLOT(importData()));
   
     connect(DCT,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(&DCT->SG,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(DCT,SIGNAL(addPoint1(double, double, int)), &Graphs[0], SLOT(addPoint(double, double, int)));
     connect(DCT,SIGNAL(addPoint2(double, double, int)), &Graphs[1], SLOT(addPoint(double, double, int)));
     
     connect(DCT,SIGNAL(CloseToLimit(QString, QString, double, double, double)),SLOT(CloseToLimitWarning(QString, QString, double, double, double)));
     
     // graphical stuff
     ui->DataD1->setScene(&Graphs[0].Scene);
     ui->DataD2->setScene(&Graphs[1].Scene);

     initAP();
     LoadConfig();

     DisplayMessage(QString("Main: Dynamic Clamp starting ..."));
     exportData(true);
     updateDeviceStatus();
     for (int i= 0; i < 2; i++) Graphs[i].init(&Graphp[i]);
 } 

MyMainWindow::~MyMainWindow()
{
  delete inChnModel;
  delete outChnModel;
  
  delete ExportLogFileDlg;
  delete LoadProtocolFileDlg;
  delete SaveProtocolFileDlg;

  delete ui;
}

void MyMainWindow::CloseToLimitWarning(QString what, QString channelName, double lowLimit, double highLimit, double value)
{
   QString msg = what+QString(" is close to channel limit!\nChannel: ")+channelName+QString("\nHigh limit: ")+QString::number(highLimit)+QString("\nValue: ")+QString::number(value)+QString("\nLow limit: ")+QString::number(lowLimit);
   //QMessageBox::warning(this, tr("Warning"), msg);
   DisplayMessage(msg);
   return;
}


void MyMainWindow::DisplayMessage(QString message)
{
  QTime tstamp= QTime::currentTime();
  QScrollBar *sb = ui->MessageWindow->verticalScrollBar();
  bool down = sb->value() == sb->maximum();
  ui->MessageWindow->addItem(tstamp.toString()+QString(": ")+message);
  if ( down )
    ui->MessageWindow->scrollToBottom();
}

void MyMainWindow::updateDeviceStatus(DeviceStatus status, const QString &name)
{
    switch ( status ) {
    case DeviceStatus::Active :
        DisplayMessage(QString("Good news: %1 found and opened successfully!").arg(name));
        break;
    case DeviceStatus::Failed :
        DisplayMessage(QString("Bad news: %1 not found or not opened successfully!").arg(name));
        break;
    case DeviceStatus::Inactive :
    default:
        break;
    }

    emit channelsChanged();
    updateStartButton();
}

void MyMainWindow::updateStartButton()
{
    bool success = !Devices.actdev.empty();
    if ( !success && HHNeuronp.size() ) {
        for ( HHNeuronData const& p : HHNeuronp ) {
            if ( p.active && p.inst.size() ) {
                for ( vInstData const& inst : p.inst ) {
                    if ( inst.active ) {
                        success = true;
                        break;
                    }
                }
            }
            if ( success )
                break;
        }
    }
    ui->StartBut->setEnabled(success);
}


void MyMainWindow::closeEvent(QCloseEvent *event)
{
  QString fname;
  if (DCT) {
    //DCT->terminate();
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
    while (!DCT->finished) Sleep(100);
    delete DCT;
  }
  SaveConfig();
  fname= QString("StdpC_last.log");
  doExportLog(fname);
  event->accept();
}

void MyMainWindow::SGMethodChanged() 
{
//  QMessageBox::warning(this, tr("My Application"),
//                tr("The signal for index change has been received"),
//               QMessageBox::Close); 

  int index= ui->SGMethodCombo->currentIndex();
  int index2= ui->BurstDetectionCombo->currentIndex();

  ui->SGSave->setEnabled(false);
  ui->SGLUTableCombo->setEnabled(false);
  ui->SpikesL->setEnabled(false);
  ui->VSpikeE->setEnabled(false); ui->VSpikeL->setEnabled(false); ui->VSpikeU->setEnabled(false);
  ui->WidthE->setEnabled(false); ui->WidthL->setEnabled(false); ui->WidthU->setEnabled(false);
  ui->VRestE->setEnabled(false); ui->VRestL->setEnabled(false); ui->VRestU->setEnabled(false);
  ui->VRestE->setEnabled(false); ui->VRestL->setEnabled(false); ui->VRestU->setEnabled(false);
  ui->ExplicitSpikeTimesL->setEnabled(false);
  ui->NumberE->setEnabled(false); ui->NumberL->setEnabled(false); ui->NumberU->setEnabled(false);
  ui->PeriodE->setEnabled(false); ui->PeriodL->setEnabled(false); ui->PeriodU->setEnabled(false);
  ui->SpikeTimesBut->setEnabled(false);
  ui->BurstDetectionL->setEnabled(false);
  ui->BurstDetectionCombo->setEnabled(false);
  ui->SGbdChannelCombo->setEnabled(false); ui->ChannelL->setEnabled(false);
  ui->ThresholdE->setEnabled(false); ui->ThresholdL->setEnabled(false); ui->ThresholdU->setEnabled(false);
  ui->NUnderE->setEnabled(false); ui->NUnderL->setEnabled(false);
  ui->NOverE->setEnabled(false); ui->NOverL->setEnabled(false);
  ui->STInputFileL->setEnabled(false); ui->STInputFileE->setEnabled(false);
  if (index == 0) ui->SGSave->setChecked(false);
  if (index == 3) {
    ui->SGSave->setEnabled(true); ui->STInputFileL->setEnabled(true); ui->STInputFileE->setEnabled(true);
  }
  else {
    if (index > 0) {
      ui->SGSave->setEnabled(true);
      ui->SGLUTableCombo->setEnabled(true);
      ui->SpikesL->setEnabled(true);
      ui->VSpikeE->setEnabled(true); ui->VSpikeL->setEnabled(true); ui->VSpikeU->setEnabled(true);
      ui->WidthE->setEnabled(true); ui->WidthL->setEnabled(true); ui->WidthU->setEnabled(true);
      ui->VRestE->setEnabled(true); ui->VRestL->setEnabled(true); ui->VRestU->setEnabled(true);
      ui->BurstDetectionL->setEnabled(true);
      ui->BurstDetectionCombo->setEnabled(true);
      if (index2 > 0) {
        ui->SGbdChannelCombo->setEnabled(true); ui->ChannelL->setEnabled(true);
        ui->ThresholdE->setEnabled(true); ui->ThresholdL->setEnabled(true); ui->ThresholdU->setEnabled(true);
        ui->NUnderE->setEnabled(true); ui->NUnderL->setEnabled(true);
        ui->NOverE->setEnabled(true); ui->NOverL->setEnabled(true);
      }
    }
    if (index == 1) {
      ui->ExplicitSpikeTimesL->setEnabled(true);
      ui->NumberE->setEnabled(true); ui->NumberL->setEnabled(true); ui->NumberU->setEnabled(true);
      ui->PeriodE->setEnabled(true); ui->PeriodL->setEnabled(true); ui->PeriodU->setEnabled(true);
      ui->SpikeTimesBut->setEnabled(true);
    }
    if (index == 2) {
      ui->STInputFileL->setEnabled(true); ui->STInputFileE->setEnabled(true);
    }
  }
}

void MyMainWindow::StartButClicked() 
{
  ui->actionLoad_Protocol->setEnabled(false);
  ui->actionSave_Protocol->setEnabled(false);
  ui->actionLoad_Script->setEnabled(false);
  ui->actionUnload_Script->setEnabled(false);
  ui->actionExport_Log->setEnabled(false);
  ui->actionExit->setEnabled(false);
  ui->actionSave_config->setEnabled(false);
  ui->menuConfig->setEnabled(false);
  ui->DAQTable->setEnabled(false);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
  while (!DCT->finished) Sleep(100);
  exportData();
  for (int i= 0; i < 2; i++) Graphs[i].init(&Graphp[i]);
//      QMessageBox::warning(this, tr("My Application"),
//                tr("init'ing outChnDlg!"),
//                QMessageBox::Ok); 

  DCT->start();
}

void MyMainWindow::StopButClicked()
{
  ui->actionLoad_Protocol->setEnabled(true);
  ui->actionSave_Protocol->setEnabled(true);
  ui->actionLoad_Script->setEnabled(!DCT->scripting);
  ui->actionUnload_Script->setEnabled(DCT->scripting);
  ui->actionExport_Log->setEnabled(true);
  ui->actionExit->setEnabled(true);
  ui->actionSave_config->setEnabled(true);
  ui->menuConfig->setEnabled(true);
  ui->DAQTable->setEnabled(true);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
}

void MyMainWindow::exportData(bool ignoreDAQ)
{
  ui->synapseTable->exportData();
  ui->currentTable->exportData();
  ui->DAQTable->exportData(ignoreDAQ);

  exportSGData();
  SpkTDlg->exportData();
  for (int i= 0; i < 2; i++) graphDlg[i]->exportData(Graphp[i]);

  DSDlg->exportData();
}
 
void MyMainWindow::importData()
{
  ui->DAQTable->importData();
  ui->synapseTable->importData();
  ui->currentTable->importData();
  importSGData();
  SpkTDlg->importData();
  for (int i= 0; i < 2; i++) graphDlg[i]->importData(Graphp[i]);

  DSDlg->importData();
}

void MyMainWindow::exportSGData() 
{
  SGp.method= ui->SGMethodCombo->currentIndex();
  SGp.active= (SGp.method > 0);
  SGp.LUTables= ui->SGLUTableCombo->currentIndex();
  if (SGp.active) SGp.saving= (ui->SGSave->checkState() > 0);
  else SGp.saving= false;
  SGp.VSpike= ui->VSpikeE->text().toDouble()/1e3;
  SGp.spkTimeScaling= 5e3/ui->WidthE->text().toDouble();
  SGp.VRest= ui->VRestE->text().toDouble()/1e3;
  
  SGp.bdType= ui->BurstDetectionCombo->currentIndex();
  SGp.bdChannel= ui->SGbdChannelCombo->currentData().value<ChannelIndex>();
  SGp.bdThresh= ui->ThresholdE->text().toDouble()/1e3;
  SGp.bdNUnder= ui->NUnderE->text().toInt();
  SGp.bdNOver= ui->NOverE->text().toInt();
  
  SGp.period= ui->PeriodE->text().toDouble()/1e3;
  SGp.SpikeNo= ui->NumberE->text().toInt();
  
  SGp.STInFName= ui->STInputFileE->text();
}

void MyMainWindow::importSGData() 
{
  QString num;
  ui->SGMethodCombo->setCurrentIndex(SGp.method);
  ui->SGSave->setChecked(SGp.saving);
  ui->SGLUTableCombo->setCurrentIndex(SGp.LUTables);
  num.setNum(SGp.VSpike*1e3);
  ui->VSpikeE->setText(num);
  num.setNum(5e3/SGp.spkTimeScaling);
  ui->WidthE->setText(num);
  num.setNum(SGp.VRest*1e3);
  ui->VRestE->setText(num);
  
  ui->BurstDetectionCombo->setCurrentIndex(SGp.bdType);
  ui->SGbdChannelCombo->setCurrentIndex(SGbdChannelModel->index(SGp.bdChannel));
  num.setNum(SGp.bdThresh*1e3);
  ui->ThresholdE->setText(num);
  num.setNum(SGp.bdNUnder);
  ui->NUnderE->setText(num);
  num.setNum(SGp.bdNOver);
  ui->NOverE->setText(num);
  
  num.setNum(SGp.period*1e3);
  ui->PeriodE->setText(num);
  num.setNum(SGp.SpikeNo);
  ui->NumberE->setText(num);
  
  ui->STInputFileE->setText(SGp.STInFName);
}

void MyMainWindow::SaveConfig()
{
  ofstream os("StdpC.conf");
  os << STDPC_PROTOCOL_HEADER << " " << STDPC_PROTOCOL_VERSION << endl << endl;

  QString D("DigiDatap"), S("SDAQp"), N("NIDAQp");
  for ( std::unique_ptr<AP> const& ap : params ) {
      if ( ap->name().startsWith(D) || ap->name().startsWith(S) || ap->name().startsWith(N) ) {
          ap->write(os);
      }
  }

  os.close();
}

void MyMainWindow::LoadConfig()
{
    ifstream is("StdpC.conf");
    if ( readProtocol(is) )
        ui->DAQTable->importData();
    else
        DisplayMessage(QString("No valid config file found; reverting to standard settings"));
    is.close();

    // special Sample-and-Hold for Attila
    SampleHoldp.active= false;
    SampleHoldp.threshV= 0.0;
}

void MyMainWindow::doSaveProtocol(QString &fname)
{
  ofstream os(fname.toLatin1());
  os << STDPC_PROTOCOL_HEADER << " " << STDPC_PROTOCOL_VERSION << endl << endl;

  exportData();

  for ( auto const& ap : params ) {
      ap->write(os);
  }

  os.close();
}


void MyMainWindow::doLoadProtocol(QString &fname)
{
  ifstream is(fname.toLatin1());
  if (!is.good()) {
    DisplayMessage(QString("Error opening Protocol file"));
    return;
  }

  // Clear params before loading
  CSynp.clear();
  abSynp.clear();
  ESynp.clear();
  DxheSynp.clear();
  mhHHp.clear();
  abHHp.clear();
  HHNeuronp.clear();
  SDAQp.clear();
  DigiDatap.clear();
#ifdef NATIONAL_INSTRUMENTS
  NIDAQp.clear();
#endif

  std::function<bool(QString)> callback = [=](QString name) {
      DisplayMessage(QString("Warning: Failed to read parameter \"%1\"").arg(name));
      return false;
  };
  readProtocol(is, &callback);
  is.close();

  importData();
}


void MyMainWindow::ExportLog()
{
  QStringList fnlist= ExportLogFileDlg->selectedFiles();
  doExportLog(*fnlist.begin());
}

void MyMainWindow::doExportLog(QString &fname)
{
  ofstream os(fname.toLatin1());
  int i= 0, done= 0;
  QListWidgetItem *it;
  
  while (!done) {
    it= ui->MessageWindow->item(i);
    if (it != NULL) {
      os << it->text().toStdString() << endl;
    }
    else done= 1;
    i++;
  }
  DisplayMessage(QString("Log exported to ")+fname);
}
    
void MyMainWindow::ClearLog()
{
  ui->MessageWindow->clear();
}

void MyMainWindow::LoadProtocol()
{
  QStringList fnlist= LoadProtocolFileDlg->selectedFiles();
  doLoadProtocol(*fnlist.begin());
}

void MyMainWindow::SaveProtocol()
{
  QStringList fnlist= SaveProtocolFileDlg->selectedFiles();
  doSaveProtocol(*fnlist.begin());
}

void MyMainWindow::LoadScript()
{
  QStringList fnlist= LoadScriptFileDlg->selectedFiles();
  if (DCT->LoadScript(*fnlist.begin())) {
    ui->actionLoad_Script->setEnabled(false);
    ui->actionUnload_Script->setEnabled(true);
  }
}

void MyMainWindow::UnLoadScript()
{
  DCT->UnloadScript();
  ui->actionLoad_Script->setEnabled(true);
  ui->actionUnload_Script->setEnabled(false);
}

void MyMainWindow::DisplayAbout()
{
    QMessageBox::information(this,tr("About StdpC"),tr("StdpC is free dynamic clamp software including plasticity of synapses and active electrode compensation. \n It is distributed under the GPL v2 license. \n You are running version StdpC 2016 RC1."));
}
