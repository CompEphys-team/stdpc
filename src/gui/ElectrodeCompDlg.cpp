#include <fstream>
#include <QFile>
#include <QTextStream>

#include "ElectrodeCompDlg.h"
#include "ui_ElectrodeCompDlg.h"
#include "MainWin.h"


ElectrodeCompDlg::ElectrodeCompDlg(QWidget *parent) :
    QDialog(parent)   
{   
    setupUi(this);

    calibrator = new Calibrator();

    numOfElectrodes = 4;

    // Get the buttons from each tab and connects them with their slots
    for ( tabNum=1; tabNum<=numOfElectrodes; tabNum++ ){
        QPushButton *elecMeasButton = electrodeTabs->findChild<QPushButton *>(QString("elecMeasButton_")+QString::number(tabNum));
        QPushButton *membMeasButton = electrodeTabs->findChild<QPushButton *>(QString("membMeasButton_")+QString::number(tabNum));
        QPushButton *calibrateButton = electrodeTabs->findChild<QPushButton *>(QString("calibrateButton_")+QString::number(tabNum));
        QCheckBox *cbCompOn = electrodeTabs->findChild<QCheckBox *>(QString("cbCompOn_")+QString::number(tabNum));

        QObject::connect(elecMeasButton, SIGNAL(released()), this, SLOT(MeasureElectrode()));
        QObject::connect(membMeasButton, SIGNAL(released()), this, SLOT(MeasureMembrane()));
        QObject::connect(calibrateButton, SIGNAL(released()), this, SLOT(CalibrateElectrode()));        
        QObject::connect(cbCompOn, SIGNAL(toggled(bool)), this, SLOT(ToggleCompensation()));
    }
}


ElectrodeCompDlg::~ElectrodeCompDlg()
{
}


// Gets the gui objects from the current electrode tab
void ElectrodeCompDlg::GetGuiElements()
{
    // The current tab
    tabNum = electrodeTabs->currentIndex()+1;

    // Info line
    lInfo = electrodeTabs->currentWidget()->findChild<QLabel *>(QString("lInfo_")+QString::number(tabNum));

    // Compensation toggler checkbox
    cbCompOn = electrodeTabs->currentWidget()->findChild<QCheckBox *>(QString("cbCompOn_")+QString::number(tabNum));

    // General group box
    leGenInjLen        = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leGenInjLen_")+QString::number(tabNum));
    leGenSamplingRate  = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leGenSamplingRate_")+QString::number(tabNum));
    leGenInChannelNum  = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leGenInChannelNum_")+QString::number(tabNum));
    leGenOutChannelNum = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leGenOutChannelNum_")+QString::number(tabNum));

    // Electrode measurement group box
    leElecMaxCurrent   = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecMaxCurrent_")+QString::number(tabNum));
    leElecMinCurrent   = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecMinCurrent_")+QString::number(tabNum));
    leElecLevelNum     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecLevelNum_")+QString::number(tabNum));

    // Membrane measurement group box
    leMembIStep        = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leMembIStep_")+QString::number(tabNum));
    leMembRepNum       = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leMembRepNum_")+QString::number(tabNum));

    // Calibration group box
    leHyperpolCurr     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leHyperpolCurr_")+QString::number(tabNum));
    leCalInjAmp        = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCalInjAmp_")+QString::number(tabNum));
    leCalFKLen         = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCalFKLen_")+QString::number(tabNum));
    leCalEKLen         = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCalEKLen_")+QString::number(tabNum));

    // Electrode results group box
    leElecMeasRes      = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecMeasRes_")+QString::number(tabNum));
    leElecMeasTime     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecMeasTime_")+QString::number(tabNum));
    leElecCalibRes     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecCalibRes_")+QString::number(tabNum));
    leElecResStd       = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecResStd_")+QString::number(tabNum));
    leElecTimeStd      = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecTimeStd_")+QString::number(tabNum));
    leElecCalibTime    = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leElecCalibTime_")+QString::number(tabNum));

    // Membrane results group box
    leCellMeasRes      = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellMeasRes_")+QString::number(tabNum));
    leCellMeasTime     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellMeasTime_")+QString::number(tabNum));
    leCellResStd       = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellResStd_")+QString::number(tabNum));
    leCellTimeStd      = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellTimeStd_")+QString::number(tabNum));
    leCellCalibRes     = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellCalibRes_")+QString::number(tabNum));
    leCellCalibTime    = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leCellCalibTime_")+QString::number(tabNum));

    // Data acquisition results group box
    leAverSamplRate    = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leAverSamplRate_")+QString::number(tabNum));
    leSTDSamplingRate  = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leSTDSamplingRate_")+QString::number(tabNum));
    leMinSRate         = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leMinSRate_")+QString::number(tabNum));
    leDesPerMinRate    = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leDesPerMinRate_")+QString::number(tabNum));
    leMaxSTime         = electrodeTabs->currentWidget()->findChild<QLineEdit *>(QString("leMaxSTime_")+QString::number(tabNum));

}

// Turns on/off the compensation on the corresponding electrode
void ElectrodeCompDlg::ToggleCompensation()
{    
    int electrodeNum = electrodeTabs->currentIndex();  // the index of the electrode
    // Compensation toggler checkbox
    cbCompOn = electrodeTabs->currentWidget()->findChild<QCheckBox *>(QString("cbCompOn_")+QString::number(electrodeNum+1));

    calibrator->SetChannelActivation(electrodeNum, cbCompOn->isChecked());
}


// Reads in the parameters of the calibration, etc., from the form
void ElectrodeCompDlg::ExportParams()
{
  GetGuiElements();

  // General params
  params.injLen              = leGenInjLen->text().toDouble() * 1e-3;  // msec -> sec
  params.samplingRate        = leGenSamplingRate->text().toDouble();   // Hz = 1/sec
  params.inputChannelNumber  = leGenInChannelNum->text().toInt();
  params.outputChannelNumber = leGenOutChannelNum->text().toInt();

  // Calibration params
  params.hyperpolCurr       = leHyperpolCurr->text().toDouble() * 1e-12;// pA -> A
  params.injCalAmp          = leCalInjAmp->text().toDouble() * 1e-12;   // pA -> A
  params.fullKernelLen      = leCalFKLen->text().toDouble() * 1e-3;     // msec -> sec
  params.electrodeKernelLen = leCalEKLen->text().toDouble() * 1e-3;     // msec -> sec

  // Electrode measurement params
  params.iMaxElec       = leElecMaxCurrent->text().toDouble() * 1e-12; // pA -> A
  params.iMinElec       = leElecMinCurrent->text().toDouble() * 1e-12; // pA -> A
  params.numberOfLevels = leElecLevelNum->text().toInt();

  // Membrane measurement params
  params.iMembStep       = leMembIStep->text().toDouble() * 1e-12; // pA -> A
  params.numberOfRepeats = leMembRepNum->text().toInt();

}


// Writes out the parameters to the form
void ElectrodeCompDlg::ImportParams()
{
  QString num;

  GetGuiElements();

  // General params
  num.setNum(params.injLen * 1e3);  // sec -> msec
  leGenInjLen->setText(num);

  num.setNum(params.samplingRate);
  leGenSamplingRate->setText(num);

  num.setNum(params.inputChannelNumber);
  leGenInChannelNum->setText(num);

  num.setNum(params.outputChannelNumber);
  leGenOutChannelNum->setText(num);


  // Calibration params
  num.setNum(params.hyperpolCurr * 1e12); // A -> pA
  leHyperpolCurr->setText(num);

  num.setNum(params.injCalAmp * 1e12); // A -> pA
  leCalInjAmp->setText(num);

  num.setNum(params.fullKernelLen * 1e3);  // sec -> msec
  leCalFKLen->setText(num);

  num.setNum(params.electrodeKernelLen * 1e3);  // sec -> msec
  leCalEKLen->setText(num);


  // Electrode measurement params
  num.setNum(params.iMaxElec * 1e12); // A -> pA
  leElecMaxCurrent->setText(num);

  num.setNum(params.iMinElec * 1e12); // A -> pA
  leElecMinCurrent->setText(num);

  num.setNum(params.numberOfLevels);
  leElecLevelNum->setText(num);

  // Membrane measurement params
  num.setNum(params.iMembStep * 1e12); // A -> pA
  leMembIStep->setText(num);

  num.setNum(params.numberOfRepeats);
  leMembRepNum->setText(num);

}


void ElectrodeCompDlg::MeasureElectrode()
{
    QString num;

    ExportParams();

    // Check the task specific parameters provided by the user
    if ( params.numberOfLevels < 2 )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Number of current levels must be at least 2"));
         return;
    }
    if ( params.iMaxElec <= params.iMinElec )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Maximal current level must be higher than minimal level"));
         return;
    }
    if ( params.iMaxElec >= outChnp[params.inputChannelNumber].maxCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested maximal current level exceeds the max limit of the channel"));
         return;
    }
    if ( params.iMinElec <= outChnp[params.inputChannelNumber].minCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested minimal current level exceeds the min limit of the channel"));
         return;
    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo->text();
    lInfo->setText("Elec. meas. in progress ...");
    lInfo->repaint();

    // Measure
    calibrator->ElectrodeMeasurement(params.numberOfLevels, params.iMinElec, params.iMaxElec);    


    if ( calibrator->elecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant calculated!"));
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator->elecRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode resistance measured!"));
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator->elecRes/1e6);       // Ohm -> MOhm
    leElecMeasRes->setText(num);

    num.setNum(calibrator->elecResStd/1e6);    // Ohm -> MOhm
    leElecResStd->setText(num);        

    int length = (int) (calibrator->elecKernelTauERatio*calibrator->elecTau*1e3);   // sec -> msec
    if ( length == 0 ) length = 1;
    num.setNum(length);
    leCalEKLen->setText(num);

    num.setNum(calibrator->elecTau*1e3);     // sec -> msec
    leElecMeasTime->setText(num);

    num.setNum(calibrator->elecTauStd*1e3);  // sec -> msec
    leElecTimeStd->setText(num);

    TimingTest();

    lInfo->setText("Electrode has been measured");
    lInfo->repaint();
    this->setEnabled(true);   

    message(QString("Electrode characteristic measurement on input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));

}


void ElectrodeCompDlg::MeasureMembrane()
{
    QString num;

    ExportParams();

    // Check the task specific parameters provided by the user
    if ( params.iMembStep >= outChnp[params.inputChannelNumber].maxCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested maximal current step exceeds the max limit of the channel"));
         return;
    }
    if ( params.iMembStep <= outChnp[params.inputChannelNumber].minCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested minimal current step exceeds the min limit of the channel"));
         return;
    }

    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo->text();
    lInfo->setText("Memb. meas. in progress ...");
    lInfo->repaint();

    // Measure
    calibrator->MembraneMeasurement(params.numberOfRepeats, params.iMembStep);

    if ( calibrator->membRes <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane resistance measured!"));
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        return;
    }
    if ( calibrator->membTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive membrane time constant measured!"));
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        return;
    }

    // Display the calculated/measured results
    num.setNum(calibrator->membRes/1e6);       // Ohm -> MOhm
    leCellMeasRes->setText(num);

    int length = (int) (calibrator->fullKernelTauMRatio*calibrator->membTau*1e3);   // sec -> msec
    if ( length == 0 ) length = 1;
    num.setNum(length);
    leCalFKLen->setText(num);

    num.setNum(calibrator->membResStd/1e6);    // Ohm -> MOhm
    leCellResStd->setText(num);

    num.setNum(calibrator->membTau*1e3);       // sec -> msec
    leCellMeasTime->setText(num);

    num.setNum(calibrator->membTauStd*1e3);    // sec -> msec
    leCellTimeStd->setText(num);

    TimingTest();

    lInfo->setText("Membrane has been measured");
    lInfo->repaint();
    this->setEnabled(true);

    message(QString("Membrane characteristic measurement on input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));

}


void ElectrodeCompDlg::CalibrateElectrode()
{
    QString num;

    ExportParams();

    // Check the task specific parameters provided by the user
    if ( params.injCalAmp+params.hyperpolCurr >= outChnp[params.inputChannelNumber].maxCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested calibration current (amplitude + hyperolarizing level) exceeds the max limit of the channel"));
         return;
    }
    if ( params.injCalAmp+params.hyperpolCurr <= outChnp[params.inputChannelNumber].minCurrent )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Requested calibration current (amplitude + hyperolarizing level) exceeds the min limit of the channel"));
         return;
    }
    if ( params.fullKernelLen >= params.injLen )
    {
         QMessageBox::warning(this, tr("Warning"), tr("Full kernel must be longer than the injection!"));
         return;
    }


    // Init
    if ( InitCalibrator() == false ) return;

    QString prevText = lInfo->text();
    lInfo->setText("Calibration in progress ...");
    lInfo->repaint();

    // Calibrate
    calibrator->Calibration(tabNum-1, (int) (params.fullKernelLen*params.samplingRate), (int) (params.electrodeKernelLen*params.samplingRate), params.injCalAmp, params.hyperpolCurr);

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
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        calibrator->DCT->aecChannels[tabNum-1]->Inactivate();
        return;
    }

    if ( calibrator->calElecTau <= 0.0 )   // Some error handling
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error: Nonpositive electrode time constant calculated!\n AEC channel is inactivated."));
        lInfo->setText(prevText);
        lInfo->repaint();
        this->setEnabled(true);
        calibrator->DCT->aecChannels[tabNum-1]->Inactivate();
        return;
    }

    num.setNum(calibrator->calMembRes/1e6); // Ohm -> MOhm
    leCellCalibRes->setText(num);

    num.setNum(calibrator->calElecRes/1e6); // Ohm -> MOhm
    leElecCalibRes->setText(num);

    num.setNum(calibrator->calMembTau*1e3); // sec -> msec
    leCellCalibTime->setText(num);

    num.setNum(calibrator->calElecTau*1e3); // sec -> msec
    leElecCalibTime->setText(num);

    TimingTest();

    lInfo->setText("Electrode has been calibrated");
    lInfo->repaint();
    cbCompOn->setEnabled(true);
    cbCompOn->setCheckable(true);
    cbCompOn->setChecked(true);
    this->setEnabled(true);

    message(QString("Calibration on input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));
}


// Initializes the Calibrator object with the parameters read from the dialog window
// and the input/output channels as well
// warns if the DAQ has not yet been initialized or the input or output channel
// are invalid (not active) and also returns false in these cases
bool ElectrodeCompDlg::InitCalibrator()
{
    long int numOfSamp = (long) (params.injLen*params.samplingRate);
    double sampPer = 1.0 / params.samplingRate;

    DAQ *board = ((MyMainWindow*) this->parent())->board;

    if ( board->initialized == false )
    {
        QMessageBox::warning(this, tr("Warning"), tr("DAQ not initialized"));
        return false;
    }

    DCThread *DCT = (((MyMainWindow*) this->parent())->DCT);

    // General init    
    calibrator->GeneralInit(numOfSamp, sampPer, board, DCT);

    this->setEnabled(false);

    // Channel init
    switch ( calibrator->ChannelInit(params.inputChannelNumber, params.outputChannelNumber) )
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
    num.setNum(calibrator->averSampRate);
    leAverSamplRate->setText(num);

    num.setNum(calibrator->stdSampRate);
    leSTDSamplingRate->setText(num);

    num.setNum(calibrator->minSampRate);
    leMinSRate->setText(num);

    num.setNum(calibrator->desPerMinRate);
    leDesPerMinRate->setText(num);

    num.setNum(calibrator->maxSampTime*1e3);  // sec -> msec
    leMaxSTime->setText(num);
}
