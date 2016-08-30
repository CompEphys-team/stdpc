
#include "AbSynDlg.h"
#include <QMessageBox>

abSynDlg::abSynDlg(int no, QWidget *parent)
     : QDialog(parent),
       sa(this)
 {
     QString lb;
     setupUi(this);
     
     No= no;
     lb.setNum(No);
     abSynDlgLabel->setText(QString("Synapse ")+lb);
     
     STDP= new STDPDlg(this, No);
     ODESTDP= new ODESTDPDlg(this, No);
     
     connect(PlasticityCombo, SIGNAL(currentIndexChanged(QString)), SLOT(PlastMethodChange()));
     connect(ResCloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(ResCloseClicked(QAbstractButton *)));
     connect(btnAssign, SIGNAL(clicked(bool)), &sa, SLOT(open()));
     connect(cbUseLegacy, &QCheckBox::stateChanged, this, [=](){
         PreSynChannelCombo->setEnabled(cbUseLegacy->isChecked());
         PostSynChannelCombo->setEnabled(cbUseLegacy->isChecked());
         OutSynChannelCombo->setEnabled(cbUseLegacy->isChecked());
     });
     connect(parent, SIGNAL(channelsChanged()), &sa, SLOT(updateChns()));
}

void abSynDlg::ResCloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  if (but->text() == QString("Reset")) {
//    reset();
  }
}

void abSynDlg::PlastMethodChange()
{
  int index= PlasticityCombo->currentIndex();
  STDP->hide();
  ODESTDP->hide();
  PlastParaBut->disconnect();
  
  if (index == 0) {
    PlastParaBut->setEnabled(false);
  }
  else {
    PlastParaBut->setEnabled(true);
  }
     
  if (index == 1) {
    connect(PlastParaBut, SIGNAL(clicked()), STDP, SLOT(show()));
  }
  if (index == 2) {
    connect(PlastParaBut, SIGNAL(clicked()), ODESTDP, SLOT(show()));
  }
}

void abSynDlg::exportData(abSynData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.PreSynChannel= PreSynChannelCombo->currentIndex();
  p.PostSynChannel= PostSynChannelCombo->currentIndex();
  p.OutSynChannel= OutSynChannelCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  p.Vrev= VrevE->text().toDouble()*1e-3;
  p.aS= aSE->text().toDouble()*1e3;
  p.bS= bSE->text().toDouble()*1e3;
  p.aR= aRE->text().toDouble()*1e3;
  p.VaR= VaRE->text().toDouble()*1e-3;
  p.saR= saRE->text().toDouble()*1e-3;
  p.bR= bRE->text().toDouble()*1e3;
  p.fixVpost= fixVpostCombo->currentIndex();
  p.Vpost= VpostE->text().toDouble()*1e-3;
  p.Plasticity= PlasticityCombo->currentIndex();
  // ST plasticity
  STDP->exportData(p.ST);
  // ODE plasticity
  ODESTDP->exportData(p.ODE);

  p.noLegacyAssign = !cbUseLegacy->isChecked();
  sa.exportData(p.assign);
}

void abSynDlg::importData(abSynData p)
{
  QString num;   
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  PreSynChannelCombo->setCurrentIndex(p.PreSynChannel);
  PostSynChannelCombo->setCurrentIndex(p.PostSynChannel);
  OutSynChannelCombo->setCurrentIndex(p.OutSynChannel);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  num.setNum(p.Vrev*1e3);
  VrevE->setText(num);
  num.setNum(p.aS*1e-3);
  aSE->setText(num);
  num.setNum(p.bS*1e-3);
  bSE->setText(num);
  num.setNum(p.aR*1e-3);
  aRE->setText(num);
  num.setNum(p.VaR*1e3);
  VaRE->setText(num);
  num.setNum(p.saR*1e3);
  saRE->setText(num);
  num.setNum(p.bR*1e-3);
  bRE->setText(num);
  fixVpostCombo->setCurrentIndex(p.fixVpost);
  num.setNum(p.Vpost*1e3);
  VpostE->setText(num);
  PlasticityCombo->setCurrentIndex(p.Plasticity);
  // ST plasticity
  STDP->importData(p.ST);
  // ODE plasticity
  ODESTDP->importData(p.ODE);

  cbUseLegacy->setChecked(!p.noLegacyAssign);
  sa.importData(p.assign);
}


void abSynDlg::updateOutChn(int chN, int *chns) 
{
  QString current;
  QString lb;
  int newInd;

  current= OutSynChannelCombo->currentText();
  while (OutSynChannelCombo->count() > 0) {
    OutSynChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    OutSynChannelCombo->addItem(lb);
  }
  newInd= OutSynChannelCombo->findText(current);
  if (newInd >= 0) OutSynChannelCombo->setCurrentIndex(newInd);
  else OutSynChannelCombo->setCurrentIndex(0);
}

void abSynDlg::updateInChn(int chN, int *chns) 
{
  QString currentPre, currentPost;
  QString lb;
  int newInd;

  currentPre= PreSynChannelCombo->currentText();
  while (PreSynChannelCombo->count() > 0) {
    PreSynChannelCombo->removeItem(0);
  }
  currentPost= PostSynChannelCombo->currentText();
  while (PostSynChannelCombo->count() > 0) {
    PostSynChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    PreSynChannelCombo->addItem(lb);
    PostSynChannelCombo->addItem(lb);
  }
  lb= QString("SG");
  PreSynChannelCombo->addItem(lb);
  PostSynChannelCombo->addItem(lb);

  newInd= PreSynChannelCombo->findText(currentPre);
  if (newInd >= 0) PreSynChannelCombo->setCurrentIndex(newInd);
  else PreSynChannelCombo->setCurrentIndex(0);
  newInd= PostSynChannelCombo->findText(currentPost);
  if (newInd >= 0) PostSynChannelCombo->setCurrentIndex(newInd);
  else PostSynChannelCombo->setCurrentIndex(0);
}
