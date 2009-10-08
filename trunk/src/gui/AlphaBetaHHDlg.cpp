
#include "AlphaBetaHHDlg.h"
#include <QMessageBox>

AlphaBetaHHDlg::AlphaBetaHHDlg(int no, QWidget *parent)
     : QDialog(parent)
 {
  setupUi(this);
  QString lb;
  lb.setNum(no);
  lb= "HH "+lb;
  HHDlgLabel->setText(lb);
}

void AlphaBetaHHDlg::exportData(abHHData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.VChannel= VChannelCombo->currentIndex();
  p.IChannel= IChannelCombo->currentIndex();
  p.gMax= gMaxE->text().toDouble();
  p.Vrev= VrevE->text().toDouble();
  p.mExpo= mExpoE->text().toInt();
  p.hExpo= hExpoE->text().toInt();
  // ab formalism
  p.maFunc= maFuncCombo->currentIndex();
  p.mka= mkaE->text().toDouble();
  p.mVa= mVaE->text().toDouble();
  p.msa= msaE->text().toDouble();
  p.mbFunc= mbFuncCombo->currentIndex();
  p.mkb= mkbE->text().toDouble();
  p.mVb= mVbE->text().toDouble();
  p.msb= msbE->text().toDouble();
  p.haFunc= haFuncCombo->currentIndex();
  p.hka= hkaE->text().toDouble();
  p.hVa= hVaE->text().toDouble();
  p.hsa= hsaE->text().toDouble();
  p.hbFunc= hbFuncCombo->currentIndex();
  p.hkb= hkbE->text().toDouble();
  p.hVb= hVbE->text().toDouble();
  p.hsb= hsbE->text().toDouble();
}

void AlphaBetaHHDlg::importData(abHHData p)
{
  QString num;
     
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  VChannelCombo->setCurrentIndex(p.VChannel);
  IChannelCombo->setCurrentIndex(p.IChannel);
  num.setNum(p.gMax);
  gMaxE->setText(num);
  num.setNum(p.Vrev);
  VrevE->setText(num);
  num.setNum(p.mExpo);
  mExpoE->setText(num);
  num.setNum(p.hExpo);
  hExpoE->setText(num);
  // ab formalism
  maFuncCombo->setCurrentIndex(p.maFunc);
  num.setNum(p.mka);
  mkaE->setText(num);
  num.setNum(p.mVa);
  mVaE->setText(num);
  num.setNum(p.msa);
  msaE->setText(num);
  mbFuncCombo->setCurrentIndex(p.mbFunc);
  num.setNum(p.mkb);
  mkbE->setText(num);
  num.setNum(p.mVb);
  mVbE->setText(num);
  num.setNum(p.msb);
  msbE->setText(num);
  haFuncCombo->setCurrentIndex(p.haFunc);
  num.setNum(p.hka);
  hkaE->setText(num);
  num.setNum(p.hVa);
  hVaE->setText(num);
  num.setNum(p.hsa);
  hsaE->setText(num);
  hbFuncCombo->setCurrentIndex(p.hbFunc);
  num.setNum(p.hkb);
  hkbE->setText(num);
  num.setNum(p.hVb);
  hVbE->setText(num);
  num.setNum(p.hsb);
  hsbE->setText(num);
}
  
void AlphaBetaHHDlg::updateOutChn(int chN, int *chns) 
{
  QString current;
  QString lb;
  int newInd;

  current= IChannelCombo->currentText();
  while (IChannelCombo->count() > 0) {
    IChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    IChannelCombo->addItem(lb);
  }
  newInd= IChannelCombo->findText(current);
  if (newInd >= 0) IChannelCombo->setCurrentIndex(newInd);
  else IChannelCombo->setCurrentIndex(0);
}

void AlphaBetaHHDlg::updateInChn(int chN, int *chns) 
{
  QString current;
  QString lb;
  int newInd;

  current= VChannelCombo->currentText();
  while (VChannelCombo->count() > 0) {
    VChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    VChannelCombo->addItem(lb);
  }
  lb= QString("SG");
  VChannelCombo->addItem(lb);
  newInd= VChannelCombo->findText(current);
  if (newInd >= 0) VChannelCombo->setCurrentIndex(newInd);
  else VChannelCombo->setCurrentIndex(0);
}
