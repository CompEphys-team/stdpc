#include "SimulDAQDlg.h"
#include "Tools.cpp"

SimulDAQDlg::SimulDAQDlg(QWidget *parent) : DAQDlg(parent)
{
  setupUi(this);
}

bool SimulDAQDlg::exportData(SDAQData &SDAQp)
{
  bool change= false;
  uint inChnNo = SDAQp.inChn.size(), outChnNo = SDAQp.outChn.size();
  getEntry(SDAQp.inFileName, InputFileE->text(), change);
  getEntry(SDAQp.outFileName, OutputFileE->text(), change);
  getEntry(inChnNo, inChannelE->text().toUInt(), change);
  getEntry(outChnNo, outChannelE->text().toUInt(), change);
  getEntry(SDAQp.inTFac, inTFacE->text().toDouble(), change);
  getEntry(SDAQp.outDt, outDtE->text().toDouble()*1e-3, change);
  if ( change ) {
      SDAQp.inChn.resize(inChnNo);
      SDAQp.outChn.resize(outChnNo);
  }
  return change;
}

void SimulDAQDlg::importData(SDAQData SDAQp)
{
  QString num;
  InputFileE->setText(SDAQp.inFileName); 
  OutputFileE->setText(SDAQp.outFileName);
  inChannelE->setText(QString::number(SDAQp.inChn.size()));
  outChannelE->setText(QString::number(SDAQp.outChn.size()));
  num.setNum(SDAQp.inTFac);
  inTFacE->setText(num);
  num.setNum(SDAQp.outDt*1e3);
  outDtE->setText(num);
}

void SimulDAQDlg::accept()
{
  bool change= exportData(SDAQp);
  if (change) reinitDAQ();
  QDialog::accept();
}

void SimulDAQDlg::reject()
{
  importData(SDAQp);
  QDialog::reject();
}

