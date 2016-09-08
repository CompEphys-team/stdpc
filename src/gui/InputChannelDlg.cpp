#include "InputChannelDlg.h"
#include <QMessageBox>

InputChannelDlg::InputChannelDlg(QWidget *parent)
     : QDialog(parent)
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
}

void InputChannelDlg::init(DAQ *board)
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

  QCheckBox *cbtmp;
  QComboBox *qctmp;
  QLineEdit *letmp;
  QLabel *lb;
  QString nm;

  clearAll();
  ChnNo= board->inChnNo;
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
  lb->setText(QString("Aquisition Range"));
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
  }
  QRect geo= this->geometry();
  geo.setHeight(Y0+ChnNo*DY+60);
  this->resize(geo.size());
  geo= buttonBox->geometry();
  geo.moveBottom(Y0+ChnNo*DY+50);
  buttonBox->setGeometry(geo);
//  QMessageBox::warning((QWidget *)parent(), tr("My Application"),
//                tr("The signal for index change has been received"),
//               QMessageBox::Close);
  accept();   // export the current state, update chn info in other dialogs
}    

InputChannelDlg::~InputChannelDlg()
{
  clearAll();
}

void InputChannelDlg::exportData()
{
  for (int i= 0; i < ChnNo; i++) {
    inChnp[i].active= (act[i]->checkState() > 0);
    inChnp[i].gain= rng[i]->currentIndex();
    inChnp[i].gainFac= factor[i]->text().toDouble();
    inChnp[i].spkDetect= (sDetect[i]->checkState() > 0);
    inChnp[i].spkDetectThresh= SDThresh[i]->text().toDouble()*1e-3;
    inChnp[i].bias= bias[i]->text().toDouble()*1e-3;
    inChnp[i].minVoltage = inLow[rng[i]->currentIndex()]*inChnp[i].gainFac;
    inChnp[i].maxVoltage = inHigh[rng[i]->currentIndex()]*inChnp[i].gainFac;
    inChnp[i].chnlSaving= (saveChnl[i]->checkState() > 0);
  }
}

void InputChannelDlg::importData()
{
  QString lb;
  
  for (int i= 0; i < ChnNo; i++) {
    if (inChnp[i].active) act[i]->setCheckState(Qt::Checked);
    else act[i]->setCheckState(Qt::Unchecked);
    rng[i]->setCurrentIndex(inChnp[i].gain); 
    lb.setNum(inChnp[i].gainFac);
    factor[i]->setText(lb);
    if (inChnp[i].spkDetect) sDetect[i]->setCheckState(Qt::Checked);
    else sDetect[i]->setCheckState(Qt::Unchecked);
    lb.setNum(inChnp[i].spkDetectThresh*1e3);
    SDThresh[i]->setText(lb);
    lb.setNum(inChnp[i].bias*1e3);
    bias[i]->setText(lb);
    if (inChnp[i].chnlSaving) saveChnl[i]->setCheckState(Qt::Checked);
    else saveChnl[i]->setCheckState(Qt::Unchecked);
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
  QDialog::reject();
}


