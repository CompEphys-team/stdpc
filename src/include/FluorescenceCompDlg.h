#ifndef FLUORESCENCECOMPDLG_H
#define FLUORESCENCECOMPDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QTimer>
#include "ChannelIndex.h"
#include "Daq.h"

namespace Ui {
class FluorescenceCompDlg;
}

class FluorescenceCompDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FluorescenceCompDlg(QLineEdit *gain, QLineEdit *bias, ChannelIndex dex, QWidget *parent = nullptr);
    ~FluorescenceCompDlg();

    void exportData();

public slots:
    void start();
    void stop();
    void acquire();
    void calculate();
    void updateInputs();

private:
    Ui::FluorescenceCompDlg *ui;
    QLineEdit *exportGain;
    QLineEdit *exportBias;
    ChannelIndex dex;
    DAQ *daq;
    inChannel *chan;
    QTimer dataTimer;
    QTimer calcTimer;

    double backupGain, backupBias;
    double gain, bias;
    double f0, df;

    static double V0, dV;

    void updateOutputs();
    void resetGraph();
};

#endif // FLUORESCENCECOMPDLG_H
