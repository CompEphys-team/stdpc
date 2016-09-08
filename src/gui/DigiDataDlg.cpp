#include "DigiDataDlg.h"
#include "Tools.cpp"

DigiDataDlg::DigiDataDlg(QWidget *parent) : DAQDlg(parent)
{
  setupUi(this);
}

bool DigiDataDlg::exportData(DigiDataData &DDAQp)
{
  bool change= false;
  bool success;
  
  short int tmp= (short int) BaseAddressE->text().toInt(&success,16);
  if (success) {
     getEntry(DDAQp.baseAddress, tmp, change);
  }
  return change;
}

void DigiDataDlg::importData(DigiDataData DDAQp)
{
  QString num;
  num.setNum(DDAQp.baseAddress,16);
  BaseAddressE->setText(num);
}

void DigiDataDlg::accept()
{
  bool change= exportData(DigiDatap);
  if (change) reinitDAQ();
  QDialog::accept();
}

void DigiDataDlg::reject()
{
  importData(DigiDatap);
  QDialog::reject();
}

