#ifndef INPUTCHANNELDLG_H
#define INPUTCHANNELDLG_H


#include <QAbstractButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "ui_InputChannelDlg.h"
#include "ObjectDataTypes.h"
#include "DigiData.h"

class InputChannelDlg : public QDialog, private Ui::InputChannelDlg
{
     Q_OBJECT

  public:
     InputChannelDlg(QWidget *parent= 0);
     void init(DAQ *);
     void exportData();
     void importData();
     virtual ~InputChannelDlg();
     
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

  public slots:
    void accept();
    void reject();
      
  private:
     bool initial;
     void clearAll();
}; 

#endif
