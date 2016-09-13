#include "MainWin.h"
#include "LUtables.h"
#include "AP.h"
#include <windows.h>
#include "ComponentTable.h"

void MyMainWindow::DAQSetup()
{
  bool success;
  QMessageBox LoadMsg;
     
  DAQtype= DAQComboBox->currentIndex();
  DAQName= DAQComboBox->currentText();
  DDataDlg->exportData(DigiDatap);
  DigiDatap.active = false;
  SDAQDlg->exportData(SDAQp);
  SDAQp.active = false;
#ifdef NATIONAL_INSTRUMENTS 
  NDQDlg->exportData(NIDAQp);
  NIDAQp.active = false;
#endif

  if (!DCT->finished) {  
    DisplayMessage("Waiting for running Dynamic Clamp Thread to finish");
    while (!DCT->finished) { Sleep(1000);}
  }

  std::vector<inChnData> *inVec;
  std::vector<outChnData> *outVec;
  switch (DAQtype) {
  case 0:
  default:
    SDAQp.active = true;
    inVec = &SDAQp.inChn;
    outVec = &SDAQp.outChn;
    theDAQDlg= SDAQDlg;
    break;
  case 1:
    DigiDatap.active = true;
    inVec = &DigiDatap.inChn;
    outVec = &DigiDatap.outChn;
    theDAQDlg= DDataDlg;
    break;
#ifdef NATIONAL_INSTRUMENTS
  case 2:
    NIDAQp.active = true;
    inVec = &NIDAQp.inChn;
    outVec = &NIDAQp.outChn;
    theDAQDlg= NDQDlg;
    break;
#endif
  }
  LoadMsg.setIcon(QMessageBox::Information);
  LoadMsg.setWindowTitle(tr("StdpC 2012"));
  LoadMsg.setText(tr("Initializing hardware ... this may take a while ..."));
  LoadMsg.show();

  success = DCT->init();
  StartBut->setEnabled(success);
  actionElectrode_setup->setEnabled(success);

  // Stop-gap
  actionInput_channels->setEnabled(success);
  actionOutput_channels->setEnabled(success);
  if ( success ) {
      inVec->resize(DCT->clk.board[0]->inChnNo);
      outVec->resize(DCT->clk.board[0]->outChnNo);
      inChnDlg->init(DCT->clk.board[0]);
      outChnDlg->init(DCT->clk.board[0]);
  } else {
      emit channelsChanged();
  }

  LoadMsg.hide();
}

MyMainWindow::MyMainWindow(QWidget *parent)
     : QMainWindow(parent)
 {
     setupUi(this);

     inChnModel = new ChannelListModel(ChannelListModel::In | ChannelListModel::Blank, this);
     outChnModel = new ChannelListModel(ChannelListModel::Out | ChannelListModel::Blank, this);

     DDataDlg= new DigiDataDlg(this);
     SDAQDlg= new SimulDAQDlg(this);
#ifdef NATIONAL_INSTRUMENTS
     NDQDlg= new NIDAQDlg(this);
     DAQComboBox->addItem(QString("National Instruments"));
#endif
     ECDlg= new ElectrodeCompDlg(this);
     DSDlg= new DataSavingDlg(this);
     inChnDlg= new InputChannelDlg(this);
     outChnDlg= new OutputChannelDlg(this);
     SpkTDlg= new SpikeTimeDlg;
     graphDlg[0]= new GraphDlg(0, this);
     graphDlg[1]= new GraphDlg(1, this);
     hhModelDlg = new HHModelDlg(this);

     QVector<GenericComponent *> prototypes;
     prototypes.push_back(new Component<HHDlg>("m/h/tau HH", &mhHHp));
     prototypes.push_back(new Component<AlphaBetaHHDlg>("a/b HH", &abHHp));
     currentTable->init(prototypes, inChnModel, outChnModel);

     prototypes.clear();
     prototypes.push_back(new Component<ChemSynDlg>("ChemSyn", &CSynp));
     prototypes.push_back(new Component<abSynDlg>("a/b Syn", &abSynp));
     prototypes.push_back(new Component<GapJunctionDlg>("Gap Junction", &ESynp));
     prototypes.push_back(new Component<DestexheSynDlg>("DestexheSyn", &DxheSynp));
     synapseTable->init(prototypes, inChnModel, outChnModel);
     
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

      // somewhat of a hack: Synchronous digital IO for DigiData boards
     DigiDatap.syncIOMask= 0x0000;  
         
     DCT= new DCThread();

     initAP();
     LoadConfig();

     SGbdChannelModel = new ChannelListModel(ChannelListModel::AnalogIn | ChannelListModel::Virtual, this);
     SGbdChannelModel->subordinate(SGbdChannelCombo);
     connect(SGMethodCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     connect(BurstDetectionCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     
     connect(SpikeTimesBut, SIGNAL(clicked()), SpkTDlg, SLOT(show()));
     connect(StartBut, SIGNAL(clicked()), SLOT(StartButClicked()));
     connect(StopBut, SIGNAL(clicked()), SLOT(StopButClicked()));
     
     connect(actionExit, SIGNAL(triggered()), SLOT(close()));
     connect(actionDAQ, SIGNAL(triggered()), SLOT(showDAQDlg()));
     connect(actionInput_channels, SIGNAL(triggered()), inChnDlg, SLOT(open()));
     connect(actionOutput_channels, SIGNAL(triggered()), outChnDlg, SLOT(open()));
     connect(actionElectrode_setup, SIGNAL(triggered()), ECDlg, SLOT(open()));
     connect(actionData_saving, SIGNAL(triggered()), DSDlg, SLOT(open()));
     connect(actionHH_models, SIGNAL(triggered()), hhModelDlg, SLOT(open()));
     connect(actionSave_config, SIGNAL(triggered()), SLOT(SaveConfig()));
     connect(actionExport_Log, SIGNAL(triggered()), ExportLogFileDlg, SLOT(show()));
     connect(ExportLogFileDlg, SIGNAL(accepted()), SLOT(ExportLog()));
     connect(actionClear_Log, SIGNAL(triggered()), SLOT(ClearLog()));
     connect(actionLoad_Protocol, SIGNAL(triggered()), LoadProtocolFileDlg, SLOT(show()));
     connect(LoadProtocolFileDlg, SIGNAL(accepted()), SLOT(LoadProtocol()));
     connect(actionSave_Protocol, SIGNAL(triggered()), SaveProtocolFileDlg, SLOT(show()));
     connect(SaveProtocolFileDlg, SIGNAL(accepted()), SLOT(SaveProtocol()));
     connect(actionLoad_Script, SIGNAL(triggered()), LoadScriptFileDlg, SLOT(show()));
     connect(LoadScriptFileDlg, SIGNAL(accepted()), SLOT(LoadScript()));
     connect(actionUnload_Script, SIGNAL(triggered()), SLOT(UnLoadScript()));    
     connect(actionAbout, SIGNAL(triggered()), this, SLOT(DisplayAbout()));
     connect(this, SIGNAL(destroyed()), SLOT(close()));
     
     connect(outChnDlg, SIGNAL(accepted()), this, SIGNAL(channelsChanged()));
     connect(inChnDlg, SIGNAL(accepted()), this, SIGNAL(channelsChanged()));
     connect(hhModelDlg, SIGNAL(accepted()), this, SIGNAL(channelsChanged()));

     connect(this, SIGNAL(channelsChanged()), inChnModel, SLOT(updateChns()));
     connect(this, SIGNAL(channelsChanged()), outChnModel, SLOT(updateChns()));
     connect(this, SIGNAL(channelsChanged()), SGbdChannelModel, SLOT(updateChns()));

     connect(DAQComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(DAQSetup()));
     connect(DDataDlg, SIGNAL(reinitDAQ()), SLOT(DAQSetup()));
     connect(SDAQDlg, SIGNAL(reinitDAQ()), SLOT(DAQSetup()));
#ifdef NATIONAL_INSTRUMENTS
     connect(NDQDlg, SIGNAL(reinitDAQ()), SLOT(DAQSetup()));
#endif
     
     connect(Graph1SetBut, SIGNAL(clicked()),graphDlg[0], SLOT(show()));
     connect(Graph2SetBut, SIGNAL(clicked()),graphDlg[1], SLOT(show()));

   
     connect(DCT,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(&DCT->SG,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(DCT,SIGNAL(addPoint1(double, double, int)), &Graphs[0], SLOT(addPoint(double, double, int)));
     connect(DCT,SIGNAL(addPoint2(double, double, int)), &Graphs[1], SLOT(addPoint(double, double, int)));
     
     connect(ECDlg,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));

     connect(ECDlg->calibrator,SIGNAL(CloseToLimit(QString, QString, double, double, double)),SLOT(CloseToLimitWarning(QString, QString, double, double, double)));
     connect(DCT,SIGNAL(CloseToLimit(QString, QString, double, double, double)),SLOT(CloseToLimitWarning(QString, QString, double, double, double)));
     
     // graphical stuff
     DataD1->setScene(&Graphs[0].Scene);
     DataD2->setScene(&Graphs[1].Scene);   

     DAQSetup();
     
     DisplayMessage(QString("Main: Dynamic Clamp starting ..."));
     exportData();
     for (int i= 0; i < 2; i++) Graphs[i].init(&Graphp[i]);
 } 

MyMainWindow::~MyMainWindow()
{
  delete inChnDlg;
  delete outChnDlg;
  delete DDataDlg;
  delete SDAQDlg;
  delete hhModelDlg;

  delete inChnModel;
  delete outChnModel;

#ifdef NATIONAL_INSTRUMENTS
  delete NDQDlg;
#endif
  
  delete ExportLogFileDlg;
  delete LoadProtocolFileDlg;
  delete SaveProtocolFileDlg;
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
  MessageWindow->addItem(tstamp.toString()+QString(": ")+message);
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

  int index= SGMethodCombo->currentIndex();
  int index2= BurstDetectionCombo->currentIndex();

  SGSave->setEnabled(false);
  SGLUTableCombo->setEnabled(false);
  SpikesL->setEnabled(false);              
  VSpikeE->setEnabled(false); VSpikeL->setEnabled(false); VSpikeU->setEnabled(false);
  WidthE->setEnabled(false); WidthL->setEnabled(false); WidthU->setEnabled(false);
  VRestE->setEnabled(false); VRestL->setEnabled(false); VRestU->setEnabled(false);
  VRestE->setEnabled(false); VRestL->setEnabled(false); VRestU->setEnabled(false);
  ExplicitSpikeTimesL->setEnabled(false);
  NumberE->setEnabled(false); NumberL->setEnabled(false); NumberU->setEnabled(false);
  PeriodE->setEnabled(false); PeriodL->setEnabled(false); PeriodU->setEnabled(false);
  SpikeTimesBut->setEnabled(false); 
  BurstDetectionL->setEnabled(false);
  BurstDetectionCombo->setEnabled(false);
  SGbdChannelCombo->setEnabled(false); ChannelL->setEnabled(false);
  ThresholdE->setEnabled(false); ThresholdL->setEnabled(false); ThresholdU->setEnabled(false);
  NUnderE->setEnabled(false); NUnderL->setEnabled(false); 
  NOverE->setEnabled(false); NOverL->setEnabled(false);
  STInputFileL->setEnabled(false); STInputFileE->setEnabled(false); 
  if (index == 0) SGSave->setChecked(false);
  if (index == 3) {
    SGSave->setEnabled(true); STInputFileL->setEnabled(true); STInputFileE->setEnabled(true);
  }
  else {
    if (index > 0) {
      SGSave->setEnabled(true);
      SGLUTableCombo->setEnabled(true);
      SpikesL->setEnabled(true);              
      VSpikeE->setEnabled(true); VSpikeL->setEnabled(true); VSpikeU->setEnabled(true);
      WidthE->setEnabled(true); WidthL->setEnabled(true); WidthU->setEnabled(true);
      VRestE->setEnabled(true); VRestL->setEnabled(true); VRestU->setEnabled(true);
      BurstDetectionL->setEnabled(true);
      BurstDetectionCombo->setEnabled(true);
      if (index2 > 0) {
        SGbdChannelCombo->setEnabled(true); ChannelL->setEnabled(true); 
        ThresholdE->setEnabled(true); ThresholdL->setEnabled(true); ThresholdU->setEnabled(true);
        NUnderE->setEnabled(true); NUnderL->setEnabled(true);
        NOverE->setEnabled(true); NOverL->setEnabled(true);
      }
    }
    if (index == 1) {
      ExplicitSpikeTimesL->setEnabled(true);
      NumberE->setEnabled(true); NumberL->setEnabled(true); NumberU->setEnabled(true);
      PeriodE->setEnabled(true); PeriodL->setEnabled(true); PeriodU->setEnabled(true);
      SpikeTimesBut->setEnabled(true);
    }
    if (index == 2) {
      STInputFileL->setEnabled(true); STInputFileE->setEnabled(true);
    }
  }
}

void MyMainWindow::StartButClicked() 
{ 
  actionLoad_Protocol->setEnabled(false);
  actionSave_Protocol->setEnabled(false);
  actionLoad_Script->setEnabled(false);
  actionUnload_Script->setEnabled(false);
  actionExport_Log->setEnabled(false);
  actionExit->setEnabled(false);
  actionSave_config->setEnabled(false);
  menuConfig->setEnabled(false);
  DAQComboBox->setEnabled(false);
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
  actionLoad_Protocol->setEnabled(true);
  actionSave_Protocol->setEnabled(true);
  actionLoad_Script->setEnabled(!DCT->scripting);
  actionUnload_Script->setEnabled(DCT->scripting);
  actionExport_Log->setEnabled(true);
  actionExit->setEnabled(true);
  actionSave_config->setEnabled(true);
  menuConfig->setEnabled(true);
  DAQComboBox->setEnabled(true);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
}

void MyMainWindow::exportData()
{
  synapseTable->exportData();
  currentTable->exportData();
  
  inChnDlg->exportData();
  outChnDlg->exportData();
  DDataDlg->exportData(DigiDatap);
  SDAQDlg->exportData(SDAQp);
#ifdef NATIONAL_INSTRUMENTS
  NDQDlg->exportData(NIDAQp);
#endif
  exportSGData();
  SpkTDlg->exportData();
  for (int i= 0; i < 2; i++) graphDlg[i]->exportData(Graphp[i]);

  DSDlg->exportData();
  ECDlg->exportData();

  hhModelDlg->exportData();
}
 
void MyMainWindow::importData()
{
  // do DAQs and channels first
  DDataDlg->importData(DigiDatap);
  SDAQDlg->importData(SDAQp);
#ifdef NATIONAL_INSTRUMENTS
  NDQDlg->importData(NIDAQp);
#endif

  inChnDlg->importData();
  inChnDlg->accept();
  outChnDlg->importData();
  outChnDlg->accept();

  synapseTable->importData();
  currentTable->importData();
  importSGData();
  SpkTDlg->importData();
  for (int i= 0; i < 2; i++) graphDlg[i]->importData(Graphp[i]);

  DSDlg->importData();
  ECDlg->importData();

  hhModelDlg->importData();
}

void MyMainWindow::exportSGData() 
{
  SGp.method= SGMethodCombo->currentIndex();
  SGp.active= (SGp.method > 0);
  SGp.LUTables= SGLUTableCombo->currentIndex();
  if (SGp.active) SGp.saving= (SGSave->checkState() > 0);
  else SGp.saving= false;
  SGp.VSpike= VSpikeE->text().toDouble()/1e3;
  SGp.spkTimeScaling= 5e3/WidthE->text().toDouble();
  SGp.VRest= VRestE->text().toDouble()/1e3;
  
  SGp.bdType= BurstDetectionCombo->currentIndex();
  SGp.bdChannel= SGbdChannelCombo->currentData().value<ChannelIndex>();
  SGp.bdThresh= ThresholdE->text().toDouble()/1e3;
  SGp.bdNUnder= NUnderE->text().toInt();
  SGp.bdNOver= NOverE->text().toInt();
  
  SGp.period= PeriodE->text().toDouble()/1e3;
  SGp.SpikeNo= NumberE->text().toInt();
  
  SGp.STInFName= STInputFileE->text();
}

void MyMainWindow::importSGData() 
{
  QString num;
  SGMethodCombo->setCurrentIndex(SGp.method);
  SGSave->setChecked(SGp.saving);
  SGLUTableCombo->setCurrentIndex(SGp.LUTables);
  num.setNum(SGp.VSpike*1e3);
  VSpikeE->setText(num);
  num.setNum(5e3/SGp.spkTimeScaling);
  WidthE->setText(num);
  num.setNum(SGp.VRest*1e3);
  VRestE->setText(num);
  
  BurstDetectionCombo->setCurrentIndex(SGp.bdType);
  SGbdChannelCombo->setCurrentIndex(SGbdChannelModel->index(SGp.bdChannel));
  num.setNum(SGp.bdThresh*1e3);
  ThresholdE->setText(num);
  num.setNum(SGp.bdNUnder);
  NUnderE->setText(num);
  num.setNum(SGp.bdNOver);
  NOverE->setText(num);
  
  num.setNum(SGp.period*1e3);
  PeriodE->setText(num);
  num.setNum(SGp.SpikeNo);
  NumberE->setText(num);
  
  STInputFileE->setText(SGp.STInFName);
}

void MyMainWindow::showDAQDlg()
{
  theDAQDlg->open();
}

void MyMainWindow::SaveConfig()
{
  ofstream os("StdpC.conf");
  
  os << DAQComboBox->currentIndex() << endl;
  os << SDAQp << endl;
  os << DigiDatap << endl;
  os.close();
}

void MyMainWindow::LoadConfig()
{
  int itmp;
  ifstream is("StdpC.conf");
  SDAQData tmpSDAQp;
  DigiDataData tmpDigiDatap;

  
  if (is.good()) {
    is >> itmp;
    is >> tmpSDAQp;
    is >> tmpDigiDatap;
  }
  if (is.good())
  {
    is.close();
    DAQComboBox->setCurrentIndex(itmp);
    SDAQp= tmpSDAQp;
    SDAQDlg->importData(SDAQp);
    DigiDatap= tmpDigiDatap;
    DDataDlg->importData(DigiDatap);
  }
  else {
    DisplayMessage(QString("No valid config file found; reverting to standard settings"));
  }

  // special Sample-and-Hold for Attila
  SampleHoldp.active= false;
  SampleHoldp.threshV= 0.0;
}

void MyMainWindow::doSaveProtocol(QString &fname)
{
  ofstream os(fname.toLatin1());

  exportData();

  os << DAQComboBox->currentIndex() << endl;
  os << SDAQp << endl;
  os << DigiDatap << endl;
#ifdef NATIONAL_INSTRUMENTS
  os << NIDAQp << endl;
#endif
  os << endl;

  for ( auto const& ap : params ) {
      ap->write(os);
  }

  os.close();
}


void MyMainWindow::doLoadProtocol(QString &fname)
{
  ifstream is(fname.toLatin1());
  char name[80];
  int itmp;

  if (!is.good()) {
    DisplayMessage(QString("Error opening Protocol file"));
    return;
  }

  is >> itmp;
  is >> SDAQp;
  is >> DigiDatap;
#ifdef NATIONAL_INSTRUMENTS
  is >> NIDAQp;
#endif
  if (!is.good()) {
    DisplayMessage(QString("Protocol file truncated ... error"));
    return;
  }
  SDAQDlg->importData(SDAQp);
  DDataDlg->importData(DigiDatap);
#ifdef NATIONAL_INSTRUMENTS
  NDQDlg->importData(NIDAQp);
#endif
  DAQComboBox->setCurrentIndex(itmp);

  // comment this?!?
  DAQSetup();

  // Clear params before loading
  CSynp.clear();
  abSynp.clear();
  ESynp.clear();
  DxheSynp.clear();
  mhHHp.clear();
  abHHp.clear();
  HHNeuronp.clear();

  is >> name;
  while ( is.good() ) {
    QString rawname(name);
    bool ok = false;
    auto const& it = AP::find(rawname);
    if ( it != *params.end() )
      it->readNow(rawname, is, &ok);
    if ( !ok )
      DisplayMessage(QString("Warning: Failed to read parameter \"%1\"").arg(name));
    is >> name;
  }

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
    it= MessageWindow->item(i);
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
  MessageWindow->clear();
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
    actionLoad_Script->setEnabled(false);
    actionUnload_Script->setEnabled(true);
  }
}

void MyMainWindow::UnLoadScript()
{
  DCT->UnloadScript();
  actionLoad_Script->setEnabled(true);
  actionUnload_Script->setEnabled(false);
}

void MyMainWindow::DisplayAbout()
{
    QMessageBox::information(this,tr("About StdpC"),tr("StdpC is free dynamic clamp software including plasticity of synapses and active electrode compensation. \n It is distributed under the GPL v2 license. \n You are running version StdpC 2016 RC1."));
}
