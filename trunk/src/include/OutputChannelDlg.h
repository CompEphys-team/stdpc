#ifndef OUTPUTCHANNELDLG_H
#define OUTPUTCHANNELDLG_H


#include <QAbstractButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "ui_outputChannelDlg.h"
#include "ObjectDataTypes.h"
#include "DigiData.h"

class OutputChannelDlg : public QDialog, private Ui::OutputChannelDlg
{
     Q_OBJECT

  public:
     OutputChannelDlg(QWidget *parent= 0);
     void init(DAQ *);
     void exportData();
     void importData();
     virtual ~OutputChannelDlg();
     
     int ChnNo;
     int lbNum;

  protected:
     QVector<double> outGain;
     QVector<QCheckBox *> act;
     QVector<QComboBox *> rng;
     QVector<QLineEdit *> factor;
     QVector<QLineEdit *> bias;
     QVector<QLabel *> allLabel;
               
  public slots:
     void accept();
     void reject();
     void appear();
     
  private:
     bool initial;
     void clearAll();

  signals:       
     void updateOutChn(int, int *);
}; 

#endif
