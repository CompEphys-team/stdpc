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

     QVector<AssignmentCellBase<CurrentAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<CurrentAssignment>(&CurrentAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::VChannel, "Input channel", 200, in));
     vec.push_back(new AssignmentCellChannel<CurrentAssignment>(&CurrentAssignment::IChannel, "Output channel", 200, out));
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
  p.factor = leFactor->text().toDouble() * 1e-6;

  assignments->exportData(p.assign);
}

void WireDlg::importData()
{
  WireData &p = WireProxy::p[idx];
  leLabel->setText(p.label);
  QString num;
  num.setNum(p.factor * 1e6);
  leFactor->setText(num);

  assignments->importData(p.assign);
}
