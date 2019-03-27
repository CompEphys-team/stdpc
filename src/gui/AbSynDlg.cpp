#include <QDoubleSpinBox>
#include "AbSynDlg.h"
#include <QMessageBox>
#include "AbSyn.h"

abSynDlg::abSynDlg(size_t no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);
     
     STDP= new STDPDlg(this);
     ODESTDP= new ODESTDPDlg(this);

     setIndex(no);
     
     connect(PlasticityCombo, SIGNAL(currentIndexChanged(QString)), SLOT(PlastMethodChange()));
     connect(ResCloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(ResCloseClicked(QAbstractButton *)));

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<SynapseAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<SynapseAssignment>(&SynapseAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PreSynChannel, "Presyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PostSynChannel, "Postsyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::OutSynChannel, "Postsyn I", 95, out));
     AssignmentCellDouble<SynapseAssignment> *tmp = new AssignmentCellDouble<SynapseAssignment>
             (&SynapseAssignment::delay, "Delay (ms)", 95);
     tmp->setRange(0., 1000.);
     tmp->setDecimals(3);
     tmp->setFactor(1e-3);
     vec.push_back(tmp);
     vec.push_back(new AssignmentCellBool<SynapseAssignment>(&SynapseAssignment::save, "Save", 30));
     assignments->init(vec);
}

void abSynDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    QString lb = QString("%1 %2").arg(abSynProxy::get()->prettyName()).arg(no);
    abSynDlgLabel->setText(lb);
    STDP->setLabel(lb);
    ODESTDP->setLabel(lb);
}

void abSynDlg::ResCloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  if (but->text() == QString("Reset")) {
//    reset();
  }
  emit labelChanged(leLabel->text());
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

void abSynDlg::exportData()
{
  abSynData &p = abSynProxy::p[idx];
  p.label = leLabel->text();
  p.LUTables= (LUCombo->currentIndex() == 1);
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

  assignments->exportData(p.assign);
}

void abSynDlg::importData()
{
  abSynData &p = abSynProxy::p[idx];
  QString num;   
  leLabel->setText(p.label);
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);
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

  assignments->importData(p.assign);
}
