#ifndef ElectrodeCompDlg_H
#define ElectrodeCompDlg_H

#include <QDialog>
#include <QVector>
#include <QMessageBox>
#include "ObjectDataTypes.h"
#include "Calibrator.h"
#include "ChannelListModel.h"


namespace Ui {
    class ElectrodeCompDlg;
}

class ElectrodeCompDlg : public QDialog
{
    Q_OBJECT
    //Q_DISABLE_COPY(ElectrodeCompDlg)

    private:
        elecCalibParams &p;
        ChannelIndex dex;

        ChannelListModel *outChnModel;

        bool InitCalibrator();        
        void TimingTest();

        Ui::ElectrodeCompDlg *ui;

    public:                
        Calibrator calibrator;

        ElectrodeCompDlg(elecCalibParams &p, ChannelIndex dex, QWidget *parent = 0);
        ~ElectrodeCompDlg();
        void exportData();
        void importData();

    public slots:
        void MeasureElectrode();
        void MeasureMembrane();
        void CalibrateElectrode();
        void ToggleCompensation();
        
    signals:
        void message(QString message);

};

#endif // ElectrodeCompDlg_H
