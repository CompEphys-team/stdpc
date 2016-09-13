
#include "GraphDlg.h"
#include <QMessageBox>

#define MINV -100
#define MAXV 60

GraphDlg::GraphDlg(int no, QWidget *parent)
     : QDialog(parent),
       clm(new ChannelListModel(ChannelListModel::AnalogIn
                                | ChannelListModel::AnalogOut
                                | ChannelListModel::None
                                | ChannelListModel::SpikeGen
                                | ChannelListModel::Virtual, this))
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
     clm->subordinate(ChannelCombo[i]);
     ChannelCombo[i]->setCurrentIndex(0);
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

   connect(parent, SIGNAL(channelsChanged()), clm, SLOT(updateChns()));
}

void GraphDlg::exportData(graphData &p)
{
  double yfac[2]= { 1e3, 1e9 };
  for (int i= 0; i < 4; i++) {
    p.color[i]= clrCombo[i]->currentText();
    p.chn[i] = ChannelCombo[i]->currentData().value<ChannelIndex>();
    p.active[i] = p.chn[i].isValid && !p.chn[i].isNone;
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
    ChannelCombo[i]->setCurrentIndex(clm->index(p.chn[i]));
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
