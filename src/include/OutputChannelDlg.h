#ifndef OUTPUTCHANNELDLG_H
#define OUTPUTCHANNELDLG_H

#include <QAbstractButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include "ui_OutputChannelDlg.h"
#include "Daq.h"

class OutputChannelDlg : public QDialog, private Ui::OutputChannelDlg
{
     Q_OBJECT

  public:
     OutputChannelDlg(size_t idx, DAQProxy *proxy, QWidget *parent= nullptr);
     void init();
     void exportData();
     void importData();
     virtual ~OutputChannelDlg();

     inline void setIndex(size_t i) { idx = i; }
     
     int ChnNo;
     int lbNum;

  protected:
     QVector<double> outLow;
     QVector<double> outHigh;
     QVector<QCheckBox *> act;
     QVector<QComboBox *> rng;
     QVector<QLineEdit *> factor;
     QVector<QLineEdit *> bias;
     QVector<QLabel *> allLabel;
     QVector<QCheckBox *> saveChnl;

     size_t idx;
     DAQProxy *proxy;
               
  public slots:
     void accept();
     void reject();
     
  private:
     void clearAll();
}; 

#endif
