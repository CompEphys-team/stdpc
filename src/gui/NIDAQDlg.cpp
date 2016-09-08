#include "NIDAQDlg.h"
#include "Tools.cpp"

NIDAQDlg::NIDAQDlg(QWidget *parent) : DAQDlg(parent)
{
  setupUi(this);
}

bool NIDAQDlg::exportData(NIDAQData &NDAQp)
{
  bool change= false;
  getEntry(NDAQp.deviceName, DeviceNameE->text(), change);
  return change;
}

void NIDAQDlg::importData(NIDAQData NDAQp)
{
  DeviceNameE->setText(NDAQp.deviceName);
}

void NIDAQDlg::accept()
{
  bool change= exportData(NIDAQp);
  if (change) reinitDAQ();
  QDialog::accept();
}

void NIDAQDlg::reject()
{
  importData(NIDAQp);
  QDialog::reject();
}

