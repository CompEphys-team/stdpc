#include "SimulDAQDlg.h"
#include "Tools.cpp"
#include "SimulDAQ.h"
#include "DeviceManager.h"
#include <QFileDialog>

SimulDAQDlg::SimulDAQDlg(int no, QWidget *parent) : DAQDlg(no, parent)
{
  setupUi(this);
  DAQLabel->setText(DAQLabel->text().arg(no));
  ChannelIndex dex;
  dex.isValid = true;
  dex.isAnalog = true;
  dex.daqClass = DAQClass::Simul;
  dex.devID = no;
  board = Devices.getDevice(dex);
  inDlg->init(board);
  outDlg->init(board);
}

bool SimulDAQDlg::exportData(bool forceInit)
{
  bool change= false;
  bool devOK = true;
  uint inChnNo = SDAQp[idx].inChn.size(), outChnNo = SDAQp[idx].outChn.size();
  getEntry(SDAQp[idx].inFileName, InputFileE->text(), change);
  getEntry(SDAQp[idx].outFileName, OutputFileE->text(), change);
  getEntry(inChnNo, inChannelE->text().toUInt(), change);
  getEntry(outChnNo, outChannelE->text().toUInt(), change);
  getEntry(SDAQp[idx].inTFac, inTFacE->text().toDouble(), change);
  getEntry(SDAQp[idx].outDt, outDtE->text().toDouble()*1e-3, change);
  if ( change ) {
      SDAQp[idx].inChn.resize(inChnNo);
      SDAQp[idx].outChn.resize(outChnNo);
  }
  if ( change || forceInit )
      devOK = initDAQ() != DeviceStatus::Failed;

  inDlg->exportData();
  outDlg->exportData();
  emit channelsChanged();
  return devOK;
}

void SimulDAQDlg::importData()
{
  QString num;
  InputFileE->setText(SDAQp[idx].inFileName);
  OutputFileE->setText(SDAQp[idx].outFileName);
  inChannelE->setText(QString::number(SDAQp[idx].inChn.size()));
  outChannelE->setText(QString::number(SDAQp[idx].outChn.size()));
  num.setNum(SDAQp[idx].inTFac);
  inTFacE->setText(num);
  num.setNum(SDAQp[idx].outDt*1e3);
  outDtE->setText(num);

  inDlg->importData();
  outDlg->importData();
}

void SimulDAQDlg::open()
{
    didInit = false;
    backup = SDAQp[idx];
    QDialog::open();
}

void SimulDAQDlg::accept()
{
  exportData();
  QDialog::accept();
}

void SimulDAQDlg::reject()
{
  SDAQp[idx] = backup;
  if ( didInit )
      initDAQ();
  emit channelsChanged();
  importData();
  QDialog::reject();
}


DeviceStatus SimulDAQDlg::initDAQ()
{
    QString name;
    DeviceStatus status = Devices.initSingle<SDAQData>(name, idx);
    emit deviceStatusChanged(status, name);
    inDlg->init(board);
    outDlg->init(board);
    didInit = true;
    return status;
}


void SimulDAQDlg::on_inChannels_clicked()
{
    exportData();
    inDlg->open();
}

void SimulDAQDlg::on_outChannels_clicked()
{
    exportData();
    outDlg->open();
}

void SimulDAQDlg::on_InputFileB_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select input file...", InputFileE->text());
    if ( !file.isEmpty() )
        InputFileE->setText(file);
}

void SimulDAQDlg::on_OutputFileB_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Select input file...", OutputFileE->text());
    if ( !file.isEmpty() )
        OutputFileE->setText(file);
}
