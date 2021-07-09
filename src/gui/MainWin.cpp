/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "MainWin.h"
#include "ui_MainWin.h"
#include "LUtables.h"
#include "AP.h"
#include <Windows.h>
#include <QScrollBar>
#include "ModelOpts.h"
#include "DaqOpts.h"

MyMainWindow::MyMainWindow(QWidget *parent)
     : QMainWindow(parent),
       ui(new Ui::MainWindow)
 {
     ui->setupUi(this);

     DSDlg= new DataSavingDlg(this);
     TrigDlg= new TriggerDlg(this);

     rateIndicator = new QLabel("Ready");
     ui->statusbar->addPermanentWidget(rateIndicator);
     loadedProtocolStatus = new QLabel("");
     ui->statusbar->addPermanentWidget(loadedProtocolStatus);
         
     DCT= new DCThread();
     
     connect(ui->StartBut, SIGNAL(clicked()), SLOT(StartButClicked()));
     connect(ui->StopBut, SIGNAL(clicked()), SLOT(StopButClicked()));
     connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MyMainWindow::TabChanged);
     
     connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
     connect(ui->actionExport_Log, SIGNAL(triggered()), this, SLOT(ExportLog()));
     connect(ui->actionClear_Log, SIGNAL(triggered()), SLOT(ClearLog()));
     connect(ui->actionLoad_Protocol, SIGNAL(triggered()), this, SLOT(LoadProtocol()));
     connect(ui->actionSave_Protocol, SIGNAL(triggered()), this, SLOT(SaveProtocol()));
     connect(ui->actionLoad_Script, SIGNAL(triggered()), this, SLOT(LoadScript()));
     connect(ui->actionUnload_Script, SIGNAL(triggered()), SLOT(UnLoadScript()));
     connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(DisplayAbout()));
     connect(ui->actionStart_trigger, SIGNAL(triggered(bool)), TrigDlg, SLOT(open()));
     connect(ui->actionData_saving, SIGNAL(triggered(bool)), DSDlg, SLOT(open()));
     connect(this, SIGNAL(destroyed()), SLOT(close()));

     connect(this, &MyMainWindow::channelsChanged, &ChannelListModel::updateChns_static_noargs);
     connect(this, &MyMainWindow::modelRemoved, &ChannelListModel::updateChns_static);
     connect(this, &MyMainWindow::channelsChanged, TrigDlg, &TriggerDlg::updateChannels);

     QTreeWidgetItem *hDAQ = new QTreeWidgetItem(ui->treeWidget);
     hDAQ->setText(0, "Data sources");
     QTreeWidgetItem *bDAQ = new QTreeWidgetItem(hDAQ);
     DAQ_tree_item = new TreeWidgetItem_DataSources(this);
     ui->treeWidget->setItemWidget(bDAQ, 0, DAQ_tree_item);

     QTreeWidgetItem *hTools = new QTreeWidgetItem(ui->treeWidget, hDAQ);
     hTools->setText(0, "Tools");
     QTreeWidgetItem *bTools = new QTreeWidgetItem(hTools);
     TreeWidgetItem_Models *box = new TreeWidgetItem_Models(ConductanceManager::Tools());
     ui->treeWidget->setItemWidget(bTools, 0, box);
     model_tree_items.push_back(box);

     QTreeWidgetItem *hSynapses = new QTreeWidgetItem(ui->treeWidget, hTools);
     hSynapses->setText(0, "Synapses");
     QTreeWidgetItem *bSynapses = new QTreeWidgetItem(hSynapses);
     box = new TreeWidgetItem_Models(ConductanceManager::Synapses());
     ui->treeWidget->setItemWidget(bSynapses, 0, box);
     model_tree_items.push_back(box);

     QTreeWidgetItem *hCurrents = new QTreeWidgetItem(ui->treeWidget, hSynapses);
     hCurrents->setText(0, "Ionic conductances");
     QTreeWidgetItem *bCurrents = new QTreeWidgetItem(hCurrents);
     box = new TreeWidgetItem_Models(ConductanceManager::Currents());
     ui->treeWidget->setItemWidget(bCurrents, 0, box);
     model_tree_items.push_back(box);
   
     connect(DCT,SIGNAL(message(QString)),SLOT(DisplayMessage(QString)));
     connect(DCT, &DCThread::updateRate, [this](int rate){
         rateIndicator->setText(QString("Update rate: %1 Hz").arg(rate));
     });
     connect(DCT, &DCThread::done, [this](){
         rateIndicator->setText("Ready");
     });
     connect(DCT,SIGNAL(CloseToLimit(QString, QString, double, double, double)),SLOT(CloseToLimitWarning(QString, QString, double, double, double)));
     
     initAP();
     LoadConfig();

     hDAQ->setExpanded(GUIp.openDAQ);
     hTools->setExpanded(GUIp.openTools);
     hSynapses->setExpanded(GUIp.openSynapses);
     hCurrents->setExpanded(GUIp.openCurrents);

     DisplayMessage(QString("Main: Dynamic Clamp starting ..."));
     exportData(true);
     updateDeviceStatus();
 } 

MyMainWindow::~MyMainWindow()
{
  delete ui;
  delete DSDlg;
  delete TrigDlg;
  delete DCT;

  ChannelListModel::destruct();
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
        if ( name.length() )
            DisplayMessage(QString("Good news: %1 found and opened successfully!").arg(name));
        break;
    case DeviceStatus::Failed :
        DisplayMessage(QString("Bad news: %1 not found or not opened successfully!").arg(name));
        break;
    case DeviceStatus::Inactive :
        break;
    }

    emit channelsChanged();
    updateStartButton();
}

void MyMainWindow::updateStartButton()
{
    ui->StartBut->setEnabled(!Devices.active().empty() || Models.empty());
}


void MyMainWindow::closeEvent(QCloseEvent *event)
{
  QString fname;
  if (DCT) {
    //DCT->terminate();
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
    while (!DCT->finished) Sleep(100);
  }
  SaveConfig();
  fname= QString("StdpC_last.log");
  doExportLog(fname);
  event->accept();
}

void MyMainWindow::StartButClicked() 
{
  if ( !DCT->stopped && DCT->settling ) {
      DCT->wakeFromSettling = true;
      return;
  }
  ui->actionLoad_Protocol->setEnabled(false);
  ui->actionSave_Protocol->setEnabled(false);
  ui->actionLoad_Script->setEnabled(false);
  ui->actionUnload_Script->setEnabled(false);
  ui->actionExport_Log->setEnabled(false);
  ui->actionExit->setEnabled(false);
  ui->menuConfigure->setEnabled(false);
  ui->cbDatasaving->setEnabled(false);
  ui->cbTrigger->setEnabled(false);
  ui->cbStopTrigger->setEnabled(false);
  ui->cbSettle->setEnabled(false);
  ui->dblSettleDuration->setEnabled(false);
  DAQ_tree_item->setEnabled(false);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
  if (!DCT->finished) {
      Sleep(10);
      emit ui->StartBut->clicked();
      return;
  }
  exportData();

  QPalette pal = palette();
  pal.setColor(QPalette::Background, QColor(255,140,0));
  ui->controlPanel->setAutoFillBackground(true);
  ui->controlPanel->setPalette(pal);

  if ( !ui->graphtab->startPlotting(DCT) )
      ui->tabWidget->setTabEnabled(1, false);
  if ( !ui->performancetab->startPlotting(DCT) )
      ui->tabWidget->setTabEnabled(2, false);
  rateIndicator->setText("Started");
  DCT->setup_and_go();
}

void MyMainWindow::StopButClicked()
{
  ui->actionLoad_Protocol->setEnabled(true);
  ui->actionSave_Protocol->setEnabled(true);
  ui->actionLoad_Script->setEnabled(!DCT->scripting);
  ui->actionUnload_Script->setEnabled(DCT->scripting);
  ui->actionExport_Log->setEnabled(true);
  ui->actionExit->setEnabled(true);
  ui->menuConfigure->setEnabled(true);
  ui->cbDatasaving->setEnabled(true);
  ui->cbTrigger->setEnabled(true);
  ui->cbStopTrigger->setEnabled(true);
  ui->cbSettle->setEnabled(true);
  ui->dblSettleDuration->setEnabled(true);
  DAQ_tree_item->setEnabled(true);
  if (!DCT->stopped) {
    DCT->stopped= true;
    DisplayMessage(QString("Main: Dynamic Clamp stopped."));
  }
  ui->graphtab->stopPlotting();
  ui->performancetab->stopPlotting();
  ui->tabWidget->setTabEnabled(1, true);
  ui->tabWidget->setTabEnabled(2, true);
  ui->controlPanel->setPalette(palette());
}

void MyMainWindow::TabChanged(int idx)
{
    static int prevIdx = 0;
    if ( prevIdx == 0 && idx != 0 )
        exportData();
    prevIdx = idx;
}

void MyMainWindow::exportData(bool ignoreDAQ)
{
  DAQ_tree_item->exportData(ignoreDAQ);
  for ( TreeWidgetItem_Models *box : model_tree_items )
      box->exportData();
  DSDlg->exportData();
  dataSavingPs.enabled = ui->cbDatasaving->isChecked();
  TrigDlg->exportData();
  Triggerp.active = ui->cbTrigger->isChecked();
  Triggerp.activeStop = ui->cbStopTrigger->isChecked();
  Settlingp.active = ui->cbSettle->isChecked();
  Settlingp.duration = ui->dblSettleDuration->value();
  ClampThreadPriority = ui->threadPriority->currentIndex();
  ui->graphtab->exportData();
  ui->performancetab->exportData();
  emit channelsChanged();

  QTreeWidgetItemIterator it(ui->treeWidget, QTreeWidgetItemIterator::HasChildren);
  GUIp.openDAQ = (*it++)->isExpanded();
  GUIp.openTools = (*it++)->isExpanded();
  GUIp.openSynapses = (*it++)->isExpanded();
  GUIp.openCurrents = (*it)->isExpanded();
}
 
void MyMainWindow::importData()
{
  DAQ_tree_item->importData();
  for ( TreeWidgetItem_Models *box : model_tree_items )
      box->importData();
  DSDlg->importData();
  ui->cbDatasaving->setChecked(dataSavingPs.enabled);
  TrigDlg->importData();
  ui->cbTrigger->setChecked(Triggerp.active);
  ui->cbStopTrigger->setChecked(Triggerp.activeStop);
  ui->cbSettle->setChecked(Settlingp.active);
  ui->dblSettleDuration->setValue(Settlingp.duration);
  ui->threadPriority->setCurrentIndex(ClampThreadPriority);
  ui->graphtab->importData();
  ui->performancetab->importData();
  updateStartButton();
}

void MyMainWindow::SaveConfig()
{
  ofstream os("StdpC.conf");
  DoSaveProtocol(os);
  os.close();
}

void MyMainWindow::LoadConfig()
{
    ifstream is("StdpC.conf");
    if ( !DoLoadProtocol(is) )
        DisplayMessage(QString("No valid config file found; reverting to default settings"));
    is.close();
}

void MyMainWindow::SaveProtocol()
{
  QString fname = QFileDialog::getSaveFileName(this, "Save protocol to file", "", "StdpC protocols (*.cpr);;All files(*)");
  if ( fname.isEmpty() )
      return;
  if ( !fname.endsWith(".cpr") && !QFileInfo(fname).exists() && !QFileInfo(QString("%1.cpr").arg(fname)).exists() )
      fname.append(".cpr");

  ofstream os(fname.toLatin1());
  DoSaveProtocol(os);
  os.close();
}

void MyMainWindow::DoSaveProtocol(ofstream &os)
{
  os << STDPC_PROTOCOL_HEADER << " " << STDPC_PROTOCOL_VERSION << endl << endl;

  exportData();

  for ( auto const& ap : AP::params() ) {
      ap->write(os);
  }
}


void MyMainWindow::LoadProtocol()
{
  QString fname = QFileDialog::getOpenFileName(this, "Load protocol file", "", "StdpC protocols (*.cpr);;All files(*)");
  if ( fname.isEmpty() )
    return;

  ifstream is(fname.toLatin1());
  if (!is.good()) {
    DisplayMessage(QString("Error opening protocol file"));
    return;
  }

  bool ret = DoLoadProtocol(is);
  is.close();

  if ( ret )
      loadedProtocolStatus->setText(QString("Loaded protocol file: %1").arg(fname));
  else
      DisplayMessage(QString("Error reading protocol file"));
}

bool MyMainWindow::DoLoadProtocol(ifstream &is)
{
  // Clear params before loading
  Models.clear();
  Devices.clear();
  Conductances.clear();
  Plotp.plot.clear();

  std::function<bool(QString)> callback = [=](QString name) {
      DisplayMessage(QString("Warning: Failed to read parameter \"%1\"").arg(name));
      return false;
  };
  bool ret = readProtocol(is, &callback);

  importData();

  return ret;
}


void MyMainWindow::ExportLog()
{
  QString fname = QFileDialog::getSaveFileName(this, "Export log to file", "", "Log files (*.log);;All files (*)");
  if ( fname.isEmpty() )
      return;
  if ( !fname.endsWith(".log") && !QFileInfo(fname).exists() && !QFileInfo(QString("%1.log").arg(fname)).exists() )
      fname.append(".log");
  doExportLog(fname);
}

void MyMainWindow::doExportLog(QString fname)
{
  ofstream os(fname.toLatin1());
  int i= 0, done= 0;
  QListWidgetItem *it;
  
  while (!done) {
    it= ui->MessageWindow->item(i);
    if (it != nullptr) {
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

void MyMainWindow::LoadScript()
{
  QString fname = QFileDialog::getOpenFileName(this, "Load script file", "", "StdpC scripts(*.scr);;All files(*)");
  if ( !fname.isEmpty() && DCT->LoadScript(fname) ) {
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
    QMessageBox::information(this,tr("About StdpC"),
tr(R"EOF(
StdpC is free dynamic clamp software including plasticity of synapses and active electrode compensation.
It is distributed under the GPL v2 license.
For more information, visit https://github.com/tnowotny/stdpc/ .
You are running StdpC version 6.1.
)EOF"));
}
