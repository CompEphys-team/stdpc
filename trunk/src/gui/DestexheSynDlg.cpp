
#include "DestexheSynDlg.h"
#include <QMessageBox>

DestexheSynDlg::DestexheSynDlg(int no, QWidget *parent)
     : QDialog(parent)
 {
     QString lb;
     setupUi(this);

     No= no;
     lb.setNum(No);
     DestexheSynDlgLabel->setText(QString("Synapse ")+lb);

     STDP= new STDPDlg(this, No);
     ODESTDP= new ODESTDPDlg(this, No);

     connect(PlasticityCombo, SIGNAL(currentIndexChanged(QString)), SLOT(PlastMethodChange()));
     connect(ResCloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(ResCloseClicked(QAbstractButton *)));
}

void DestexheSynDlg::ResCloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  if (but->text() == QString("Reset")) {
//    reset();
  }
}

void DestexheSynDlg::PlastMethodChange()
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

void DestexheSynDlg::exportData(DestexheSynData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.PreSynChannel= PreSynChannelCombo->currentIndex();
  p.PostSynChannel= PostSynChannelCombo->currentIndex();
  p.OutSynChannel= OutSynChannelCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  p.Vpre= VpreE->text().toDouble()*1e-3;
  p.Vrev= VrevE->text().toDouble()*1e-3;
  p.trelease= treleaseE->text().toDouble()*1e-3;
  p.alpha= alphaE->text().toDouble()*1e3;
  p.beta= betaE->text().toDouble()*1e3;
  p.fixVpost= fixVpostCombo->currentIndex();
  p.Vpost= VpostE->text().toDouble()*1e-3;
  p.Plasticity= PlasticityCombo->currentIndex();
  // ST plasticity
  STDP->exportData(p.ST);
  // ODE plasticity
  ODESTDP->exportData(p.ODE);
}

void DestexheSynDlg::importData(DestexheSynData p)
{
  QString num;
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  PreSynChannelCombo->setCurrentIndex(p.PreSynChannel);
  PostSynChannelCombo->setCurrentIndex(p.PostSynChannel);
  OutSynChannelCombo->setCurrentIndex(p.OutSynChannel);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  num.setNum(p.Vpre*1e3);
  VpreE->setText(num);
  num.setNum(p.Vrev*1e3);
  VrevE->setText(num);
  num.setNum(p.trelease*1e3);
  treleaseE->setText(num);
  num.setNum(p.alpha*1e-3);
  alphaE->setText(num);
  num.setNum(p.beta*1e-3);
  betaE->setText(num);
  fixVpostCombo->setCurrentIndex(p.fixVpost);
  num.setNum(p.Vpost*1e3);
  VpostE->setText(num);
  PlasticityCombo->setCurrentIndex(p.Plasticity);
  // ST plasticity
  STDP->importData(p.ST);
  // ODE plasticity
  ODESTDP->importData(p.ODE);
}


void DestexheSynDlg::updateOutChn(int chN, int *chns)
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

void DestexheSynDlg::updateInChn(int chN, int *chns)
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
