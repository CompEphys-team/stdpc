#include "SimulDAQDlg.h"
#include "Tools.cpp"
#include "SimulDAQ.h"
#include <QFileDialog>

SimulDAQDlg::SimulDAQDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    DAQDlg(idx, proxy, parent)
{
  setupUi(this);
  connect(inChannels, SIGNAL(clicked(bool)), this, SLOT(openInChnDlg()));
  connect(outChannels, SIGNAL(clicked(bool)), this, SLOT(openOutChnDlg()));
  label = DAQLabel->text();
  setIndex(idx);
}

void SimulDAQDlg::setIndex(size_t no)
{
    DAQLabel->setText(label.arg(no));
    DAQDlg::setIndex(no);
}

void SimulDAQDlg::exportData(bool forceInit)
{
  bool change= false;
  uint inChnNo = SimulDAQProxy::p[idx].inChn.size(), outChnNo = SimulDAQProxy::p[idx].outChn.size();
  getEntry(inChnNo, inChannelE->text().toUInt(), change);
  getEntry(outChnNo, outChannelE->text().toUInt(), change);
  if ( change ) {
      SimulDAQProxy::p[idx].inChn.resize(inChnNo);
      SimulDAQProxy::p[idx].outChn.resize(outChnNo);
      for ( inChnData &i : SimulDAQProxy::p[idx].inChn )
          i.active = true;
      for ( outChnData &i : SimulDAQProxy::p[idx].outChn )
          i.active = true;
  }
  getEntry(SimulDAQProxy::p[idx].inFileName, InputFileE->text(), change);
  getEntry(SimulDAQProxy::p[idx].outFileName, OutputFileE->text(), change);
  getEntry(SimulDAQProxy::p[idx].inTFac, inTFacE->text().toDouble(), change);
  getEntry(SimulDAQProxy::p[idx].outDt, outDtE->text().toDouble()*1e-3, change);

  DAQDlg::exportData(change || forceInit);
}

void SimulDAQDlg::importData()
{
  QString num;
  InputFileE->setText(SimulDAQProxy::p[idx].inFileName);
  OutputFileE->setText(SimulDAQProxy::p[idx].outFileName);
  inChannelE->setText(QString::number(SimulDAQProxy::p[idx].inChn.size()));
  outChannelE->setText(QString::number(SimulDAQProxy::p[idx].outChn.size()));
  num.setNum(SimulDAQProxy::p[idx].inTFac);
  inTFacE->setText(num);
  num.setNum(SimulDAQProxy::p[idx].outDt*1e3);
  outDtE->setText(num);

  DAQDlg::importData();
}

void SimulDAQDlg::backup()
{
    bak = SimulDAQProxy::p[idx];
}

void SimulDAQDlg::restoreBackup()
{
  SimulDAQProxy::p[idx] = bak;
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
