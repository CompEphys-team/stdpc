#ifndef MEASMETHODDLG_H
#define MEASMETHODDLG_H

#include <QtGui/QDialog>
#include <QVector>
#include <QMessageBox>
#include "ui_MeasMethodDlg.h"
#include "ObjectDataTypes.h"
#include "DCThread.h"

namespace Ui {
    class MeasMethodDlg;
}

class MeasMethodDlg : public QDialog, private Ui::MeasMethodDlg
{
    Q_OBJECT
    //Q_DISABLE_COPY(MeasMethodDlg)

    public:
        DAQ *board;
        DCThread *DCT;
        mmParams params;
        double samplingPeriod;
        long int numberOfSamples;
        long int sqLen;

        QVector<double> currents;
        QVector<double> voltages;
        QVector<double> beforeCompVs;
        QVector<double> afterCompVs;
        QVector<double> controlVs;
        QVector<double> times;

        inChannel *inChn;
        outChannel *outChn;

        MeasMethodDlg(QWidget *parent = 0);
        ~MeasMethodDlg();
        void exportParams();
        void importParams();
        void saveData(QString &fname, QVector<double> data);        
        bool generalInit(bool);
        bool initChannels(bool);
        void timeTest();
        void createInputCurrent(bool);


    protected:
        virtual void changeEvent(QEvent *e);

    public slots:
        void newMethodSelected();
        void newInputTypeSelected();
        void checkLin();
        void calibrate();
        void testKernel();
        void loadKernel();
        
    signals:
        void message(QString message);

};

#endif // MEASMETHODDLG_H
