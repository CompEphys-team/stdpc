
#include "SpikeTimeDlg.h"

SpikeTimeDlg::SpikeTimeDlg(QWidget *parent)
     : QDialog(parent)
 {
     setupUi(this);
}

void SpikeTimeDlg::exportData()
{
  SGp.SpikeT[0]= isi0E->text().toDouble()/1e3;
  SGp.SpikeT[1]= isi1E->text().toDouble()/1e3;
  SGp.SpikeT[2]= isi2E->text().toDouble()/1e3;
  SGp.SpikeT[3]= isi3E->text().toDouble()/1e3;
  SGp.SpikeT[4]= isi4E->text().toDouble()/1e3;
  SGp.SpikeT[5]= isi5E->text().toDouble()/1e3;
  SGp.SpikeT[6]= isi6E->text().toDouble()/1e3;
  SGp.SpikeT[7]= isi7E->text().toDouble()/1e3;
  SGp.SpikeT[8]= isi8E->text().toDouble()/1e3;
  SGp.SpikeT[9]= isi9E->text().toDouble()/1e3;
}

void SpikeTimeDlg::importData()
{
  QString num;
  
  num.setNum(SGp.SpikeT[0]*1e3);
  isi0E->setText(num);
  num.setNum(SGp.SpikeT[1]*1e3);
  isi1E->setText(num);
  num.setNum(SGp.SpikeT[2]*1e3);
  isi2E->setText(num);
  num.setNum(SGp.SpikeT[3]*1e3);
  isi3E->setText(num);
  num.setNum(SGp.SpikeT[4]*1e3);
  isi4E->setText(num);
  num.setNum(SGp.SpikeT[5]*1e3);
  isi5E->setText(num);
  num.setNum(SGp.SpikeT[6]*1e3);
  isi6E->setText(num);
  num.setNum(SGp.SpikeT[7]*1e3);
  isi7E->setText(num);
  num.setNum(SGp.SpikeT[8]*1e3);
  isi8E->setText(num);
  num.setNum(SGp.SpikeT[9]*1e3);
  isi9E->setText(num);
}
