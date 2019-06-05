/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QDoubleSpinBox>
#include "ChemSynDlg.h"
#include <QMessageBox>

ChemSynDlg::ChemSynDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
     
     STDP= new STDPDlg(this);
     ODESTDP= new ODESTDPDlg(this);

     label = ChemSynDlgLabel->text();
     setIndex(no);
     
     connect(PlasticityCombo, SIGNAL(currentIndexChanged(QString)), SLOT(PlastMethodChange()));
     connect(ResCloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(ResCloseClicked(QAbstractButton *)));
     connect(STDCombo, SIGNAL(currentIndexChanged(QString)), SLOT(STDComboChange()));

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
     assignments->init(vec);
}

void ChemSynDlg::setIndex(int no)
{
    QString lb = label.arg(no);
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

  assignments->exportData(p.assign);
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

  assignments->importData(p.assign);
}
