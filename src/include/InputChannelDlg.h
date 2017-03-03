#ifndef INPUTCHANNELDLG_H
#define INPUTCHANNELDLG_H


#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "ui_InputChannelDlg.h"
#include "ObjectDataTypes.h"
#include "Daq.h"

class InputChannelDlg : public QDialog, private Ui::InputChannelDlg
{
     Q_OBJECT

  public:
     InputChannelDlg(size_t idx, DAQProxy *proxy, QWidget *parent= 0);
     void init();
     void importData();
     virtual ~InputChannelDlg();

     inline void setIndex(size_t i) { idx = i; }
     
     int ChnNo;
     int lbNum;
     
  protected:
     QVector<double> inLow;
     QVector<double> inHigh;
     QVector<QCheckBox *> act;
     QVector<QComboBox *> rng;
     QVector<QLineEdit *> factor;
     QVector<QLabel *> allLabel;
     QVector<QCheckBox *> sDetect;
     QVector<QLineEdit *> SDThresh;
     QVector<QLineEdit *> bias;
     QVector<QCheckBox *> saveChnl;
     QVector<QPushButton *> calib;

     QVector<elecCalibParams> calibBackup;

     size_t idx;
     DAQProxy *proxy;

  public slots:
    void accept();
    void reject();
    void open();

    void exportData();
      
  private:
     void clearAll();
}; 

#endif
