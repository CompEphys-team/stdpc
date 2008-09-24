
#include "GraphDlg.h"
#include <QMessageBox>

#define MINV -100
#define MAXV 60

GraphDlg::GraphDlg(int no, QWidget *parent)
     : QDialog(parent)
 {
   QColor clr;
   QString lb;
   
   setupUi(this);

   myNo= no;
   for (int i= 0; i < 4; i++) {
     clrCombo[i] = new QComboBox(this);
     clrCombo[i]->setObjectName(QString("clrCombo")+QString(i));
     clrCombo[i]->setGeometry(QRect(370, 40+30*i, 69, 22));
     clrCombo[i]->addItem(QString("red"));
     clrCombo[i]->addItem(QString("green"));
     clrCombo[i]->addItem(QString("blue"));
     clrCombo[i]->addItem(QString("black"));
     //clrCombo[i]->insertItems(0, clr.colorNames());
     clrCombo[i]->setCurrentIndex(i);
     ChannelCombo[i] = new QComboBox(this);
     ChannelCombo[i]->setObjectName(QString("ChannelCombo")+QString(i));
     ChannelCombo[i]->setGeometry(QRect(40, 40+30*i, 69, 22));
     lb= QString("None");
     ChannelCombo[i]->addItem(lb);
     lb= QString("SG");
     ChannelCombo[i]->addItem(lb);
     MinE[i] = new QLineEdit(this);
     MinE[i]->setObjectName(QString("MinE")+QString(i));
     MinE[i]->setGeometry(QRect(140, 40+30*i, 48, 20));
     lb.setNum(MINV);
     MinE[i]->setText(lb);
     MaxE[i] = new QLineEdit(this);
     MaxE[i]->setObjectName(QString("MaxE")+QString(i));
     MaxE[i]->setGeometry(QRect(210, 40+30*i, 48, 20));
     lb.setNum(MAXV);
     MaxE[i]->setText(lb);
     UnitCombo[i] = new QComboBox(this);
     UnitCombo[i]->setObjectName(QString("UnitCombo")+QString(i));
     UnitCombo[i]->setGeometry(QRect(280, 40+30*i, 69, 22));
     UnitCombo[i]->addItem(QString("mV"));
     UnitCombo[i]->addItem(QString("nA"));
   }
   inChnNo= 0;
   outChnNo= 0;
}

void GraphDlg::exportData(graphData &p)
{
  double yfac[2]= { 1e3, 1e9 };
  for (int i= 0; i < 4; i++) {
    p.color[i]= clrCombo[i]->currentText();
    p.chn[i]= ChannelCombo[i]->currentIndex();
    if (p.chn[i] > 0) p.active[i]= true;
    else p.active[i]= false;
    p.yfac[i]= yfac[UnitCombo[i]->currentIndex()];
    p.miny[i]= MinE[i]->text().toDouble()/p.yfac[i];
    p.maxy[i]= MaxE[i]->text().toDouble()/p.yfac[i];
  } 
  p.xrange= xRangeE->text().toDouble();
  p.dt= 1.0/dtE->text().toDouble();
  p.xtNo= xTicksE->text().toInt();
  p.ytNo= yTicksE->text().toInt();
}
  
void GraphDlg::importData(graphData p)
{
  QString num;
  
  for (int i= 0; i < 4; i++) {
    ChannelCombo[i]->setCurrentIndex(p.chn[i]);
    if (p.yfac[i] == 1e3) UnitCombo[i]->setCurrentIndex(0);
    else UnitCombo[i]->setCurrentIndex(1);
    num.setNum(p.miny[i]*p.yfac[i]);
    MinE[i]->setText(num);
    num.setNum(p.maxy[i]*p.yfac[i]);
    MaxE[i]->setText(num);
  } 
  num.setNum(p.xrange);
  xRangeE->setText(num);
  num.setNum(1.0/p.dt);
  dtE->setText(num);
  num.setNum(p.xtNo);
  xTicksE->setText(num);
  num.setNum(p.ytNo);
  yTicksE->setText(num);
}
  
void GraphDlg::updateOutChn(int chN, int *chns) 
{
  QString current;
  QString lb;
  int newInd;

  for (int j= 0; j < 4; j++) {
    current= ChannelCombo[j]->currentText();
    for (int i= 0; i < outChnNo; i++) {
      ChannelCombo[j]->removeItem(inChnNo+1);
    }
    for (int i= chN-1; i >=0; i--) {
      lb.setNum(chns[i]);
      ChannelCombo[j]->insertItem(inChnNo+1, QString("out")+lb);
    }
    newInd= ChannelCombo[j]->findText(current);
    if (newInd >= 0) ChannelCombo[j]->setCurrentIndex(newInd);
    else ChannelCombo[j]->setCurrentIndex(0);
  }
  outChnNo= chN;
}

void GraphDlg::updateInChn(int chN, int *chns) 
{
  QString current;
  QString lb;
  int newInd;
  
  for (int j= 0; j < 4; j++) {
    current= ChannelCombo[j]->currentText();
    for (int i= 0; i < inChnNo; i++) {
      ChannelCombo[j]->removeItem(1);
    }
    for (int i= chN-1; i >= 0; i--) {
      lb.setNum(chns[i]);
      ChannelCombo[j]->insertItem(1, QString("in")+lb);
    }
    newInd= ChannelCombo[j]->findText(current);
    if (newInd >= 0) ChannelCombo[j]->setCurrentIndex(newInd);
    else ChannelCombo[j]->setCurrentIndex(0);
  }
  inChnNo= chN;
}
