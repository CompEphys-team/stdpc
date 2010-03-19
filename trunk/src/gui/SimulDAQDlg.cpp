#include "SimulDAQDlg.h"
#include "Tools.cpp"

SimulDAQDlg::SimulDAQDlg(QWidget *parent) : DAQDlg(parent)
{
  setupUi(this);
}

bool SimulDAQDlg::exportData(SDAQData &SDAQp)
{
  bool change= false;
  getEntry(SDAQp.inFileName, InputFileE->text(), change);
  getEntry(SDAQp.outFileName, OutputFileE->text(), change);
  getEntry(SDAQp.inChnNo, inChannelE->text().toShort(), change);
  getEntry(SDAQp.outChnNo, outChannelE->text().toShort(), change);
  getEntry(SDAQp.inTFac, inTFacE->text().toDouble(), change);
  getEntry(SDAQp.outDt, outDtE->text().toDouble()*1e-3, change);
  return change;
}

void SimulDAQDlg::importData(SDAQData SDAQp)
{
  QString num;
  InputFileE->setText(SDAQp.inFileName); 
  OutputFileE->setText(SDAQp.outFileName);
  num.setNum(SDAQp.inChnNo);
  inChannelE->setText(num);
  num.setNum(SDAQp.outChnNo);
  outChannelE->setText(num);
  num.setNum(SDAQp.inTFac);
  inTFacE->setText(num);
  num.setNum(SDAQp.outDt*1e3);
  outDtE->setText(num);
}

void SimulDAQDlg::accept()
{
  bool change= exportData(SDAQp);
  if (change) reinitDAQ();
  ((QWidget *)parent())->setEnabled(true);
  hide();
}

void SimulDAQDlg::reject()
{
  importData(SDAQp);
  ((QWidget *)parent())->setEnabled(true);
  hide();
}

void SimulDAQDlg::appear()
{
  ((QWidget *)parent())->setEnabled(false);
  this->setEnabled(true);
  show();
}

