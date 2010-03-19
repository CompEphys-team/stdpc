#ifndef ElectrodeCompDlg_H
#define ElectrodeCompDlg_H

#include <QtGui/QDialog>
#include <QVector>
#include <QMessageBox>
#include "ui_ElectrodeCompDlg.h"
#include "ObjectDataTypes.h"
#include "Calibrator.h"


namespace Ui {
    class ElectrodeCompDlg;
}

class ElectrodeCompDlg : public QDialog, private Ui::ElectrodeCompDlg
{
    Q_OBJECT
    //Q_DISABLE_COPY(ElectrodeCompDlg)

    private:       
        int tabNum;

        //------------------------------------------------------//
        // Objects to store the GUI elements of the current tab
            QLabel *lInfo;
            QCheckBox *cbCompOn;

            // General group box
            QLineEdit *leGenInjLen;
            QLineEdit *leGenSamplingRate;
            QLineEdit *leGenInChannelNum;
            QLineEdit *leGenOutChannelNum;

            // Electrode measurement group box
            QLineEdit *leElecMaxCurrent;
            QLineEdit *leElecMinCurrent;
            QLineEdit *leElecLevelNum;

            // Membrane measurement group box
            QLineEdit *leMembIStep;
            QLineEdit *leMembRepNum;

            // Calibration group box
            QLineEdit *leHyperpolCurr;
            QLineEdit *leCalInjAmp;
            QLineEdit *leCalFKLen;
            QLineEdit *leCalEKLen;

            // Electrode results group box
            QLineEdit *leElecMeasRes;
            QLineEdit *leElecMeasTime;
            QLineEdit *leElecCalibRes;
            QLineEdit *leElecResStd;
            QLineEdit *leElecTimeStd;
            QLineEdit *leElecCalibTime;

            // Membrane results group box
            QLineEdit *leCellMeasRes;
            QLineEdit *leCellMeasTime;
            QLineEdit *leCellResStd;
            QLineEdit *leCellTimeStd;
            QLineEdit *leCellCalibRes;
            QLineEdit *leCellCalibTime;

            // Data acquisition results group box
            QLineEdit *leAverSamplRate;
            QLineEdit *leSTDSamplingRate;
            QLineEdit *leMinSRate;
            QLineEdit *leDesPerMinRate;
            QLineEdit *leMaxSTime;

        //------------------------------------------------------//

        int numOfElectrodes;

        elecCalibParams params;

        void GetGuiElements();
        bool InitCalibrator();        
        void ExportParams();
        void ImportParams();
        void TimingTest();

    public:        
        Calibrator *calibrator;
        ElectrodeCompDlg(QWidget *parent = 0);
        ~ElectrodeCompDlg();

    public slots:
        void MeasureElectrode();
        void MeasureMembrane();
        void CalibrateElectrode();
        void ToggleCompensation();
        
    signals:
        void message(QString message);

};

#endif // ElectrodeCompDlg_H
