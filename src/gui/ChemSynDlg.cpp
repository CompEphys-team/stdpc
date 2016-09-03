
#include "ChemSynDlg.h"
#include <QMessageBox>

ChemSynDlg::ChemSynDlg(int no, QWidget *parent)
     : QDialog(parent),
       sa(this)
 {
     QString lb;
     setupUi(this);
     
     No= no;
     lb.setNum(No);
     ChemSynDlgLabel->setText(QString("Synapse ")+lb);
     
     STDP= new STDPDlg(this, No);
     ODESTDP= new ODESTDPDlg(this, No);
     
     connect(PlasticityCombo, SIGNAL(currentIndexChanged(QString)), SLOT(PlastMethodChange()));
     connect(ResCloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(ResCloseClicked(QAbstractButton *)));
     connect(STDCombo, SIGNAL(currentIndexChanged(QString)), SLOT(STDComboChange()));
     connect(btnAssign, SIGNAL(clicked(bool)), &sa, SLOT(open()));
     connect(parent, SIGNAL(channelsChanged()), &sa, SLOT(updateChns()));
}

void ChemSynDlg::ResCloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  if (but->text() == QString("Reset")) {
//    reset();
  }
}

void ChemSynDlg::PlastMethodChange()
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

void ChemSynDlg::STDComboChange()
{
  int index= STDCombo->currentIndex();
  
  if (index == 0) {
    STDAmplL->setEnabled(false); STDAmplE->setEnabled(false);
    STDVThreshL->setEnabled(false); STDVThreshE->setEnabled(false); STDVThreshU->setEnabled(false);
    STDVSlopeL->setEnabled(false); STDVSlopeE->setEnabled(false); STDVSlopeU->setEnabled(false);
    STDtau0L->setEnabled(false); STDtau0E->setEnabled(false); STDtau0U->setEnabled(false);
    STDtauAmplL->setEnabled(false); STDtauAmplE->setEnabled(false);
    STDtauVThreshL->setEnabled(false); STDtauVThreshE->setEnabled(false); STDtauVThreshU->setEnabled(false);
    STDtauVSlopeL->setEnabled(false); STDtauVSlopeE->setEnabled(false); STDtauVSlopeU->setEnabled(false);
  }
   
  if (index == 1) {
    STDAmplL->setEnabled(true); STDAmplE->setEnabled(true);
    STDVThreshL->setEnabled(true); STDVThreshE->setEnabled(true); STDVThreshU->setEnabled(true);
    STDVSlopeL->setEnabled(true); STDVSlopeE->setEnabled(true); STDVSlopeU->setEnabled(true);
    STDtau0L->setEnabled(true); STDtau0E->setEnabled(true); STDtau0U->setEnabled(true);
    STDtauAmplL->setEnabled(true); STDtauAmplE->setEnabled(true);
    STDtauVThreshL->setEnabled(true); STDtauVThreshE->setEnabled(true); STDtauVThreshU->setEnabled(true);
    STDtauVSlopeL->setEnabled(true); STDtauVSlopeE->setEnabled(true); STDtauVSlopeU->setEnabled(true);
  }
  if (index == 2) {
    connect(PlastParaBut, SIGNAL(clicked()), ODESTDP, SLOT(show()));
  }
}


void ChemSynDlg::exportData(CSynData &p)
{
  p.LUTables= (LUCombo->currentIndex() == 1);
  p.MgBlock= (MgBlockCombo->currentIndex() == 1);
  p.gSyn= gSynE->text().toDouble()*1e-9;
  p.VSyn= VSynE->text().toDouble()*1e-3;
  p.tauSyn= tauSynE->text().toDouble()*1e-3;
  p.VThresh= VThreshE->text().toDouble()*1e-3;
  p.VSlope= VSlopeE->text().toDouble()*1e-3;
  p.STD= STDCombo->currentIndex();
  p.STDAmpl= STDAmplE->text().toDouble();
  p.STDVThresh= STDVThreshE->text().toDouble()*1e-3;
  p.STDVSlope= STDVSlopeE->text().toDouble()*1e-3;
  p.STDtauAmpl= STDtauAmplE->text().toDouble();
  p.STDtau0= STDtau0E->text().toDouble()*1e-3;
  p.STDtauVThresh= STDtauVThreshE->text().toDouble()*1e-3;
  p.STDtauVSlope= STDtauVSlopeE->text().toDouble()*1e-3;
  p.fixVpost= fixVpostCombo->currentIndex();
  p.Vpost= VpostE->text().toDouble()*1e-3;
  p.Mgfac= MgfacE->text().toDouble();
  p.Mgexpo= MgexpoE->text().toDouble();
  p.Plasticity= PlasticityCombo->currentIndex();
  // ST plasticity
  STDP->exportData(p.ST);
  // ODE plasticity
  ODESTDP->exportData(p.ODE);

  sa.exportData(p.assign);
}

void ChemSynDlg::importData(CSynData p)
{
  QString num;   
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
  if (p.MgBlock) MgBlockCombo->setCurrentIndex(1);
  else MgBlockCombo->setCurrentIndex(0);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  num.setNum(p.VSyn*1e3);
  VSynE->setText(num);
  num.setNum(p.tauSyn*1e3);
  tauSynE->setText(num);
  num.setNum(p.VThresh*1e3);
  VThreshE->setText(num);
  num.setNum(p.VSlope*1e3);
  VSlopeE->setText(num);
  STDCombo->setCurrentIndex(p.STD);
  num.setNum(p.STDAmpl);
  STDAmplE->setText(num);
  num.setNum(p.STDVThresh*1e3);
  STDVThreshE->setText(num);
  num.setNum(p.STDVSlope*1e3);
  STDVSlopeE->setText(num);
  num.setNum(p.STDtauAmpl);
  STDtauAmplE->setText(num);
  num.setNum(p.STDtau0*1e3);
  STDtau0E->setText(num);
  num.setNum(p.STDtauVThresh*1e3);
  STDtauVThreshE->setText(num);
  num.setNum(p.STDtauVSlope*1e3);
  STDtauVSlopeE->setText(num);
  fixVpostCombo->setCurrentIndex(p.fixVpost);
  num.setNum(p.Vpost*1e3);
  VpostE->setText(num);
  num.setNum(p.Mgfac);
  MgfacE->setText(num);
  num.setNum(p.Mgexpo);
  MgexpoE->setText(num);
  PlasticityCombo->setCurrentIndex(p.Plasticity);
  // ST plasticity
  STDP->importData(p.ST);
  // ODE plasticity
  ODESTDP->importData(p.ODE);

  sa.importData(p.assign);
}
