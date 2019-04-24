#include <QDoubleSpinBox>
#include "ChemSynDlg.h"
#include <QMessageBox>
#include "ChemSyn.h"

ChemSynDlg::ChemSynDlg(size_t no, QWidget *parent)
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

void ChemSynDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    QString lb = QString("%1 %2").arg(ChemSynProxy::get()->prettyName()).arg(no);
    ChemSynDlgLabel->setText(lb);
    STDP->setLabel(lb);
    ODESTDP->setLabel(lb);
}

void ChemSynDlg::ResCloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  if (but->text() == QString("Reset")) {
//    reset();
  }
  emit labelChanged(leLabel->text());
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

void ChemSynDlg::exportData()
{
  CSynData &p = ChemSynProxy::p[idx];
  p.label = leLabel->text();
  p.LUTables= (LUCombo->currentIndex() == 1);

  p.gSyn= gSynE->text().toDouble()*1e-9;
  p.VSyn= VSynE->text().toDouble()*1e-3;
  p.tauSyn= tauSynE->text().toDouble()*1e-3;
  p.VThresh= VThreshE->text().toDouble()*1e-3;
  p.VSlope= VSlopeE->text().toDouble()*1e-3;

  p.STD= STD->isChecked();
  p.STDAmpl= STDAmplE->text().toDouble();
  p.STDVThresh= STDVThreshE->text().toDouble()*1e-3;
  p.STDVSlope= STDVSlopeE->text().toDouble()*1e-3;
  p.STDtauAmpl= STDtauAmplE->text().toDouble();
  p.STDtau0= STDtau0E->text().toDouble()*1e-3;
  p.STDtauVThresh= STDtauVThreshE->text().toDouble()*1e-3;
  p.STDtauVSlope= STDtauVSlopeE->text().toDouble()*1e-3;

  p.fixVpost= fixVpost->isChecked();
  p.Vpost= VpostE->text().toDouble()*1e-3;

  p.MgBlock= MgBlock->isChecked();
  p.Mgfac= MgfacE->text().toDouble();
  p.Mgexpo= MgexpoE->text().toDouble();

  p.stochastic = stochastic->isChecked();
  p.stoch_nRel = stoch_nRel->text().toInt();
  p.stoch_pRel = stoch_pRel->text().toDouble();
  p.stoch_variance = stoch_variance->text().toDouble();

  p.Plasticity= PlasticityCombo->currentIndex();
  // ST plasticity
  STDP->exportData(p.ST);
  // ODE plasticity
  ODESTDP->exportData(p.ODE);

  assignments->exportData(p.assign);
}

void ChemSynDlg::importData()
{
  CSynData &p = ChemSynProxy::p[idx];
  QString num;
  leLabel->setText(p.label);
  if (p.LUTables) LUCombo->setCurrentIndex(1);
  else LUCombo->setCurrentIndex(0);

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

  STD->setChecked(p.STD);
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

  fixVpost->setChecked(p.fixVpost);
  num.setNum(p.Vpost*1e3);
  VpostE->setText(num);

  MgBlock->setChecked(p.MgBlock);
  num.setNum(p.Mgfac);
  MgfacE->setText(num);
  num.setNum(p.Mgexpo);
  MgexpoE->setText(num);

  stochastic->setChecked(p.stochastic);
  stoch_nRel->setText(QString::number(p.stoch_nRel));
  stoch_pRel->setText(QString::number(p.stoch_pRel));
  stoch_variance->setText(QString::number(p.stoch_variance));

  PlasticityCombo->setCurrentIndex(p.Plasticity);
  // ST plasticity
  STDP->importData(p.ST);
  // ODE plasticity
  ODESTDP->importData(p.ODE);

  assignments->importData(p.assign);
}
