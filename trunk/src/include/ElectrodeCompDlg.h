#ifndef ElectrodeCompDlg_H
#define ElectrodeCompDlg_H

#include <QDialog>
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
        int elecNum;

        //------------------------------------------------------//
        // Objects to store the GUI elements of the tabs

            // Buttons
            QPushButton *elecMeasButton[MAX_ELECTRODE_NO];
            QPushButton *membMeasButton[MAX_ELECTRODE_NO];
            QPushButton *calibrateButton[MAX_ELECTRODE_NO];

            // Header
            QLabel *lInfo[MAX_ELECTRODE_NO];
            QCheckBox *cbCompOn[MAX_ELECTRODE_NO];

            // General group box
            QLineEdit *leGenSamplingRate[MAX_ELECTRODE_NO];
            QLineEdit *leGenInChannelNum[MAX_ELECTRODE_NO];
            QLineEdit *leGenOutChannelNum[MAX_ELECTRODE_NO];

            // Electrode measurement group box
            QLineEdit *leElecMaxCurrent[MAX_ELECTRODE_NO];
            QLineEdit *leElecMinCurrent[MAX_ELECTRODE_NO];
            QLineEdit *leElecLevelNum[MAX_ELECTRODE_NO];
            QLineEdit *leElecInjLen[MAX_ELECTRODE_NO];

            // Membrane measurement group box
            QLineEdit *leMembIStep[MAX_ELECTRODE_NO];
            QLineEdit *leMembRepNum[MAX_ELECTRODE_NO];
            QLineEdit *leMembInjLen[MAX_ELECTRODE_NO];

            // Calibration group box
            QLineEdit *leHyperpolCurr[MAX_ELECTRODE_NO];
            QLineEdit *leCalInjAmp[MAX_ELECTRODE_NO];
            QLineEdit *leCalFKLen[MAX_ELECTRODE_NO];
            QLineEdit *leCalEKLen[MAX_ELECTRODE_NO];
            QLineEdit *leCalInjLen[MAX_ELECTRODE_NO];

            // Electrode results group box
            QLineEdit *leElecMeasRes[MAX_ELECTRODE_NO];
            QLineEdit *leElecMeasTime[MAX_ELECTRODE_NO];
            QLineEdit *leElecCalibRes[MAX_ELECTRODE_NO];
            QLineEdit *leElecResStd[MAX_ELECTRODE_NO];
            QLineEdit *leElecTimeStd[MAX_ELECTRODE_NO];
            QLineEdit *leElecCalibTime[MAX_ELECTRODE_NO];

            // Membrane results group box
            QLineEdit *leCellMeasRes[MAX_ELECTRODE_NO];
            QLineEdit *leCellMeasTime[MAX_ELECTRODE_NO];
            QLineEdit *leCellResStd[MAX_ELECTRODE_NO];
            QLineEdit *leCellTimeStd[MAX_ELECTRODE_NO];
            QLineEdit *leCellCalibRes[MAX_ELECTRODE_NO];
            QLineEdit *leCellCalibTime[MAX_ELECTRODE_NO];

            // Data acquisition results group box
            QLineEdit *leAverSamplRate[MAX_ELECTRODE_NO];
            QLineEdit *leSTDSamplingRate[MAX_ELECTRODE_NO];
            QLineEdit *leStdPerMeanRate[MAX_ELECTRODE_NO];
            QLineEdit *leMinSRate[MAX_ELECTRODE_NO];
            QLineEdit *leDesPerMinRate[MAX_ELECTRODE_NO];
            QLineEdit *leMaxSTime[MAX_ELECTRODE_NO];

        //------------------------------------------------------//

        bool InitCalibrator();        
        void TimingTest();

    public:                
        Calibrator *calibrator;        
        ElectrodeCompDlg(QWidget *parent = 0);
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
