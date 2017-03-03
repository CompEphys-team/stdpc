
#include "AlphaBetaHHDlg.h"
#include <QMessageBox>

AlphaBetaHHDlg::AlphaBetaHHDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
  setupUi(this);

  label = HHDlgLabel->text();
  setIndex(no);

  QVector<AssignmentCellBase<CurrentAssignment>*> vec;
  vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
  vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "V in", 180, in));
  vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "I out", 180, out));
  assignments->init(vec);
}

void AlphaBetaHHDlg::setIndex(int no)
{
    HHDlgLabel->setText(label.arg(no));
}

void AlphaBetaHHDlg::exportData(abHHData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.gMax= gMaxE->text().toDouble()*1e-9;
  p.Vrev= VrevE->text().toDouble()*1e-3;
  p.mExpo= mExpoE->text().toInt();
  p.hExpo= hExpoE->text().toInt();
  // ab formalism
  p.maFunc= maFuncCombo->currentIndex();
  p.mka= mkaE->text().toDouble()*1e3;
  p.mVa= mVaE->text().toDouble()*1e-3;
  p.msa= msaE->text().toDouble()*1e-3;
  p.mbFunc= mbFuncCombo->currentIndex();
  p.mkb= mkbE->text().toDouble()*1e3;
  p.mVb= mVbE->text().toDouble()*1e-3;
  p.msb= msbE->text().toDouble()*1e-3;
  p.haFunc= haFuncCombo->currentIndex();
  p.hka= hkaE->text().toDouble()*1e3;
  p.hVa= hVaE->text().toDouble()*1e-3;
  p.hsa= hsaE->text().toDouble()*1e-3;
  p.hbFunc= hbFuncCombo->currentIndex();
  p.hkb= hkbE->text().toDouble()*1e3;
  p.hVb= hVbE->text().toDouble()*1e-3;
  p.hsb= hsbE->text().toDouble()*1e-3;

  assignments->exportData(p.assign);
}

void AlphaBetaHHDlg::importData(abHHData p)
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
  // ab formalism
  maFuncCombo->setCurrentIndex(p.maFunc);
  num.setNum(p.mka*1e-3);
  mkaE->setText(num);
  num.setNum(p.mVa*1e3);
  mVaE->setText(num);
  num.setNum(p.msa*1e3);
  msaE->setText(num);
  mbFuncCombo->setCurrentIndex(p.mbFunc);
  num.setNum(p.mkb*1e-3);
  mkbE->setText(num);
  num.setNum(p.mVb*1e3);
  mVbE->setText(num);
  num.setNum(p.msb*1e3);
  msbE->setText(num);
  haFuncCombo->setCurrentIndex(p.haFunc);
  num.setNum(p.hka*1e-3);
  hkaE->setText(num);
  num.setNum(p.hVa*1e3);
  hVaE->setText(num);
  num.setNum(p.hsa*1e3);
  hsaE->setText(num);
  hbFuncCombo->setCurrentIndex(p.hbFunc);
  num.setNum(p.hkb*1e-3);
  hkbE->setText(num);
  num.setNum(p.hVb*1e3);
  hVbE->setText(num);
  num.setNum(p.hsb*1e3);
  hsbE->setText(num);

  assignments->importData(p.assign);
}
