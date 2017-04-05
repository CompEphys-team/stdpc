#include "GapJunction.h"
#include "GapJunctionDlg.h"

GapJunctionDlg::GapJunctionDlg(int no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);
     
     label = gapJunctDlgLabel->text();
     setIndex(no);

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<GapJunctionAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<GapJunctionAssignment>(&GapJunctionAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preInChannel, "Presyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preOutChannel, "Presyn I", 95, out));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postInChannel, "Postsyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postOutChannel, "Postsyn I", 95, out));
     assignments->init(vec);
}

void GapJunctionDlg::exportData()
{
  GJunctData &p = GapJunctionProxy::p[idx];
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  assignments->exportData(p.assign);
}

void GapJunctionDlg::importData()
{
  GJunctData &p = GapJunctionProxy::p[idx];
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
