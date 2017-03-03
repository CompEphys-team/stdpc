#include <fstream>
#include <QFile>
#include <QTextStream>

#include "ElectrodeCompDlg.h"
#include "ui_ElectrodeCompDlg.h"
#include "MainWin.h"
#include "Global.h"
#include "ChannelIndex.h"


ElectrodeCompDlg::~ElectrodeCompDlg()
{
    delete ui;
}

ElectrodeCompDlg::ElectrodeCompDlg(elecCalibParams &p, ChannelIndex dex, QWidget *parent) :
    QDialog(parent),
    p(p),
    dex(dex),
    outChnModel(ChannelListModel::AnalogOut, this),
    ui(new Ui::ElectrodeCompDlg)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle().arg(dex.prettyName()));

    outChnModel.updateChns();
    ui->leGenOutChannelNum_1->setModel(&outChnModel);
    ui->leGenCopyChannelNum_1->setModel(&outChnModel);

    if ( p.samplingRate == 0 ) // Use unset samplingRate as proxy for uninitialised p => export defaults
        exportData();
    else
        importData();
    ui->label_inChn->setText(dex.prettyName());

    connect(ui->elecMeasButton_1, SIGNAL(released()), this, SLOT(MeasureElectrode()));
    connect(ui->membMeasButton_1, SIGNAL(released()), this, SLOT(MeasureMembrane()));
    connect(ui->calibrateButton_1, SIGNAL(released()), this, SLOT(CalibrateElectrode()));
    connect(ui->cbCompOn_1, SIGNAL(toggled(bool)), this, SLOT(ToggleCompensation()));
}


// Turns on/off the compensation on the corresponding electrode
void ElectrodeCompDlg::ToggleCompensation()
{
    calibrator.SetChannelActivation(ui->cbCompOn_1->isChecked());
}


// Reads in the parameters of the calibration, etc., from the form
void ElectrodeCompDlg::exportData()
{
    // copy channel
    p.copyChnOn           = ui->CopyChannelCheckBox_1->isChecked();
    p.copyChn             = ui->leGenCopyChannelNum_1->currentData().value<ChannelIndex>();
    // General params
    p.samplingRate        = ui->leGenSamplingRate_1->text().toDouble() * 1e3; // kHz -> Hz
    p.outputChannelNumber = ui->leGenOutChannelNum_1->currentData().value<ChannelIndex>();

    // Calibration params
    p.hyperpolCurr       = ui->leHyperpolCurr_1->text().toDouble() * 1e-9; // nA -> A
    p.injCalAmp          = ui->leCalInjAmp_1->text().toDouble() * 1e-9;    // nA -> A
    p.injCalLen          = ui->leCalInjLen_1->text().toDouble() * 1e-3;    // msec -> sec
    p.fullKernelLen      = ui->leCalFKLen_1->text().toDouble() * 1e-3;     // msec -> sec
    p.electrodeKernelLen = ui->leCalEKLen_1->text().toDouble() * 1e-3;     // msec -> sec

    // Electrode measurement params
    p.iMaxElec       = ui->leElecMaxCurrent_1->text().toDouble() * 1e-9;  // nA -> A
    p.iMinElec       = ui->leElecMinCurrent_1->text().toDouble() * 1e-9;  // nA -> A
    p.numberOfLevels = ui->leElecLevelNum_1->text().toInt();
    p.injLenPerLevel = ui->leElecInjLen_1->text().toDouble() * 1e-3;      // msec -> sec

    // Membrane measurement params
    p.iMembStep       = ui->leMembIStep_1->text().toDouble() * 1e-9;   // nA -> A
    p.numberOfRepeats = ui->leMembRepNum_1->text().toInt();
    p.injLenPerRepeat = ui->leMembInjLen_1->text().toDouble() * 1e-3;  // msec -> sec
}


// Writes out the parameters to the form
void ElectrodeCompDlg::importData()
{
    QString num;

    // copy channel
    ui->CopyChannelCheckBox_1->setChecked(p.copyChnOn);
    ui->leGenCopyChannelNum_1->setCurrentIndex(outChnModel.index(p.copyChn));

    // General params
    num.setNum(p.samplingRate / 1e3);  // Hz -> kHz
    ui->leGenSamplingRate_1->setText(num);
    ui->leGenOutChannelNum_1->setCurrentIndex(outChnModel.index(p.outputChannelNumber));

    // Calibration params
    num.setNum(p.hyperpolCurr * 1e9);  // A -> nA
    ui->leHyperpolCurr_1->setText(num);
    num.setNum(p.injCalAmp * 1e9);  // A -> nA
    ui->leCalInjAmp_1->setText(num);
    num.setNum(p.injCalLen * 1e3);  // sec -> msec
    ui->leCalInjLen_1->setText(num);
    num.setNum(p.fullKernelLen * 1e3);  // sec -> msec
    ui->leCalFKLen_1->setText(num);
    num.setNum(p.electrodeKernelLen * 1e3);  // sec -> msec
    ui->leCalEKLen_1->setText(num);

    // Electrode measurement params
    num.setNum(p.iMaxElec * 1e9);  // A -> nA
    ui->leElecMaxCurrent_1->setText(num);
    num.setNum(p.iMinElec * 1e9);  // A -> nA
    ui->leElecMinCurrent_1->setText(num);
    num.setNum(p.numberOfLevels);
    ui->leElecLevelNum_1->setText(num);
    num.setNum(p.injLenPerLevel * 1e3);  // sec -> msec
    ui->leElecInjLen_1->setText(num);

    // Membrane measurement params
    num.setNum(p.iMembStep * 1e9); // A -> nA
    ui->leMembIStep_1->setText(num);
    num.setNum(p.numberOfRepeats);
    ui->leMembRepNum_1->setText(num);
    num.setNum(p.injLenPerRepeat * 1e3);  // sec -> msec
    ui->leMembInjLen_1->setText(num);

}


void ElectrodeCompDlg::MeasureElectrode()
{
    QString num;

    exportData();

    // Check the task specific parameters provided by the user
    if ( p.numberOfLevels < 2 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Number of current levels must be at least 2"));
         return;
    }
    if ( p.injLenPerLevel <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Injection length must be positive"));
         return;
    }
    if ( p.iMaxElec <= p.iMinElec )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Maximal current level must be higher than minimal level"));
         return;
    }
    if ( p.iMaxElec > p.outputChannelNumber.getOutChnData()->maxCurrent )
    {
         QString temp;
         temp.setNum(p.outputChannelNumber.getOutChnData()->maxCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested maximal current level exceeds the max limit of the channel: ")+temp+QString("A"));
         return;
    }
    if ( p.iMinElec < p.outputChannelNumber.getOutChnData()->minCurrent )
    {
         QString temp;
         temp.setNum(p.outputChannelNumber.getOutChnData()->minCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested minimum current level exceeds the min limit of the channel: ")+temp+QString("A"));
         return;
    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = ui->lInfo_1->text();
    ui->lInfo_1->setText("Elec. meas. in progress ...");
    ui->lInfo_1->repaint();


    // ------------------------------------------- //
    // Measure
    calibrator.ElectrodeMeasurement(p.injLenPerLevel, p.samplingRate, p.numberOfLevels, p.iMinElec, p.iMaxElec);
    // ------------------------------------------- //


    if ( calibrator.incorrectMeasurement > 0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect measurements occurred! (OS interruption or low sampling frequency?)"));
    }
    if ( calibrator.elecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant measured. Try increasing the sampling frequency!"));
        ui->lInfo_1->setText(prevText);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator.elecRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode resistance measured!"));
        ui->lInfo_1->setText(prevText);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator.elecRes/1e6, 'f', 2);       // Ohm -> MOhm
    ui->leElecMeasRes_1->setText(num);

    num.setNum(calibrator.elecResStd/1e6, 'f', 2);    // Ohm -> MOhm
    ui->leElecResStd_1->setText(num);

    double length = calibrator.elecKernelTauERatio*calibrator.elecTau*1e3;   // sec -> msec
    if( length < 1.0 ) length = 1.0;        // minimal limit
    else if( length > 5.0 ) length = 5.0;   // maximal limit
    if( p.fullKernelLen * 1e3 < length ) length = p.fullKernelLen * 1e3 / calibrator.maxElecKFullKRation; // hopefully not the case :)

    num.setNum(length, 'f', 1);
    ui->leCalEKLen_1->setText(num);

    num.setNum(calibrator.elecTau*1e3, 'f', 2);     // sec -> msec
    ui->leElecMeasTime_1->setText(num);

    num.setNum(calibrator.elecTauStd*1e3, 'f', 2);  // sec -> msec
    ui->leElecTimeStd_1->setText(num);

    TimingTest();

    ui->lInfo_1->setText("Electrode has been measured");
    ui->lInfo_1->repaint();
    this->setEnabled(true);

    message(QString("Electrode characteristic measurement on input channel %1 and output channel %2 finished.")
            .arg(dex.prettyName(), p.outputChannelNumber.prettyName()));

}


void ElectrodeCompDlg::MeasureMembrane()
{
    QString num;

    exportData();

    // Check the task specific parameters provided by the user
    if ( p.numberOfRepeats < 1 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Number of repeats must be at least 1"));
         return;
    }
    if ( p.injLenPerRepeat <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Injection length must be positive"));
         return;
    }
    if ( p.iMembStep > p.outputChannelNumber.getOutChnData()->maxCurrent )
    {
         QString temp;
         temp.setNum(p.outputChannelNumber.getOutChnData()->maxCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested maximal current step exceeds the max limit of the channel: ")+temp+QString("A"));
         return;
    }
    if ( p.iMembStep < p.outputChannelNumber.getOutChnData()->minCurrent )
    {
         QString temp;
         temp.setNum(p.outputChannelNumber.getOutChnData()->minCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested minimum current step exceeds the min limit of the channel: ")+temp+QString("A"));
         return;

    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = ui->lInfo_1->text();
    ui->lInfo_1->setText("Memb. meas. in progress ...");
    ui->lInfo_1->repaint();


    // ------------------------------------------- //
    // Measure
    calibrator.MembraneMeasurement(p.injLenPerRepeat, p.samplingRate, p.numberOfRepeats, p.iMembStep);
    // ------------------------------------------- //


    if ( calibrator.incorrectMeasurement > 0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect measurements occurred possibly due to OS interruption"));
    }
    if ( calibrator.membRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane resistance measured!"));
        ui->lInfo_1->setText(prevText);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator.membTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane time constant measured!"));
        ui->lInfo_1->setText(prevText);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator.membRes/1e6, 'f', 2);       // Ohm -> MOhm
    ui->leCellMeasRes_1->setText(num);

    double length = calibrator.fullKernelTauMRatio*calibrator.membTau*1e3;   // sec -> msec
    if( length < 10.0 ) length = 10.0;        // minimal limit
    else if( length > 50.0 ) length = 50.0;   // maximal limit
    if( p.electrodeKernelLen * 1e3 > length ) length = p.electrodeKernelLen * 1e3 * calibrator.maxElecKFullKRation; // hopefully not the case :)

    num.setNum(length, 'f', 0);
    ui->leCalFKLen_1->setText(num);

    num.setNum(calibrator.membResStd/1e6, 'f', 2);    // Ohm -> MOhm
    ui->leCellResStd_1->setText(num);

    num.setNum(calibrator.membTau*1e3, 'f', 2);       // sec -> msec
    ui->leCellMeasTime_1->setText(num);

    num.setNum(calibrator.membTauStd*1e3, 'f', 2);    // sec -> msec
    ui->leCellTimeStd_1->setText(num);

    TimingTest();

    ui->lInfo_1->setText("Membrane has been measured");
    ui->lInfo_1->repaint();
    this->setEnabled(true);

    message(QString("Membrane characteristic measurement on input channel %1 and output channel %2 finished.")
            .arg(dex.prettyName(), p.outputChannelNumber.prettyName()));

}


void ElectrodeCompDlg::CalibrateElectrode()
{
    QString num;

    exportData();

    // Check the task specific parameters provided by the user
    if ( p.injCalAmp+p.hyperpolCurr > p.outputChannelNumber.getOutChnData()->maxCurrent )
    {
         QString temp1;
         QString temp2;
         temp1.setNum(p.outputChannelNumber.getOutChnData()->maxCurrent);
         temp2.setNum(p.injCalAmp+p.hyperpolCurr);
         QMessageBox::warning(this, tr("Warning"), QString("Requested calibration current (amplitude + hyperolarizing level): ")+temp2+QString("A, exceeds the max limit of the channel: ")+temp1+QString("A"));
         return;
    }
    if ( p.injCalAmp+p.hyperpolCurr < p.outputChannelNumber.getOutChnData()->minCurrent )
    {
         QString temp1;
         QString temp2;
         temp1.setNum(p.outputChannelNumber.getOutChnData()->minCurrent);
         temp2.setNum(p.injCalAmp+p.hyperpolCurr);
         QMessageBox::warning(this, tr("Warning"), QString("Requested calibration current (amplitude + hyperolarizing level): ")+temp2+QString("A, exceeds the min limit of the channel: ")+temp1+QString("A"));
         return;
    }
    if ( p.fullKernelLen >= p.injCalLen )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Full kernel must not be longer than the injection"));
         return;
    }
    if ( p.fullKernelLen <= p.electrodeKernelLen )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Full kernel must higher than electrode kernel"));
         return;
    }
    if ( p.electrodeKernelLen <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Electrode kernel must have a positive length"));
         return;
    }


    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = ui->lInfo_1->text();
    ui->lInfo_1->setText("Calibration in progress ...");
    ui->lInfo_1->repaint();


    // ------------------------------------------- //
    // Calibrate
    calibrator.Calibration(p.injCalLen, p.samplingRate, (int) (p.fullKernelLen*p.samplingRate), (int) (p.electrodeKernelLen*p.samplingRate), p.injCalAmp, p.hyperpolCurr);
    // ------------------------------------------- //


    if ( calibrator.calMembTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane time constant calculated!"));
    }

    if ( calibrator.calMembRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane resistance calculated!"));
    }

    if ( calibrator.calElecRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode resistance calculated!\n AEC channel is inactivated."));
        ui->lInfo_1->setText(prevText);
        ui->cbCompOn_1->setChecked(false);
        ui->cbCompOn_1->setEnabled(false);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        Devices.getInChan(dex)->aec.Inactivate();
        return;
    }

    if ( calibrator.calElecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant calculated!\n AEC channel is inactivated."));
        ui->lInfo_1->setText(prevText);
        ui->cbCompOn_1->setEnabled(false);
        ui->lInfo_1->repaint();
        this->setEnabled(true);
        Devices.getInChan(dex)->aec.Inactivate();
        return;
    }

    num.setNum(calibrator.calMembRes/1e6, 'f', 2); // Ohm -> MOhm
    ui->leCellCalibRes_1->setText(num);

    num.setNum(calibrator.calElecRes/1e6, 'f', 2); // Ohm -> MOhm
    ui->leElecCalibRes_1->setText(num);

    num.setNum(calibrator.calMembTau*1e3, 'f', 2); // sec -> msec
    ui->leCellCalibTime_1->setText(num);

    num.setNum(calibrator.calElecTau*1e3, 'f', 2); // sec -> msec
    ui->leElecCalibTime_1->setText(num);

    TimingTest();

    if ( calibrator.stdPerAverRate > 0.05 ) QMessageBox::warning(this, tr("Warning"), tr("High std/mean rate in sampling times.\nToo many program running in the background?\nYou might want to consider recalibration."));
    if ( calibrator.desPerMinRate > 50 )    QMessageBox::warning(this, tr("Warning"), tr("High desired/min rate in sampling times.\nToo many program running in the background?\nYou might want to consider recalibration."));

    ui->lInfo_1->setText("Electrode has been calibrated");
    ui->lInfo_1->repaint();
    ui->cbCompOn_1->setEnabled(true);
    ui->cbCompOn_1->setChecked(true);
    this->setEnabled(true);

    message(QString("Calibration on input channel %1 and output channel %2 finished.")
            .arg(dex.prettyName(), p.outputChannelNumber.prettyName()));
}


// Initializes the Calibrator object with the parameters read from the dialog window
// and the input/output channels as well
// warns if the input or output channel
// are invalid (not active) and also returns false in these cases
bool ElectrodeCompDlg::InitCalibrator()
{
    // Channel init
    switch ( calibrator.ChannelInit(dex, p.outputChannelNumber) )
    {
    case 0: break;
    case 1: QMessageBox::warning(this, tr("Warning"), tr("Selected input channel is not an existing active channel"));
            this->setEnabled(true);
            return false;
    case 2: QMessageBox::warning(this, tr("Warning"), tr("Selected output channel is not an existing active channel"));
            this->setEnabled(true);
            return false;
    case 3: QMessageBox::warning(this, tr("Warning"), tr("Selected input device is not active"));
            this->setEnabled(true);
            return false;
    case 4: QMessageBox::warning(this, tr("Warning"), tr("Selected output device is not active"));
            this->setEnabled(true);
            return false;
    }

    return true;
}


// Displays some statistics about the data acquisition
void ElectrodeCompDlg::TimingTest()
{
    QString num;

    calibrator.CalcSamplingStats();

    // Display them
    num.setNum((int) (calibrator.averSampRate));
    ui->leAverSamplRate_1->setText(num);

    num.setNum((int) (calibrator.stdSampRate));
    ui->leSTDSamplingRate_1->setText(num);

    num.setNum(calibrator.stdPerAverRate, 'f', 3);
    ui->leStdPerMeanRate_1->setText(num);

    num.setNum((int) (calibrator.minSampRate));
    ui->leMinSRate_1->setText(num);

    num.setNum(calibrator.desPerMinRate, 'f', 2);
    ui->leDesPerMinRate_1->setText(num);

    num.setNum(calibrator.maxSampTime*1e3, 'f', 2);  // sec -> msec
    ui->leMaxSTime_1->setText(num);
}
