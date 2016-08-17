#include <fstream>
#include <QFile>
#include <QTextStream>

#include "ElectrodeCompDlg.h"
#include "ui_ElectrodeCompDlg.h"
#include "MainWin.h"
#include "Global.h"


ElectrodeCompDlg::ElectrodeCompDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    calibrator = new Calibrator();

    // Get the objects and buttons from each tab and connect the buttons with their slots
    for ( elecNum=0; elecNum<MAX_ELECTRODE_NO; elecNum++ ){

        // Info line
        lInfo[elecNum] = electrodeTabs->widget(elecNum)->findChild<QLabel *>(QString("lInfo_")+QString::number(elecNum+1));

        // Compensation toggler checkbox
        cbCompOn[elecNum] = electrodeTabs->widget(elecNum)->findChild<QCheckBox *>(QString("cbCompOn_")+QString::number(elecNum));

        // compensated voltage checkbox
        compVCopyOn[elecNum]= electrodeTabs->widget(elecNum)->findChild<QCheckBox *>(QString("CopyChannelCheckBox_")+QString::number(elecNum+1));
        compVCopyChannel[elecNum]= electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leGenCopyChannelNum_")+QString::number(elecNum+1));
        // General group box
        leGenSamplingRate[elecNum]  = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leGenSamplingRate_")+QString::number(elecNum+1));
        leGenInChannelNum[elecNum]  = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leGenInChannelNum_")+QString::number(elecNum+1));
        leGenOutChannelNum[elecNum] = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leGenOutChannelNum_")+QString::number(elecNum+1));

        // Electrode measurement group box
        leElecMaxCurrent[elecNum]   = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecMaxCurrent_")+QString::number(elecNum+1));
        leElecMinCurrent[elecNum]   = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecMinCurrent_")+QString::number(elecNum+1));
        leElecLevelNum[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecLevelNum_")+QString::number(elecNum+1));
        leElecInjLen[elecNum]       = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecInjLen_")+QString::number(elecNum+1));

        // Membrane measurement group box
        leMembIStep[elecNum]        = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leMembIStep_")+QString::number(elecNum+1));
        leMembRepNum[elecNum]       = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leMembRepNum_")+QString::number(elecNum+1));
        leMembInjLen[elecNum]       = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leMembInjLen_")+QString::number(elecNum+1));

        // Calibration group box
        leHyperpolCurr[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leHyperpolCurr_")+QString::number(elecNum+1));
        leCalInjAmp[elecNum]        = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCalInjAmp_")+QString::number(elecNum+1));
        leCalInjLen[elecNum]        = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCalInjLen_")+QString::number(elecNum+1));
        leCalFKLen[elecNum]         = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCalFKLen_")+QString::number(elecNum+1));
        leCalEKLen[elecNum]         = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCalEKLen_")+QString::number(elecNum+1));

        // Electrode results group box
        leElecMeasRes[elecNum]      = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecMeasRes_")+QString::number(elecNum+1));
        leElecMeasTime[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecMeasTime_")+QString::number(elecNum+1));
        leElecCalibRes[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecCalibRes_")+QString::number(elecNum+1));
        leElecResStd[elecNum]       = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecResStd_")+QString::number(elecNum+1));
        leElecTimeStd[elecNum]      = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecTimeStd_")+QString::number(elecNum+1));
        leElecCalibTime[elecNum]    = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leElecCalibTime_")+QString::number(elecNum+1));

        // Membrane results group box
        leCellMeasRes[elecNum]      = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellMeasRes_")+QString::number(elecNum+1));
        leCellMeasTime[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellMeasTime_")+QString::number(elecNum+1));
        leCellResStd[elecNum]       = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellResStd_")+QString::number(elecNum+1));
        leCellTimeStd[elecNum]      = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellTimeStd_")+QString::number(elecNum+1));
        leCellCalibRes[elecNum]     = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellCalibRes_")+QString::number(elecNum+1));
        leCellCalibTime[elecNum]    = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leCellCalibTime_")+QString::number(elecNum+1));

        // Data acquisition results group box
        leAverSamplRate[elecNum]    = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leAverSamplRate_")+QString::number(elecNum+1));
        leSTDSamplingRate[elecNum]  = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leSTDSamplingRate_")+QString::number(elecNum+1));
        leStdPerMeanRate[elecNum]   = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leStdPerMeanRate_")+QString::number(elecNum+1));
        leMinSRate[elecNum]         = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leMinSRate_")+QString::number(elecNum+1));
        leDesPerMinRate[elecNum]    = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leDesPerMinRate_")+QString::number(elecNum+1));
        leMaxSTime[elecNum]         = electrodeTabs->widget(elecNum)->findChild<QLineEdit *>(QString("leMaxSTime_")+QString::number(elecNum+1));

        elecMeasButton[elecNum] = electrodeTabs->findChild<QPushButton *>(QString("elecMeasButton_")+QString::number(elecNum+1));
        membMeasButton[elecNum] = electrodeTabs->findChild<QPushButton *>(QString("membMeasButton_")+QString::number(elecNum+1));
        calibrateButton[elecNum] = electrodeTabs->findChild<QPushButton *>(QString("calibrateButton_")+QString::number(elecNum+1));
        cbCompOn[elecNum] = electrodeTabs->findChild<QCheckBox *>(QString("cbCompOn_")+QString::number(elecNum+1));

        QObject::connect(elecMeasButton[elecNum], SIGNAL(released()), this, SLOT(MeasureElectrode()));
        QObject::connect(membMeasButton[elecNum], SIGNAL(released()), this, SLOT(MeasureMembrane()));
        QObject::connect(calibrateButton[elecNum], SIGNAL(released()), this, SLOT(CalibrateElectrode()));
        QObject::connect(cbCompOn[elecNum], SIGNAL(toggled(bool)), this, SLOT(ToggleCompensation()));
    }

}


ElectrodeCompDlg::~ElectrodeCompDlg()
{
}


// Turns on/off the compensation on the corresponding electrode
void ElectrodeCompDlg::ToggleCompensation()
{
    elecNum = electrodeTabs->currentIndex();  // the index of the electrode
    calibrator->SetChannelActivation(elecNum, cbCompOn[elecNum]->isChecked());
}


// Reads in the parameters of the calibration, etc., from the form
void ElectrodeCompDlg::exportData()
{
    for( int i=0; i<MAX_ELECTRODE_NO; i++ ) {

        // copy channel
        elecCalibPs[i].copyChnOn           = compVCopyOn[i]->isChecked();
        elecCalibPs[i].copyChn             = compVCopyChannel[i]->text().toInt();
        // General params
        elecCalibPs[i].samplingRate        = leGenSamplingRate[i]->text().toDouble() * 1e3; // kHz -> Hz
        elecCalibPs[i].inputChannelNumber  = leGenInChannelNum[i]->text().toInt();
        elecCalibPs[i].outputChannelNumber = leGenOutChannelNum[i]->text().toInt();

        // Calibration params
        elecCalibPs[i].hyperpolCurr       = leHyperpolCurr[i]->text().toDouble() * 1e-9; // nA -> A
        elecCalibPs[i].injCalAmp          = leCalInjAmp[i]->text().toDouble() * 1e-9;    // nA -> A
        elecCalibPs[i].injCalLen          = leCalInjLen[i]->text().toDouble() * 1e-3;    // msec -> sec
        elecCalibPs[i].fullKernelLen      = leCalFKLen[i]->text().toDouble() * 1e-3;     // msec -> sec
        elecCalibPs[i].electrodeKernelLen = leCalEKLen[i]->text().toDouble() * 1e-3;     // msec -> sec

        // Electrode measurement params
        elecCalibPs[i].iMaxElec       = leElecMaxCurrent[i]->text().toDouble() * 1e-9;  // nA -> A
        elecCalibPs[i].iMinElec       = leElecMinCurrent[i]->text().toDouble() * 1e-9;  // nA -> A
        elecCalibPs[i].numberOfLevels = leElecLevelNum[i]->text().toInt();
        elecCalibPs[i].injLenPerLevel = leElecInjLen[i]->text().toDouble() * 1e-3;      // msec -> sec

        // Membrane measurement params
        elecCalibPs[i].iMembStep       = leMembIStep[i]->text().toDouble() * 1e-9;   // nA -> A
        elecCalibPs[i].numberOfRepeats = leMembRepNum[i]->text().toInt();
        elecCalibPs[i].injLenPerRepeat = leMembInjLen[i]->text().toDouble() * 1e-3;  // msec -> sec
    }
}


// Writes out the parameters to the form
void ElectrodeCompDlg::importData()
{
  QString num;

  for( int i=0; i<MAX_ELECTRODE_NO; i++ ) {

    // copy channel
    compVCopyOn[i]->setChecked(elecCalibPs[i].copyChnOn);
    num.setNum(elecCalibPs[i].copyChn);
    compVCopyChannel[i]->setText(num);
    // General params
    num.setNum(elecCalibPs[i].samplingRate / 1e3);  // Hz -> kHz
    leGenSamplingRate[i]->setText(num);
    num.setNum(elecCalibPs[i].inputChannelNumber);
    leGenInChannelNum[i]->setText(num);
    num.setNum(elecCalibPs[i].outputChannelNumber);
    leGenOutChannelNum[i]->setText(num);

    // Calibration params
    num.setNum(elecCalibPs[i].hyperpolCurr * 1e9);  // A -> nA
    leHyperpolCurr[i]->setText(num);
    num.setNum(elecCalibPs[i].injCalAmp * 1e9);  // A -> nA
    leCalInjAmp[i]->setText(num);
    num.setNum(elecCalibPs[i].injCalLen * 1e3);  // sec -> msec
    leCalInjLen[i]->setText(num);
    num.setNum(elecCalibPs[i].fullKernelLen * 1e3);  // sec -> msec
    leCalFKLen[i]->setText(num);
    num.setNum(elecCalibPs[i].electrodeKernelLen * 1e3);  // sec -> msec
    leCalEKLen[i]->setText(num);

    // Electrode measurement params
    num.setNum(elecCalibPs[i].iMaxElec * 1e9);  // A -> nA
    leElecMaxCurrent[i]->setText(num);
    num.setNum(elecCalibPs[i].iMinElec * 1e9);  // A -> nA
    leElecMinCurrent[i]->setText(num);
    num.setNum(elecCalibPs[i].numberOfLevels);
    leElecLevelNum[i]->setText(num);
    num.setNum(elecCalibPs[i].injLenPerLevel * 1e3);  // sec -> msec
    leElecInjLen[i]->setText(num);

    // Membrane measurement params
    num.setNum(elecCalibPs[i].iMembStep * 1e9); // A -> nA
    leMembIStep[i]->setText(num);
    num.setNum(elecCalibPs[i].numberOfRepeats);
    leMembRepNum[i]->setText(num);
    num.setNum(elecCalibPs[i].injLenPerRepeat * 1e3);  // sec -> msec
    leMembInjLen[i]->setText(num);
  }

}


void ElectrodeCompDlg::MeasureElectrode()
{
    QString num;

    exportData();
    elecNum = electrodeTabs->currentIndex();

    // Check the task specific parameters provided by the user
    if ( elecCalibPs[elecNum].numberOfLevels < 2 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Number of current levels must be at least 2"));
         return;
    }
    if ( elecCalibPs[elecNum].injLenPerLevel <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Injection length must be positive"));
         return;
    }
    if ( elecCalibPs[elecNum].iMaxElec <= elecCalibPs[elecNum].iMinElec )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Maximal current level must be higher than minimal level"));
         return;
    }
    if ( elecCalibPs[elecNum].iMaxElec > outChnp[elecCalibPs[elecNum].outputChannelNumber].maxCurrent )
    {
         QString temp;
         temp.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].maxCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested maximal current level exceeds the max limit of the channel: ")+temp+QString("A"));
         return;
    }
    if ( elecCalibPs[elecNum].iMinElec < outChnp[elecCalibPs[elecNum].outputChannelNumber].minCurrent )
    {
         QString temp;
         temp.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].minCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested minimum current level exceeds the min limit of the channel: ")+temp+QString("A"));
         return;
    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo[elecNum]->text();
    lInfo[elecNum]->setText("Elec. meas. in progress ...");
    lInfo[elecNum]->repaint();


    // ------------------------------------------- //
    // Measure
    calibrator->ElectrodeMeasurement(elecCalibPs[elecNum].injLenPerLevel, elecCalibPs[elecNum].samplingRate, elecCalibPs[elecNum].numberOfLevels, elecCalibPs[elecNum].iMinElec, elecCalibPs[elecNum].iMaxElec);
    // ------------------------------------------- //


    if ( calibrator->incorrectMeasurement > 0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect measurements occurred! (OS interruption or low sampling frequency?)"));
    }
    if ( calibrator->elecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant measured. Try increasing the sampling frequency!"));
        lInfo[elecNum]->setText(prevText);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator->elecRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode resistance measured!"));
        lInfo[elecNum]->setText(prevText);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator->elecRes/1e6, 'f', 2);       // Ohm -> MOhm
    leElecMeasRes[elecNum]->setText(num);

    num.setNum(calibrator->elecResStd/1e6, 'f', 2);    // Ohm -> MOhm
    leElecResStd[elecNum]->setText(num);

    double length = calibrator->elecKernelTauERatio*calibrator->elecTau*1e3;   // sec -> msec
    if( length < 1.0 ) length = 1.0;        // minimal limit
    else if( length > 5.0 ) length = 5.0;   // maximal limit
    if( elecCalibPs[elecNum].fullKernelLen * 1e3 < length ) length = elecCalibPs[elecNum].fullKernelLen * 1e3 / calibrator->maxElecKFullKRation; // hopefully not the case :)

    num.setNum(length, 'f', 1);
    leCalEKLen[elecNum]->setText(num);

    num.setNum(calibrator->elecTau*1e3, 'f', 2);     // sec -> msec
    leElecMeasTime[elecNum]->setText(num);

    num.setNum(calibrator->elecTauStd*1e3, 'f', 2);  // sec -> msec
    leElecTimeStd[elecNum]->setText(num);

    TimingTest();

    lInfo[elecNum]->setText("Electrode has been measured");
    lInfo[elecNum]->repaint();
    this->setEnabled(true);

    message(QString("Electrode characteristic measurement on input channel ")+QString::number(elecCalibPs[elecNum].inputChannelNumber)+
            QString(" and output channel ")+QString::number(elecCalibPs[elecNum].outputChannelNumber)+QString(" finished."));

}


void ElectrodeCompDlg::MeasureMembrane()
{
    QString num;

    exportData();
    elecNum = electrodeTabs->currentIndex();

    // Check the task specific parameters provided by the user
    if ( elecCalibPs[elecNum].numberOfRepeats < 1 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Number of repeats must be at least 1"));
         return;
    }
    if ( elecCalibPs[elecNum].injLenPerRepeat <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Injection length must be positive"));
         return;
    }
    if ( elecCalibPs[elecNum].iMembStep > outChnp[elecCalibPs[elecNum].inputChannelNumber].maxCurrent )
    {
         QString temp;
         temp.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].maxCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested maximal current step exceeds the max limit of the channel: ")+temp+QString("A"));
         return;
    }
    if ( elecCalibPs[elecNum].iMembStep < outChnp[elecCalibPs[elecNum].inputChannelNumber].minCurrent )
    {
         QString temp;
         temp.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].minCurrent);
         QMessageBox::warning(this, tr("Warning"), QString("Requested minimum current step exceeds the min limit of the channel: ")+temp+QString("A"));
         return;

    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo[elecNum]->text();
    lInfo[elecNum]->setText("Memb. meas. in progress ...");
    lInfo[elecNum]->repaint();


    // ------------------------------------------- //
    // Measure
    calibrator->MembraneMeasurement(elecCalibPs[elecNum].injLenPerRepeat, elecCalibPs[elecNum].samplingRate, elecCalibPs[elecNum].numberOfRepeats, elecCalibPs[elecNum].iMembStep);
    // ------------------------------------------- //


    if ( calibrator->incorrectMeasurement > 0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect measurements occurred possibly due to OS interruption"));
    }
    if ( calibrator->membRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane resistance measured!"));
        lInfo[elecNum]->setText(prevText);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator->membTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane time constant measured!"));
        lInfo[elecNum]->setText(prevText);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator->membRes/1e6, 'f', 2);       // Ohm -> MOhm
    leCellMeasRes[elecNum]->setText(num);

    double length = calibrator->fullKernelTauMRatio*calibrator->membTau*1e3;   // sec -> msec
    if( length < 10.0 ) length = 10.0;        // minimal limit
    else if( length > 50.0 ) length = 50.0;   // maximal limit
    if( elecCalibPs[elecNum].electrodeKernelLen * 1e3 > length ) length = elecCalibPs[elecNum].electrodeKernelLen * 1e3 * calibrator->maxElecKFullKRation; // hopefully not the case :)

    num.setNum(length, 'f', 0);
    leCalFKLen[elecNum]->setText(num);

    num.setNum(calibrator->membResStd/1e6, 'f', 2);    // Ohm -> MOhm
    leCellResStd[elecNum]->setText(num);

    num.setNum(calibrator->membTau*1e3, 'f', 2);       // sec -> msec
    leCellMeasTime[elecNum]->setText(num);

    num.setNum(calibrator->membTauStd*1e3, 'f', 2);    // sec -> msec
    leCellTimeStd[elecNum]->setText(num);

    TimingTest();

    lInfo[elecNum]->setText("Membrane has been measured");
    lInfo[elecNum]->repaint();
    this->setEnabled(true);

    message(QString("Membrane characteristic measurement on input channel ")+QString::number(elecCalibPs[elecNum].inputChannelNumber)+
            QString(" and output channel ")+QString::number(elecCalibPs[elecNum].outputChannelNumber)+QString(" finished."));

}


void ElectrodeCompDlg::CalibrateElectrode()
{
    QString num;

    exportData();
    elecNum = electrodeTabs->currentIndex();

    // Check the task specific parameters provided by the user
    if ( elecCalibPs[elecNum].injCalAmp+elecCalibPs[elecNum].hyperpolCurr > outChnp[elecCalibPs[elecNum].inputChannelNumber].maxCurrent )
    {
         QString temp1;
         QString temp2;
         temp1.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].maxCurrent);
         temp2.setNum(elecCalibPs[elecNum].injCalAmp+elecCalibPs[elecNum].hyperpolCurr);
         QMessageBox::warning(this, tr("Warning"), QString("Requested calibration current (amplitude + hyperolarizing level): ")+temp2+QString("A, exceeds the max limit of the channel: ")+temp1+QString("A"));
         return;
    }
    if ( elecCalibPs[elecNum].injCalAmp+elecCalibPs[elecNum].hyperpolCurr < outChnp[elecCalibPs[elecNum].inputChannelNumber].minCurrent )
    {
         QString temp1;
         QString temp2;
         temp1.setNum(outChnp[elecCalibPs[elecNum].outputChannelNumber].minCurrent);
         temp2.setNum(elecCalibPs[elecNum].injCalAmp+elecCalibPs[elecNum].hyperpolCurr);
         QMessageBox::warning(this, tr("Warning"), QString("Requested calibration current (amplitude + hyperolarizing level): ")+temp2+QString("A, exceeds the min limit of the channel: ")+temp1+QString("A"));
         return;
    }
    if ( elecCalibPs[elecNum].fullKernelLen >= elecCalibPs[elecNum].injCalLen )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Full kernel must not be longer than the injection"));
         return;
    }
    if ( elecCalibPs[elecNum].fullKernelLen <= elecCalibPs[elecNum].electrodeKernelLen )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Full kernel must higher than electrode kernel"));
         return;
    }
    if ( elecCalibPs[elecNum].electrodeKernelLen <= 0 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Electrode kernel must have a positive length"));
         return;
    }


    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo[elecNum]->text();
    lInfo[elecNum]->setText("Calibration in progress ...");
    lInfo[elecNum]->repaint();


    // ------------------------------------------- //
    // Calibrate
    calibrator->Calibration(elecCalibPs[elecNum].injCalLen, elecCalibPs[elecNum].samplingRate, elecNum, (int) (elecCalibPs[elecNum].fullKernelLen*elecCalibPs[elecNum].samplingRate), (int) (elecCalibPs[elecNum].electrodeKernelLen*elecCalibPs[elecNum].samplingRate), elecCalibPs[elecNum].injCalAmp, elecCalibPs[elecNum].hyperpolCurr);
    // ------------------------------------------- //


    if ( calibrator->calMembTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane time constant calculated!"));
    }

    if ( calibrator->calMembRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane resistance calculated!"));
    }

    if ( calibrator->calElecRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode resistance calculated!\n AEC channel is inactivated."));
        lInfo[elecNum]->setText(prevText);
        cbCompOn[elecNum]->setChecked(false);
        cbCompOn[elecNum]->setEnabled(false);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);        
        calibrator->DCT->aecChannels[elecNum]->Inactivate();
        return;
    }

    if ( calibrator->calElecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant calculated!\n AEC channel is inactivated."));
        lInfo[elecNum]->setText(prevText);
        cbCompOn[elecNum]->setEnabled(false);
        lInfo[elecNum]->repaint();
        this->setEnabled(true);        
        calibrator->DCT->aecChannels[elecNum]->Inactivate();
        return;
    }

    num.setNum(calibrator->calMembRes/1e6, 'f', 2); // Ohm -> MOhm
    leCellCalibRes[elecNum]->setText(num);

    num.setNum(calibrator->calElecRes/1e6, 'f', 2); // Ohm -> MOhm
    leElecCalibRes[elecNum]->setText(num);

    num.setNum(calibrator->calMembTau*1e3, 'f', 2); // sec -> msec
    leCellCalibTime[elecNum]->setText(num);

    num.setNum(calibrator->calElecTau*1e3, 'f', 2); // sec -> msec
    leElecCalibTime[elecNum]->setText(num);

    TimingTest();

    if ( calibrator->stdPerAverRate > 0.05 ) QMessageBox::warning(this, tr("Warning"), tr("High std/mean rate in sampling times.\nToo many program running in the background?\nYou might want to consider recalibration."));
    if ( calibrator->desPerMinRate > 50 )    QMessageBox::warning(this, tr("Warning"), tr("High desired/min rate in sampling times.\nToo many program running in the background?\nYou might want to consider recalibration."));

    lInfo[elecNum]->setText("Electrode has been calibrated");
    lInfo[elecNum]->repaint();
    cbCompOn[elecNum]->setEnabled(true);
    cbCompOn[elecNum]->setCheckable(true);
    cbCompOn[elecNum]->setChecked(true);
    this->setEnabled(true);

    message(QString("Calibration on input channel ")+QString::number(elecCalibPs[elecNum].inputChannelNumber)+
            QString(" and output channel ")+QString::number(elecCalibPs[elecNum].outputChannelNumber)+QString(" finished."));
}


// Initializes the Calibrator object with the parameters read from the dialog window
// and the input/output channels as well
// warns if the DAQ has not yet been initialized or the input or output channel
// are invalid (not active) and also returns false in these cases
bool ElectrodeCompDlg::InitCalibrator()
{
    DAQ *board = ((MyMainWindow*) this->parent())->board;

    if ( board->initialized == false )
    {
        QMessageBox::warning(this, tr("Warning"), tr("DAQ not initialized"));
        return false;
    }

    DCThread *DCT = (((MyMainWindow*) this->parent())->DCT);

    // General init
    calibrator->GeneralInit(board, DCT);

    this->setEnabled(false);

    // Channel init
    switch ( calibrator->ChannelInit( (short int) elecCalibPs[elecNum].inputChannelNumber, (short int) elecCalibPs[elecNum].outputChannelNumber) )
    {
            case 0: break;
            case 1: QMessageBox::warning(this, tr("Warning"), tr("Selected input channel is not an exsisting active channel"));
                    this->setEnabled(true);
                    return false;
            case 2: QMessageBox::warning(this, tr("Warning"), tr("Selected output channel is not an exsisting active channel"));
                    this->setEnabled(true);
                    return false;
    }

    return true;
}


// Displays some statistics about the data acquisition
void ElectrodeCompDlg::TimingTest()
{
    QString num;

    calibrator->CalcSamplingStats();

    // Display them
    num.setNum((int) (calibrator->averSampRate));
    leAverSamplRate[elecNum]->setText(num);

    num.setNum((int) (calibrator->stdSampRate));
    leSTDSamplingRate[elecNum]->setText(num);

    num.setNum(calibrator->stdPerAverRate, 'f', 3);
    leStdPerMeanRate[elecNum]->setText(num);

    num.setNum((int) (calibrator->minSampRate));
    leMinSRate[elecNum]->setText(num);

    num.setNum(calibrator->desPerMinRate, 'f', 2);
    leDesPerMinRate[elecNum]->setText(num);

    num.setNum(calibrator->maxSampTime*1e3, 'f', 2);  // sec -> msec
    leMaxSTime[elecNum]->setText(num);
}
