
#include "mainwin.h"
#include "LUtables.h"
#include <windows.h>

void MyMainWindow::DAQSetup()
{
  QString name;
  bool success;
  QMessageBox LoadMsg;
     
  DAQtype= DAQComboBox->currentIndex();
  DAQName= DAQComboBox->currentText();
  DDataDlg->exportData(DigiDatap);
  SDAQDlg->exportData(SDAQp);
#ifdef NIDAQ 
  NDQDlg->exportData(NIDAQp);
#endif

  if (!DCT->finished) {  
    DisplayMessage("Waiting for running Dynamic Clamp Thread to finish");
    while (!DCT->finished) { Sleep(1000);}
  }
  if (board != NULL) delete board;
  switch (DAQtype) {
  case 0: 
    board= new SimulDAQ;
    theDAQDlg= SDAQDlg;;
    break;
  case 1:
    board= new DigiData;
    theDAQDlg= DDataDlg;
    break;
#ifdef NIDAQ
  case 2:
    board= new NIDAQ;
    theDAQDlg= NDQDlg;       
#endif
  }
  LoadMsg.setIcon(QMessageBox::Information);
  LoadMsg.setWindowTitle(tr("StdpC 2008"));
  LoadMsg.setText(tr("Initializing hardware ... this may take a while ..."));
  //LoadMsg.removeButton(LoadMsg.button(QMessageBox::Ok));
  LoadMsg.show();
 
  success= board->initialize_board(name);
  if (success) {
    DisplayMessage(QString("Good news: ")+name+QString(" found and opened successfully!"));
    StartBut->setEnabled(true);
  }
  else {
    DisplayMessage(QString("Bad news: ")+name+QString(" not found or not opened successfully!"));
    StartBut->setEnabled(false);
  }

  if (inChnp != NULL) delete[] inChnp;
  inChnp= new inChnData[board->inChnNo];
  inChnDlg->init(board); // already exports to inChnp ...

  if (outChnp != NULL) delete[] outChnp;
  outChnp= new outChnData[board->outChnNo];
  outChnDlg->init(board); // already exports to outChnp ...

  DCT->init(board);

  // set up the adjustable parameters that they reflect the right channel numbers ...     
  setupAP();
  
  LoadMsg.hide();
}

MyMainWindow::MyMainWindow(QWidget *parent)
     : QMainWindow(parent)
 {
     setupUi(this);
     DDataDlg= new DigiDataDlg(this);
     SDAQDlg= new SimulDAQDlg(this);
#ifdef NIDAQ
     NDQDlg= new NIDAQDlg(this);
     DAQComboBox->addItem(QString("National Instruments"));
#endif     
     inChnDlg= new InputChannelDlg(this);
     outChnDlg= new OutputChannelDlg(this);
     for (int i= 0; i < 6; i++) {
       CSynDlg[i]= new ChemSynDlg(i, this);
       GJunctDlg[i]= new GapJunctionDlg(i, this);
       nHHDlg[i]= new HHDlg(i, this);
       abHHDlg[i]= new AlphaBetaHHDlg(i, this);
     }
     SpkTDlg= new SpikeTimeDlg;
     graphDlg[0]= new GraphDlg(0, this);
     graphDlg[1]= new GraphDlg(1, this);
     
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
   
     inChnp= NULL;
     outChnp= NULL;

     LoadConfig();

     connect(SGMethodCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     connect(BurstDetectionCombo, SIGNAL(currentIndexChanged(QString)), SLOT(SGMethodChanged()));
     connect(Syn0Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn0ComboChanged()));
     connect(Syn1Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn1ComboChanged()));
     connect(Syn2Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn2ComboChanged()));
     connect(Syn3Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn3ComboChanged()));
     connect(Syn4Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn4ComboChanged()));
     connect(Syn5Combo, SIGNAL(currentIndexChanged(QString)), SLOT(Syn5ComboChanged()));
     connect(HH0Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH0ComboChanged()));
     connect(HH1Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH1ComboChanged()));
     connect(HH2Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH2ComboChanged()));
     connect(HH3Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH3ComboChanged()));
     connect(HH4Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH4ComboChanged()));
     connect(HH5Combo, SIGNAL(currentIndexChanged(QString)), SLOT(HH5ComboChanged()));
     
     connect(SpikeTimesBut, SIGNAL(clicked()), SpkTDlg, SLOT(show()));
     connect(StartBut, SIGNAL(clicked()), SLOT(StartButClicked()));
     connect(StopBut, SIGNAL(clicked()), SLOT(StopButClicked()));
     
     connect(actionExit, SIGNAL(triggered()), SLOT(close()));
     connect(actionDAQ, SIGNAL(triggered()), SLOT(showDAQDlg()));
     connect(actionInput_channels, SIGNAL(triggered()), inChnDlg, SLOT(appear()));
     connect(actionOutput_channels, SIGNAL(triggered()), outChnDlg, SLOT(appear()));
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
     connect(this, SIGNAL(destroyed()), SLOT(Exit()));
     
     for (int i= 0; i < 6; i++) {
       connect(outChnDlg, SIGNAL(updateOutChn(int, int*)), CSynDlg[i], SLOT(updateOutChn(int, int*)));
       connect(outChnDlg, SIGNAL(updateOutChn(int, int*)), GJunctDlg[i], SLOT(updateOutChn(int, int*)));
       connect(outChnDlg, SIGNAL(updateOutChn(int, int*)), nHHDlg[i], SLOT(updateOutChn(int, int*)));
       connect(outChnDlg, SIGNAL(updateOutChn(int, int*)), abHHDlg[i], SLOT(updateOutChn(int, int*)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), CSynDlg[i], SLOT(updateInChn(int, int*)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), GJunctDlg[i], SLOT(updateInChn(int, int*)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), nHHDlg[i], SLOT(updateInChn(int, int*)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), abHHDlg[i], SLOT(updateInChn(int, int*)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), SLOT(updateSGInChn(int, int*)));
     }
     
     for (int i= 0; i < 2; i++) {
       connect(outChnDlg, SIGNAL(updateOutChn(int, int*)), graphDlg[i], SLOT(updateOutChn(int, int *)));
       connect(inChnDlg, SIGNAL(updateInChn(int, int*)), graphDlg[i], SLOT(updateInChn(int, int *)));
     }
     connect(DAQComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(DAQSetup()));
     connect(DDataDlg, SIGNAL(reinitDAQ()), SLOT(DAQSetup()));
     connect(SDAQDlg, SIGNAL(reinitDAQ()), SLOT(DAQSetup()));
     
     connect(Graph1SetBut, SIGNAL(clicked()),graphDlg[0], SLOT(show()));
     connect(Graph2SetBut, SIGNAL(clicked()),graphDlg[1], SLOT(show()));

   
     connect(DCT,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(&DCT->SG,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(DCT,SIGNAL(addPoint1(double, double, int)), &Graphs[0], SLOT(addPoint(double, double, int)));
     connect(DCT,SIGNAL(addPoint2(double, double, int)), &Graphs[1], SLOT(addPoint(double, double, int)));
     
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
  for (int i= 0; i < 6; i++) {
    delete CSynDlg[i];
    delete GJunctDlg[i];
    delete nHHDlg[i];
    delete abHHDlg[i];
  }
  delete inChnDlg;
  delete outChnDlg;
  delete DDataDlg;
  delete SDAQDlg;

#ifdef NIDAQ
  delete NDQDlg;
#endif
  
  delete ExportLogFileDlg;
  delete LoadProtocolFileDlg;
  delete SaveProtocolFileDlg;
//  delete[] inChnp;
//  delete[] outChnp;
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
  if (index == 3) {
    STInputFileL->setEnabled(true); STInputFileE->setEnabled(true);
  }
  else {
    if (index > 0) {
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
      NumberE->setEnabled(true); NumberL->setEnabled(true); NumberU->setEnabled(true);
      PeriodE->setEnabled(true); PeriodL->setEnabled(true); PeriodU->setEnabled(true);
      SpikeTimesBut->setEnabled(true);
    }
    if (index == 2) {
      STInputFileL->setEnabled(true); STInputFileE->setEnabled(true);
    }
  }
}       


#include "SynXComboChanged.cc"
#include "HHXComboChanged.cc"

void MyMainWindow::StartButClicked() 
{ 
  actionLoad_Protocol->setEnabled(false);
  actionSave_Protocol->setEnabled(false);
  actionLoad_Script->setEnabled(false);
  actionUnload_Script->setEnabled(false);
  actionExport_Log->setEnabled(false);
  actionExit->setEnabled(false);
  actionInput_channels->setEnabled(false);
  actionOutput_channels->setEnabled(false);
  actionDAQ->setEnabled(false);
  actionSave_config->setEnabled(false);
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
  actionInput_channels->setEnabled(true);
  actionOutput_channels->setEnabled(true);
  actionDAQ->setEnabled(true);
  actionSave_config->setEnabled(true);
  DAQComboBox->setEnabled(true);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
}

void MyMainWindow::exportData()
{
  // collect data from the synapses
  synType[0]= Syn0Combo->currentIndex();
  synType[1]= Syn1Combo->currentIndex();
  synType[2]= Syn2Combo->currentIndex();
  synType[3]= Syn3Combo->currentIndex();
  synType[4]= Syn4Combo->currentIndex();
  synType[5]= Syn5Combo->currentIndex();

  for (int i= 0; i < 6; i++) {
    CSynp[i].active= (synType[i] == 1);
    ESynp[i].active= (synType[i] == 2);
    CSynDlg[i]->exportData(CSynp[i]);
    GJunctDlg[i]->exportData(ESynp[i]);
  }
  // collect data from the HH conductances
  HHType[0]= HH0Combo->currentIndex();
  HHType[1]= HH1Combo->currentIndex();
  HHType[2]= HH2Combo->currentIndex();
  HHType[3]= HH3Combo->currentIndex();
  HHType[4]= HH4Combo->currentIndex();
  HHType[5]= HH5Combo->currentIndex();
  
  for (int i= 0; i < 6; i++) {
    mhHHp[i].active= (HHType[i] == 1);
    abHHp[i].active= (HHType[i] == 2);
    nHHDlg[i]->exportData(mhHHp[i]);
    abHHDlg[i]->exportData(abHHp[i]);
  }
  
  inChnDlg->exportData();
  outChnDlg->exportData();
  DDataDlg->exportData(DigiDatap);
  SDAQDlg->exportData(SDAQp);
#ifdef NIDAQ
  NDQDlg->exportData(NIDAQp);
#endif
  exportSGData();
  SpkTDlg->exportData();
  for (int i= 0; i < 2; i++) graphDlg[i]->exportData(Graphp[i]);
  // collect spike generator data
      
}
 
void MyMainWindow::importData()
{
  // collect data from the synapses
  for (int i= 0; i < MAX_SYN_NO; i++) {
    synType[i]= 0;
    if (CSynp[i].active) synType[i]= 1;
    if (ESynp[i].active) synType[i]= 2;
  }
  Syn0Combo->setCurrentIndex(synType[0]);
  Syn1Combo->setCurrentIndex(synType[1]);
  Syn2Combo->setCurrentIndex(synType[2]);
  Syn3Combo->setCurrentIndex(synType[3]);
  Syn4Combo->setCurrentIndex(synType[4]);
  Syn5Combo->setCurrentIndex(synType[5]);

  for (int i= 0; i < 6; i++) {
    CSynDlg[i]->importData(CSynp[i]);
    GJunctDlg[i]->importData(ESynp[i]);
  }
  
  // collect data from the HH conductances
  for (int i= 0; i < MAX_HH_NO; i++) {
    HHType[i]= 0;
    if (mhHHp[i].active) HHType[i]= 1;
    if (abHHp[i].active) HHType[i]= 2;
  }
  HH0Combo->setCurrentIndex(HHType[0]);
  HH1Combo->setCurrentIndex(HHType[1]);
  HH2Combo->setCurrentIndex(HHType[2]);
  HH3Combo->setCurrentIndex(HHType[3]);
  HH4Combo->setCurrentIndex(HHType[4]);
  HH5Combo->setCurrentIndex(HHType[5]);
  
  for (int i= 0; i < 6; i++) {
    nHHDlg[i]->importData(mhHHp[i]);
    abHHDlg[i]->importData(abHHp[i]);
  }
  
  inChnDlg->importData();
  inChnDlg->accept();
  outChnDlg->importData();
  outChnDlg->accept();
  DDataDlg->importData(DigiDatap);
  SDAQDlg->importData(SDAQp);
#ifdef NIDAQ
  NDQDlg->importData(NIDAQp);
#endif
  importSGData();
  SpkTDlg->importData();
  for (int i= 0; i < 2; i++) graphDlg[i]->importData(Graphp[i]);
  // collect spike generator data
      
}

void MyMainWindow::exportSGData() 
{
  SGp.method= SGMethodCombo->currentIndex();
  SGp.active= (SGp.method > 0);
  SGp.LUTables= SGLUTableCombo->currentIndex();
  SGp.VSpike= VSpikeE->text().toDouble()/1e3;
  SGp.spkTimeScaling= 5e3/WidthE->text().toDouble();
  SGp.VRest= VRestE->text().toDouble()/1e3;
  
  SGp.bdType= BurstDetectionCombo->currentIndex();
  SGp.bdChannel= SGbdChannelCombo->currentIndex();
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
  SGLUTableCombo->setCurrentIndex(SGp.LUTables);
  num.setNum(SGp.VSpike*1e3);
  VSpikeE->setText(num);
  num.setNum(5e3/SGp.spkTimeScaling);
  WidthE->setText(num);
  num.setNum(SGp.VRest*1e3);
  VRestE->setText(num);
  
  BurstDetectionCombo->setCurrentIndex(SGp.bdType);
  SGbdChannelCombo->setCurrentIndex(SGp.bdChannel);
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

void MyMainWindow::updateSGInChn(int chN, int *chns) 
{
  QString lb;

  while (SGbdChannelCombo->count() > 0) {
    SGbdChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    SGbdChannelCombo->addItem(lb);
  }
}

void MyMainWindow::showDAQDlg()
{
  theDAQDlg->appear();
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
}


void MyMainWindow::ExportLog()
{
  QStringList fnlist= ExportLogFileDlg->selectedFiles();
  doExportLog(*fnlist.begin());
}

void MyMainWindow::doExportLog(QString &fname)
{
  ofstream os(fname.toAscii());
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


#include "AP.cc"