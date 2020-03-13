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

#include "InputChannelDlg.h"
#include <QMessageBox>
#include "ElectrodeCompDlg.h"

InputChannelDlg::InputChannelDlg(size_t idx, DAQProxy *proxy, QWidget *parent) :
    QDialog(parent),
    idx(idx),
    proxy(proxy)
{
  setupUi(this);
}

void InputChannelDlg::clearAll()
{
  QVector<QCheckBox *>::Iterator actIter;
  QVector<QComboBox *>::Iterator rngIter;
  QVector<QLineEdit *>::Iterator factorIter;
  QVector<QLabel *>::Iterator lbIter;
  QVector<QCheckBox *>::Iterator saveIter;
  
  for (actIter= act.begin(); actIter != act.end(); actIter++) {
    delete *actIter;
  }
  act.clear();
  for (rngIter= rng.begin(); rngIter != rng.end(); rngIter++) {
    delete *rngIter;
  }
  rng.clear();
  for (factorIter= factor.begin(); factorIter != factor.end(); factorIter++) {
    delete *factorIter;
  }
  factor.clear();
  for (lbIter= allLabel.begin(); lbIter != allLabel.end(); lbIter++) {
    delete *lbIter;
  }
  allLabel.clear();
  for (actIter= sDetect.begin(); actIter != sDetect.end(); actIter++) {
    delete *actIter;
  }
  sDetect.clear();
  for (factorIter= SDThresh.begin(); factorIter != SDThresh.end(); factorIter++) {
    delete *factorIter;
  }
  SDThresh.clear();
  for (factorIter= bias.begin(); factorIter != bias.end(); factorIter++) {
    delete *factorIter;
  }
  bias.clear();
  for (saveIter= saveChnl.begin(); saveIter != saveChnl.end(); saveIter++) {
    delete *saveIter;
  }
  saveChnl.clear();
  for ( QPushButton *btn : calib ) {
      delete btn;
  }
  calib.clear();
}

void InputChannelDlg::init()
{
  #define Y0 100
  #define DY 22
  #define X0 30
  #define X1 120
  #define X2 250
  #define X3 320
  #define X4 420
  #define X5 480
  #define X6 555
  #define X7 600
  #define X8 675
  #define X9 700
  #define X10 750

  QCheckBox *cbtmp;
  QComboBox *qctmp;
  QLineEdit *letmp;
  QPushButton *btntmp;
  QLabel *lb;
  QString nm;

  DAQ *board = Devices.getDevice(ChannelIndex(proxy, idx));

  clearAll();
  ChnNo= board->inChnNo;
  board->params()->inChn.resize(ChnNo);
  inLow = QVector<double>(board->inGainNo);
  for(int i= 0; i < board->inGainNo; i++){
    inLow[i]= board->inLow[i];
  }
  inHigh = QVector<double>(board->inGainNo);
  for(int i= 0; i < board->inGainNo; i++){
    inHigh[i]= board->inHigh[i];
  }
  lbNum= 0;

  lb= new QLabel(this);
  lb->setGeometry(QRect(X1, Y0-45, 100, 36));
  lb->setText(QString("Acquisition Range"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X2, Y0-45, 120, 36));
  lb->setText(QString("Conversion factor"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X4, Y0-45, 80, 36));
  lb->setText(QString("   Spike\nDetection"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X5, Y0-45, 120, 36));
  lb->setText(QString("Detection Threshold"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X7, Y0-45, 60, 36));
  lb->setText(QString("Bias Voltage"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X9, Y0-45, 60, 36));
  lb->setText(QString("  Save\nChannel"));
  allLabel.append(lb);

  for (int i= 0; i < ChnNo; i++) {
    nm.setNum(i);
    cbtmp= new QCheckBox(this);
    cbtmp->setGeometry(QRect(X0, Y0+i*DY, 70, 18));
    cbtmp->setObjectName(QString("Analog In ")+nm);
    cbtmp->setText(QString("Analog ")+nm);
    if (i < 2) cbtmp->setCheckState(Qt::Checked);
    connect(cbtmp, SIGNAL(clicked(bool)), this, SLOT(exportData()));
    act.append(cbtmp);
    qctmp= new QComboBox(this);
    qctmp->setGeometry(QRect(X1, Y0+i*DY, 100, 18));
    for (int j= 0; j < board->inGainNo; j++) {
      qctmp->addItem(QString(board->inGainText[j]));
    }
    qctmp->setCurrentIndex(board->inGainNo-1);
    rng.append(qctmp);
    letmp= new QLineEdit(this);
    letmp->setGeometry(QRect(X2, Y0+i*DY, 60, 18));
    nm.setNum(0.1);
    letmp->setText(nm);
    letmp->setAlignment(Qt::AlignRight);
    factor.append(letmp);
    lb= new QLabel(this);
    lb->setGeometry(QRect(X3, Y0+i*DY, 120, 18));
    lb->setText(QString("V [world]/V [ADC]"));
    allLabel.append(lb);
    cbtmp= new QCheckBox(this);
    cbtmp->setGeometry(QRect(X4+15, Y0+i*DY, 15, 18));
    cbtmp->setObjectName(QString("Spike Detect ")+nm);
    sDetect.append(cbtmp);
    letmp= new QLineEdit(this);
    letmp->setGeometry(QRect(X5, Y0+i*DY, 60, 18));
    nm.setNum(-10);
    letmp->setText(nm);
    letmp->setAlignment(Qt::AlignRight);
    SDThresh.append(letmp);
    lb= new QLabel(this);
    lb->setGeometry(QRect(X6, Y0+i*DY, 40, 18));
    lb->setText(QString("mV"));
    allLabel.append(lb);
    letmp= new QLineEdit(this);
    letmp->setGeometry(QRect(X7, Y0+i*DY, 60, 18));
    nm.setNum(0);
    letmp->setText(nm);
    letmp->setAlignment(Qt::AlignRight);
    bias.append(letmp);
    lb= new QLabel(this);
    lb->setGeometry(QRect(X8, Y0+i*DY, 40, 18));
    lb->setText(QString("mV"));
    allLabel.append(lb);
    cbtmp= new QCheckBox(this);
    cbtmp->setGeometry(QRect(X9+15, Y0+i*DY, 15, 18));
    cbtmp->setObjectName(QString("Save Channel ")+nm);
    saveChnl.append(cbtmp);
    btntmp= new QPushButton("Calibration", this);
    btntmp->setGeometry(QRect(X10, Y0+i*DY-1, 70, 20));
    btntmp->setObjectName(QString("Calibrate ")+nm);
    connect(btntmp, &QPushButton::clicked, [=](){
        ChannelIndex dex(proxy, idx, i, true);
        ElectrodeCompDlg *calibDlg = new ElectrodeCompDlg(proxy->param(idx).inChn[i].calib, dex, this);
        connect(calibDlg, SIGNAL(message(QString)), this->parent(), SIGNAL(message(QString)));
        connect(&calibDlg->calibrator, SIGNAL(CloseToLimit(QString,QString,double,double,double)),
                this->parent(), SIGNAL(CloseToLimit(QString,QString,double,double,double)));
        if ( calibDlg->exec() == QDialog::Accepted )
            calibDlg->exportData();
        delete calibDlg;
    });
    btntmp->setEnabled(false);
    calib.append(btntmp);
  }
  QRect geo= this->geometry();
  geo.setHeight(Y0+ChnNo*DY+60);
  this->resize(geo.size());
  geo= buttonBox->geometry();
  geo.moveBottom(Y0+ChnNo*DY+50);
  buttonBox->setGeometry(geo);
}    

InputChannelDlg::~InputChannelDlg()
{
  clearAll();
}

void InputChannelDlg::exportData()
{
  DAQData *p =& proxy->param(idx);
  for (int i= 0; i < ChnNo && size_t(i) < p->inChn.size(); i++) {
    p->inChn[i].active= (act[i]->checkState() > 0);
    p->inChn[i].gain= rng[i]->currentIndex();
    double gainFac = factor[i]->text().toDouble();
    p->inChn[i].gainFac= gainFac;
    p->inChn[i].spkDetect= (sDetect[i]->checkState() > 0);
    p->inChn[i].spkDetectThresh= SDThresh[i]->text().toDouble()*1e-3;
    p->inChn[i].bias= bias[i]->text().toDouble()*1e-3;
    p->inChn[i].minVoltage = inLow[rng[i]->currentIndex()]*gainFac;
    p->inChn[i].maxVoltage = inHigh[rng[i]->currentIndex()]*gainFac;
    p->inChn[i].chnlSaving= (saveChnl[i]->checkState() > 0);
    calib[i]->setEnabled(Devices.getDevice(ChannelIndex(proxy, idx))->initialized
                         && p->inChn[i].active && proxy->daqClass() != "SimulDAQ");
  }
}

void InputChannelDlg::importData()
{
  DAQData *p =& proxy->param(idx);
  for (int i= 0; i < ChnNo; i++) {
    act[i]->setChecked(p->inChn[i].active);
    if ( p->inChn[i].gain >= 0 && p->inChn[i].gain < rng[i]->count() )
        rng[i]->setCurrentIndex(p->inChn[i].gain);
    else
        rng[i]->setCurrentIndex(rng[i]->count()-1);
    factor[i]->setText(QString::number(p->inChn[i].gainFac));
    sDetect[i]->setChecked(p->inChn[i].spkDetect);
    SDThresh[i]->setText(QString::number(p->inChn[i].spkDetectThresh*1e3));
    bias[i]->setText(QString::number(p->inChn[i].bias*1e3));
    saveChnl[i]->setChecked(p->inChn[i].chnlSaving);
  }
}

void InputChannelDlg::accept()
{
  exportData();
  QDialog::accept();
}

void InputChannelDlg::reject()
{
  importData();
  DAQData *p =& proxy->param(idx);
  for ( int i = 0; i < ChnNo; i++ )
      p->inChn[i].calib = calibBackup[i];
  QDialog::reject();
}

void InputChannelDlg::open()
{
    DAQData *p =& proxy->param(idx);
    calibBackup.resize(ChnNo);
    for ( int i = 0; i < ChnNo; i++ )
        calibBackup[i] = p->inChn[i].calib;
    QDialog::open();
}
