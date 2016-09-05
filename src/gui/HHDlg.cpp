#include <QString>
#include "HHDlg.h"
#include <QMessageBox>

HHDlg::HHDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
     QString lb;
     lb.setNum(no);
     lb= "HH "+lb;
     HHDlgLabel->setText(lb);

     QVector<Dropdown<CurrentAssignment>> vec;
     vec.push_back(Dropdown<CurrentAssignment>(&CurrentAssignment::VChannel, in, "V in", 95));
     vec.push_back(Dropdown<CurrentAssignment>(&CurrentAssignment::IChannel, out, "I out", 95));
     assignments->init(vec);
}

void HHDlg::exportData(mhHHData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.gMax= gMaxE->text().toDouble()*1e-9;
  p.Vrev= VrevE->text().toDouble()*1e-3;
  p.mExpo= mExpoE->text().toInt();
  p.hExpo= hExpoE->text().toInt();
  // mh formalism
  p.Vm= VmE->text().toDouble()*1e-3;
  p.sm= smE->text().toDouble()*1e-3;
  p.Cm= CmE->text().toDouble();
  p.taumType= taumTypeCombo->currentIndex();
  p.taum= taumE->text().toDouble()*1e-3;
  p.taumAmpl= taumAmplE->text().toDouble()*1e-3;
  p.Vtaum= VtaumE->text().toDouble()*1e-3;
  p.staum= staumE->text().toDouble()*1e-3;
  p.Vh= VhE->text().toDouble()*1e-3;
  p.sh= shE->text().toDouble()*1e-3;
  p.Ch= ChE->text().toDouble();
  p.tauhType= tauhTypeCombo->currentIndex();
  p.tauh= tauhE->text().toDouble()*1e-3;
  p.tauhAmpl= tauhAmplE->text().toDouble()*1e-3;
  p.Vtauh= VtauhE->text().toDouble()*1e-3;
  p.stauh= stauhE->text().toDouble()*1e-3;

  assignments->exportData(p.assign);
}

void HHDlg::importData(mhHHData p)
{
  QString num;
  
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  num.setNum(p.gMax*1e9);
  gMaxE->setText(num);
  num.setNum(p.Vrev*1e3);
  VrevE->setText(num);
  num.setNum(p.mExpo);
  mExpoE->setText(num);
  num.setNum(p.hExpo);
  hExpoE->setText(num);
  // mh formalism
  num.setNum(p.Vm*1e3);
  VmE->setText(num);
  num.setNum(p.sm*1e3);
  smE->setText(num);
  num.setNum(p.Cm);
  CmE->setText(num);
  taumTypeCombo->setCurrentIndex(p.taumType);
  num.setNum(p.taum*1e3);
  taumE->setText(num);
  num.setNum(p.taumAmpl*1e3);
  taumAmplE->setText(num);
  num.setNum(p.Vtaum*1e3);
  VtaumE->setText(num);
  num.setNum(p.staum*1e3);
  staumE->setText(num);
  num.setNum(p.Vh*1e3);
  VhE->setText(num);
  num.setNum(p.sh*1e3);
  shE->setText(num);
  num.setNum(p.Ch);
  ChE->setText(num);
  tauhTypeCombo->setCurrentIndex(p.tauhType);
  num.setNum(p.tauh*1e3);
  tauhE->setText(num);
  num.setNum(p.tauhAmpl*1e3);
  tauhAmplE->setText(num);
  num.setNum(p.Vtauh*1e3);
  VtauhE->setText(num);
  num.setNum(p.stauh*1e3);
  stauhE->setText(num);

  assignments->importData(p.assign);
}
