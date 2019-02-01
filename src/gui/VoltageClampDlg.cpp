#include <QDoubleSpinBox>
#include "VoltageClampDlg.h"
#include <QMessageBox>

VoltageClampDlg::VoltageClampDlg(int no, ChannelListModel *in, ChannelListModel *out, QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);

     label = VoltageClampDlgLabel->text();
     setIndex(no);
     
     connect(CloseBox, SIGNAL(clicked(QAbstractButton *)), SLOT(CloseClicked(QAbstractButton *)));

     QVector<AssignmentCellBase<VoltageClampAssignment>*> vec;
     vec.push_back(new AssignmentCellBool<VoltageClampAssignment>(&VoltageClampAssignment::active, "Active", 47));
     vec.push_back(new AssignmentCellChannel<VoltageClampAssignment>(&VoltageClampAssignment::PreSynChannel, "Command V", 95, in));
     vec.push_back(new AssignmentCellChannel<VoltageClampAssignment>(&VoltageClampAssignment::PostSynChannel, "Cell V", 95, in));
     vec.push_back(new AssignmentCellChannel<VoltageClampAssignment>(&VoltageClampAssignment::OutSynChannel, "Clamp I", 95, out));
     assignments->init(vec);
}

void VoltageClampDlg::setIndex(int no)
{
    QString lb = label.arg(no);
    VoltageClampDlgLabel->setText(lb);
}

void VoltageClampDlg::CloseClicked(QAbstractButton *but)
{
  if (but->text() == QString("Close")) {
    hide();
  }
}

void VoltageClampDlg::exportData(VoltageClampData &p)
{
  p.gP= gPE->text().toDouble()*1e-6;
  p.gI= gIE->text().toDouble()*1e-6;
  p.gP= gPE->text().toDouble()+1e-6;
  p.decayI= decayE->text().toDouble();
  p.tstepD= tDE->text().toInt();

  assignments->exportData(p.assign);
}

void VoltageClampDlg::importData(VoltageClampData p)
{
  QString num;   
  num.setNum(p.gP*1e6);
  gPE->setText(num);
  num.setNum(p.gI*1e6);
  gIE->setText(num);
  num.setNum(p.gD*1e6);
  gDE->setText(num);
  num.setNum(p.decayI);
  decayE->setText(num);
  num.setNum(p.tstepD);
  tDE->setText(num);

  assignments->importData(p.assign);
}
