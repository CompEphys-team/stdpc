#include "OutputChannelDlg.h"

OutputChannelDlg::OutputChannelDlg(QWidget *parent)
     : QDialog(parent)
{
  setupUi(this);
}

void OutputChannelDlg::clearAll()
{
  QVector<QCheckBox *>::Iterator actIter;
  QVector<QComboBox *>::Iterator rngIter;
  QVector<QLineEdit *>::Iterator factorIter;
  QVector<QLineEdit *>::Iterator biasIter;
  QVector<QLabel *>::Iterator lbIter;
  
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
  for (biasIter= bias.begin(); biasIter != bias.end(); biasIter++) {
    delete *biasIter;
  }
  bias.clear();
  for (lbIter= allLabel.begin(); lbIter != allLabel.end(); lbIter++) {
    delete *lbIter;
  }
  allLabel.clear();
}


void OutputChannelDlg::init(DAQ *board)
{
  #define Y0 90
  #define DY 22
  #define X0 30
  #define X1 120
  #define X2 250
  #define X3 320
  #define X4 480
  #define X5 550
  
  QLabel *lb;
  QCheckBox *cbtmp;
  QComboBox *qctmp;
  QLineEdit *letmp; 
  QString nm;
     
  clearAll();
  ChnNo= board->outChnNo;
 
  lb= new QLabel(this);
  lb->setGeometry(QRect(X1, Y0-40, 100, 36));
  lb->setText(QString("Aquisition Range"));
  allLabel.append(lb);
  
  lb= new QLabel(this);
  lb->setGeometry(QRect(X2, Y0-40, 120, 36));
  lb->setText(QString("Conversion factor"));
  allLabel.append(lb);

  lb= new QLabel(this);
  lb->setGeometry(QRect(X4, Y0-40, 120, 36));
  lb->setText(QString("Bias current"));
  allLabel.append(lb);
  
  for (int i= 0; i < ChnNo; i++) {
    nm.setNum(i);
    cbtmp= new QCheckBox(this);
    cbtmp->setGeometry(QRect(X0, Y0+i*DY, 70, 18));
    cbtmp->setObjectName(QString("Analog Out ")+nm);
    cbtmp->setText(QString("Analog ")+nm);
    if (i < 2) cbtmp->setCheckState(Qt::Checked);
    act.append(cbtmp);
    qctmp= new QComboBox(this);
    qctmp->setGeometry(QRect(X1, Y0+i*DY, 100, 18));
    for (int j= 0; j < board->outGainNo; j++) {
      qctmp->addItem(QString(board->outGainText[j]));
    }
    qctmp->setCurrentIndex(board->outGainNo-1);
    rng.append(qctmp);
    letmp= new QLineEdit(this);
    letmp->setGeometry(QRect(X2, Y0+i*DY, 60, 18));
    nm.setNum(0.1);
    letmp->setText(nm);
    factor.append(letmp);
    lb= new QLabel(this);
    lb->setGeometry(QRect(X3, Y0+i*DY, 120, 18));
    lb->setText(QString("V [DAC]/nA [world]"));
    allLabel.append(lb);
    letmp= new QLineEdit(this);
    letmp->setGeometry(QRect(X4, Y0+i*DY, 60, 18));
    nm.setNum(0.0);
    letmp->setText(nm);
    bias.append(letmp);
    lb= new QLabel(this);
    lb->setGeometry(QRect(X5, Y0+i*DY, 120, 18));
    lb->setText(QString("nA"));
    allLabel.append(lb);
  }
  QRect geo= this->geometry();
  geo.setHeight(Y0+ChnNo*DY+60);
  this->setGeometry(geo);
  geo= buttonBox->geometry();
  geo.moveBottom(Y0+ChnNo*DY+50);
  buttonBox->setGeometry(geo);
  accept();  // export the current state, update chn info in other dialogs
}    

OutputChannelDlg::~OutputChannelDlg()
{
  clearAll();
}



void OutputChannelDlg::exportData()
{
  for (int i= 0; i < ChnNo; i++) {
    outChnp[i].active= (act[i]->checkState() > 0);
    outChnp[i].gain= rng[i]->currentIndex();
    outChnp[i].gainFac= factor[i]->text().toDouble();
    outChnp[i].bias= bias[i]->text().toDouble()*1e-9;
  }
}

void OutputChannelDlg::importData()
{
  QString lb;
  
  for (int i= 0; i < ChnNo; i++) {
    if (outChnp[i].active) act[i]->setCheckState(Qt::Checked);
    else act[i]->setCheckState(Qt::Unchecked);
    rng[i]->setCurrentIndex(inChnp[i].gain); 
    lb.setNum(outChnp[i].gainFac);
    factor[i]->setText(lb);
    lb.setNum(outChnp[i].bias*1e9);
    bias[i]->setText(lb);
  }
}

void OutputChannelDlg::accept()
{
  int chns[ChnNo];
  int chN= 0;
  
  exportData();
  for (int i= 0; i < ChnNo; i++) {
    if (act[i]->checkState() > 0) chns[chN++]= i;
  }
  updateOutChn(chN, chns);
  ((QWidget *)parent())->setEnabled(true);
  hide();
}

void OutputChannelDlg::reject()
{
  importData();
  ((QWidget *)parent())->setEnabled(true);
  hide();
}

void OutputChannelDlg::appear()
{
  ((QWidget *)parent())->setEnabled(false);
  this->setEnabled(true);
  show();
}
