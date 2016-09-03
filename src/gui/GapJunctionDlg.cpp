
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
     connect(parent, SIGNAL(channelsChanged()), &gja, SLOT(updateChns()));
}

void GapJunctionDlg::exportData(GJunctData &p) 
{
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  gja.exportData(p.assign);
}

void GapJunctionDlg::importData(GJunctData p) 
{
  QString num;
  typeCombo->setCurrentIndex(p.type);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  gja.importData(p.assign);
}
