
#include "GapJunctionDlg.h"

GapJunctionDlg::GapJunctionDlg(int no, QWidget *parent)
     : QDialog(parent),
       gja(this)
 {
     QString lb;
     setupUi(this);
     
     No= no;
     lb.setNum(No);
     gapJunctDlgLabel->setText(QString("Synapse ")+lb);

     connect(btnAssign, SIGNAL(clicked(bool)), &gja, SLOT(open()));
     connect(cbUseLegacy, &QCheckBox::stateChanged, this, [=](){
         preInChannelCombo->setEnabled(cbUseLegacy->isChecked());
         postInChannelCombo->setEnabled(cbUseLegacy->isChecked());
         preOutChannelCombo->setEnabled(cbUseLegacy->isChecked());
         postOutChannelCombo->setEnabled(cbUseLegacy->isChecked());
     });
     connect(parent, SIGNAL(channelsChanged()), &gja, SLOT(updateChns()));
}

void GapJunctionDlg::exportData(GJunctData &p) 
{
  p.preInChannel= preInChannelCombo->currentIndex();
  p.postInChannel= postInChannelCombo->currentIndex();
  p.preOutChannel= preOutChannelCombo->currentIndex();
  p.postOutChannel= postOutChannelCombo->currentIndex();
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  p.noLegacyAssign = !cbUseLegacy->isChecked();
  gja.exportData(p.assign);
}

void GapJunctionDlg::importData(GJunctData p) 
{
  QString num;
  
  preInChannelCombo->setCurrentIndex(p.preInChannel);
  postInChannelCombo->setCurrentIndex(p.postInChannel);
  preOutChannelCombo->setCurrentIndex(p.preOutChannel);
  postOutChannelCombo->setCurrentIndex(p.postOutChannel);
  typeCombo->setCurrentIndex(p.type);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  cbUseLegacy->setChecked(!p.noLegacyAssign);
  gja.importData(p.assign);
}

void GapJunctionDlg::updateOutChn(int chN, int *chns) 
{
  QString currentPre, currentPost;
  QString lb;
  int newInd;

  currentPre= preOutChannelCombo->currentText();
  while (preOutChannelCombo->count() > 0) {
    preOutChannelCombo->removeItem(0);
  }
  currentPost= postOutChannelCombo->currentText();
  while (postOutChannelCombo->count() > 0) {
    postOutChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    preOutChannelCombo->addItem(lb);
    postOutChannelCombo->addItem(lb);
  }
  lb= QString("none");
  preOutChannelCombo->addItem(lb);
  postOutChannelCombo->addItem(lb);

  newInd= preOutChannelCombo->findText(currentPre);
  if (newInd >= 0) preOutChannelCombo->setCurrentIndex(newInd);
  else preOutChannelCombo->setCurrentIndex(0);
  newInd= postOutChannelCombo->findText(currentPost);
  if (newInd >= 0) postOutChannelCombo->setCurrentIndex(newInd);
  else postOutChannelCombo->setCurrentIndex(0);
}

void GapJunctionDlg::updateInChn(int chN, int *chns) 
{
  QString currentPre, currentPost;
  QString lb;
  int newInd;

  currentPre= preInChannelCombo->currentText();
  while (preInChannelCombo->count() > 0) {
    preInChannelCombo->removeItem(0);
  }
  currentPost= postInChannelCombo->currentText();
  while (postInChannelCombo->count() > 0) {
    postInChannelCombo->removeItem(0);
  }
  for (int i= 0; i < chN; i++) {
    lb.setNum(chns[i]);
    preInChannelCombo->addItem(lb);
    postInChannelCombo->addItem(lb);
  }
  lb= QString("SG");
  preInChannelCombo->addItem(lb);
  postInChannelCombo->addItem(lb);

  newInd= preInChannelCombo->findText(currentPre);
  if (newInd >= 0) preInChannelCombo->setCurrentIndex(newInd);
  else preInChannelCombo->setCurrentIndex(0);
  newInd= postInChannelCombo->findText(currentPost);
  if (newInd >= 0) postInChannelCombo->setCurrentIndex(newInd);
  else postInChannelCombo->setCurrentIndex(0);
}
