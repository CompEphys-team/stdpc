#include "GapJunction.h"
#include "GapJunctionDlg.h"

GapJunctionDlg::GapJunctionDlg(size_t no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);
     
     setIndex(no);

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<GapJunctionAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<GapJunctionAssignment>(&GapJunctionAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preInChannel, "Presyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::preOutChannel, "Presyn I", 95, out));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postInChannel, "Postsyn V", 95, in));
     vec.push_back(new AssignmentCellChannel<GapJunctionAssignment>(&GapJunctionAssignment::postOutChannel, "Postsyn I", 95, out));
     vec.push_back(new AssignmentCellBool<GapJunctionAssignment>(&GapJunctionAssignment::save, "Save", 30));
     assignments->init(vec);
}

void GapJunctionDlg::exportData()
{
  GJunctData &p = GapJunctionProxy::p[idx];
  p.label = leLabel->text();
  p.type= typeCombo->currentIndex();
  p.gSyn= gSynE->text().toDouble()*1e-9;
  assignments->exportData(p.assign);
}

void GapJunctionDlg::importData()
{
  GJunctData &p = GapJunctionProxy::p[idx];
  QString num;
  leLabel->setText(p.label);
  typeCombo->setCurrentIndex(p.type);
  num.setNum(p.gSyn*1e9);
  gSynE->setText(num);
  assignments->importData(p.assign);
}

void GapJunctionDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    gapJunctDlgLabel->setText(QString("%1 %2").arg(GapJunctionProxy::get()->prettyName()).arg(no));
}

void GapJunctionDlg::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( button->text() == "Close" )
        hide();
    emit labelChanged(leLabel->text());
}
