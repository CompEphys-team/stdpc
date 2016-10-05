
#include "GapJunctionDlg.h"

GapJunctionDlg::GapJunctionDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
     
     label = gapJunctDlgLabel->text();
     setIndex(no);

     QVector<Dropdown<GapJunctionAssignment>> vec;
     vec.push_back(Dropdown<GapJunctionAssignment>(&GapJunctionAssignment::preInChannel, in, "Presyn V", 95));
     vec.push_back(Dropdown<GapJunctionAssignment>(&GapJunctionAssignment::preOutChannel, out, "Presyn I", 95));
     vec.push_back(Dropdown<GapJunctionAssignment>(&GapJunctionAssignment::postInChannel, in, "Postsyn V", 95));
     vec.push_back(Dropdown<GapJunctionAssignment>(&GapJunctionAssignment::postOutChannel, out, "Postsyn I", 95));
     assignments->init(vec);
}

void GapJunctionDlg::exportData(GJunctData &p) 
{
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  assignments->exportData(p.assign);
}

void GapJunctionDlg::importData(GJunctData p) 
{
  QString num;
  typeCombo->setCurrentIndex(p.type);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  assignments->importData(p.assign);
}

void GapJunctionDlg::setIndex(int no)
{
    gapJunctDlgLabel->setText(label.arg(no));
}
