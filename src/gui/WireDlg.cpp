#include <QDoubleSpinBox>
#include "WireDlg.h"
#include <QMessageBox>
#include "Wire.h"

WireDlg::WireDlg(int no, QWidget *parent)
     : ConductanceDlg(no, parent)
 {
     setupUi(this);

     setIndex(no);

     connect(CloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(CloseClicked(QAbstractButton *)));

     ChannelListModel *in = ChannelListModel::getModel(ChannelListModel::In | ChannelListModel::Blank);
     ChannelListModel *out = ChannelListModel::getModel(ChannelListModel::Out | ChannelListModel::Blank);

     QVector<AssignmentCellBase<SynapseAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<SynapseAssignment>(&SynapseAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PreSynChannel, "Input channel", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::PostSynChannel, "\"post\" channel", 95, in));
     vec.push_back(new AssignmentCellChannel<SynapseAssignment>(&SynapseAssignment::OutSynChannel, "Output channel", 95, out));
     assignments->init(vec);
}

void WireDlg::setIndex(size_t no)
{
    ConductanceDlg::setIndex(no);
    QString lb = QString("%1 %2").arg(WireProxy::get()->prettyName()).arg(no);
    WireDlgLabel->setText(lb);
}

void WireDlg::CloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
  emit labelChanged(leLabel->text());
}

void WireDlg::exportData()
{
  WireData &p = WireProxy::p[idx];
  p.label = leLabel->text();

  assignments->exportData(p.assign);
}

void WireDlg::importData()
{
  WireData &p = WireProxy::p[idx];
  leLabel->setText(p.label);
  QString num;   

  assignments->importData(p.assign);
}
