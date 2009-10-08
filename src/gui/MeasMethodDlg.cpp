#include <fstream>
#include <time.h>
#include <math.h>
#include <QFile>
#include <QTextStream>

#include "MeasMethodDlg.h"
#include "ui_MeasMethodDlg.h"
#include "mainwin.h"

#include "SimulDAQ.h"
#ifdef NIDAQ
#include "NIDAQ.h"
#endif


MeasMethodDlg::MeasMethodDlg(QWidget *parent) :
    QDialog(parent)   
{   
    setupUi(this);

    currents = QVector<double>(0);
    voltages = QVector<double>(0);
    beforeCompVs = QVector<double>(0);
    afterCompVs = QVector<double>(0);
    controlVs = QVector<double>(0);

    lInfo->setVisible(false);

    leTestSqWaveLength->setVisible(false);
    labelTestSqWaveLength->setVisible(false);  

    QObject::connect(cbMeasType, SIGNAL(currentIndexChanged(int)), this, SLOT(newMethodSelected()));
    QObject::connect(cbTestInputType, SIGNAL(currentIndexChanged(int)), this, SLOT(newInputTypeSelected()));
    QObject::connect(linCheckButton, SIGNAL(released()), this, SLOT(checkLin()));
    QObject::connect(calibrateButton, SIGNAL(released()), this, SLOT(calibrate()));
    QObject::connect(loadKernelButton, SIGNAL(released()), this, SLOT(loadKernel()));
    QObject::connect(testKernelButton, SIGNAL(released()), this, SLOT(testKernel()));
}

MeasMethodDlg::~MeasMethodDlg()
{
}

void MeasMethodDlg::exportParams()
{
  // General params
  params.oneElectrode        = cbMeasType->currentIndex();
  params.injLen              = leGenInjLen->text().toDouble() * 1e-3;  // msec -> sec
  params.samplingRate        = leGenSamplingRate->text().toDouble();   // Hz = 1/sec
  params.inputChannelNumber  = leGenInChannelNum->text().toInt();
  params.outputChannelNumber = leGenOutChannelNum->text().toInt();

  // Calibration params
  params.injCalAmp           = leCalInjAmp->text().toDouble() * 1e-12; // pA -> A

  // Linearity check params
  params.iMax                = leLinMaxCurrent->text().toDouble() * 1e-12; // pA -> A
  params.iMin                = leLinMinCurrent->text().toDouble() * 1e-12; // pA -> A
  params.numberOfLevels      = leLinLevelNum->text().toInt();

  // Kernel test params
  params.testInputType       = cbTestInputType->currentIndex();
  params.injTestAmp          = leTestInjAmp->text().toDouble() * 1e-12;      // pA -> A
  params.sqWaveLen           = leTestSqWaveLength->text().toDouble() * 1e-3; // msec -> sec
  params.controlChannelNumber= leTestCtrlChannelNum->text().toInt();
}

void MeasMethodDlg::importParams()
{
  QString num;

  // General params
  if (params.oneElectrode) cbMeasType->setCurrentIndex(0);
  else                     cbMeasType->setCurrentIndex(1);

  num.setNum(params.injLen * 1e3);  // sec -> msec
  leGenInjLen->setText(num);

  num.setNum(params.samplingRate);
  leGenSamplingRate->setText(num);

  num.setNum(params.inputChannelNumber);
  leGenInChannelNum->setText(num);

  num.setNum(params.outputChannelNumber);
  leGenOutChannelNum->setText(num);

  // Calibration params
  num.setNum(params.injCalAmp * 1e12); // A -> pA
  leCalInjAmp->setText(num);

  // Linearity check params
  num.setNum(params.iMax * 1e12); // A -> pA
  leLinMaxCurrent->setText(num);

  num.setNum(params.iMin * 1e12); // A -> pA
  leLinMinCurrent->setText(num);

  num.setNum(params.numberOfLevels);
  leLinLevelNum->setText(num);

  // Kernel test params
  cbTestInputType->setCurrentIndex(params.testInputType);

  num.setNum(params.injTestAmp * 1e12); // A -> pA
  leTestInjAmp->setText(num);

  num.setNum(params.sqWaveLen * 1e3); // sec -> msec
  leTestSqWaveLength->setText(num);

  num.setNum(params.controlChannelNumber);
  leTestCtrlChannelNum->setText(num);

}

void MeasMethodDlg::newMethodSelected()
{    
    if(cbMeasType->currentIndex() == 0){        
        aecTools->setEnabled(true);
    }
    else {        
        aecTools->setEnabled(false);
    }
}

void MeasMethodDlg::newInputTypeSelected()
{
    if(cbTestInputType->currentIndex() == 3){
        leTestSqWaveLength->setVisible(true);
        labelTestSqWaveLength->setVisible(true);
    }
    else {
        leTestSqWaveLength->setVisible(false);
        labelTestSqWaveLength->setVisible(false);
    }
}


void MeasMethodDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MeasMethodDlg::checkLin()
{
    int sample, level, i, levelNum;
    int offSetMeasureLength = 50;
    double dt, t = 0.0;
    double vOffSet;
    double startSSRatio = 0.6; // The ratio of the sampling time after which the response is considered to be steady state
    double avrg = 0.0;
    double std = 0.0;

    QString num;

    QVector<double> iLevels = QVector<double>(0);   // Current levels
    QVector<double> vLevels = QVector<double>(0);   // Steady state voltages
    QVector<double> rLevels = QVector<double>(0);   // Calculated resistances

    QString fname_results="linearityResults.dat";

    if(generalInit(false) == false) return;

    levelNum = params.numberOfLevels;

    if(levelNum < 2){
         QMessageBox::warning(this, tr("Warning"), tr("Number of test levels must be at least 2"));
         this->setEnabled(true);
         return;
    }

    if(params.iMax <= params.iMin){
         QMessageBox::warning(this, tr("Warning"), tr("Maximal level must be higher than minimal level"));
         this->setEnabled(true);
         return;
    }  

    // Init current levels
    double ilevel;
    for(i=0; i<params.numberOfLevels; i++){
        ilevel = params.iMin + i*(params.iMax-params.iMin)/(params.numberOfLevels-1);
        if(ilevel != 0.0)   iLevels.append(ilevel);
        else                --levelNum;    // Skip injection at zero level
    }  

    vLevels.resize(levelNum);
    rLevels.resize(levelNum);    


    // Measure the voltage offset
    vOffSet = 0.0;
    for (sample=0; sample<offSetMeasureLength; sample++)
    {
        // Channel read
        board->get_scan(inChn);

        // Save voltage
        vOffSet += inChn[params.inputChannelNumber].V;
    }
    vOffSet /= offSetMeasureLength;


    for (level=0; level<levelNum; level++)
    {
        num.setNum(level+1);
        lInfo->setText("Linearity check ... level "+num);
        lInfo->repaint();

        // Set output to the next level
        outChn[params.outputChannelNumber].I= iLevels[level];
        board->write_analog_out(outChn);


        board->reset_RTC();
        ////////////////////////////////////////////////
        // (Not so) Time cricital loop begins
        for (sample=0; sample<numberOfSamples; sample++)
        {
            // Channel read
            board->get_scan(inChn);

            // Save voltage
            voltages[sample]= inChn[params.inputChannelNumber].V;

            // Wait till the next sampling period is over
            dt= 0.0;
            do
            {
                dt+= board->get_RTC();
            } while (dt < samplingPeriod);
            t+= dt;
        }
        // (Not so) Time cricital loop ends
        ////////////////////////////////////////////////

        // Calculate average voltage response from the end of the recording
        for(i=(int) (startSSRatio*numberOfSamples); i<numberOfSamples; i++){
            vLevels[level] += voltages[i];
        }
        vLevels[level] /= (numberOfSamples - (int) (startSSRatio*numberOfSamples));

    }

    // Reset board
    board->reset_board();

    lInfo->setText("Calculating linearity ...");
    lInfo->repaint();

    // Resistances
    for(i=0; i<levelNum; i++){
        rLevels[i] = (vLevels[i]-vOffSet)/iLevels[i];
    }

    // Average resistance
    for(i=0; i<levelNum; i++){
        avrg += rLevels[i];
    }
    avrg /= levelNum;

    // Standard deviation
    for(i=0; i<levelNum; i++){
        std += (rLevels[i] - avrg) * (rLevels[i] - avrg);
    }
    std = sqrt(std/levelNum);

    num.setNum(avrg/1e6);       // Ohm -> MOhm
    leLinAverRes->setText(num);

    num.setNum(std/1e6);        // Ohm -> MOhm
    leLinResStd->setText(num);

    lInfo->setText("Writing out data ...");
    lInfo->repaint();

    saveData(fname_results, rLevels);

    lInfo->setText("Timing test calculation ...");
    lInfo->repaint();

    lInfo->setVisible(false);

    this->setEnabled(true);

    message(QString("Linearity check of input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));
}

void MeasMethodDlg::calibrate()
{    
    int sample;
    double dt, t= 0.0;

    QString num;
    QString fname_currents="I.dat";
    QString fname_voltages="V.dat";    

    if(generalInit(false) == false) return;

    // Create input
    lInfo->setText("Initializing current input ...");
    lInfo->repaint();

    createInputCurrent(false);

    lInfo->setText("Calibration in progress ...");
    lInfo->repaint();

    ////////////////////////////////////////////////
    // Time cricital loop begins
    board->reset_RTC();

    for (sample=0; sample<numberOfSamples; sample++)
    {
        // Channel read
        board->get_scan(inChn);

        // Save voltage
        voltages[sample]= inChn[params.inputChannelNumber].V;

        // Channel write
        outChn[params.outputChannelNumber].I= currents[sample];
        board->write_analog_out(outChn);        

        // Wait till the next sampling period is over
        dt= 0.0;
        do
        {
            dt+= board->get_RTC();
        } while (dt < samplingPeriod);
        t+= dt;

        // Saving time
        times[sample]= t;
    }
    // Time cricital loop ends
    ////////////////////////////////////////////////

    // Reset board
    board->reset_board();

    lInfo->setText("Writing out data ...");
    lInfo->repaint();

    saveData(fname_currents.insert(1,num.setNum(params.inputChannelNumber)), currents);
    saveData(fname_voltages.insert(1,num.setNum(params.inputChannelNumber)), voltages);

    lInfo->setText("Timing test calculation ...");
    lInfo->repaint();

    timeTest();

    //------------------------------------------//
    // Objects for testing the data acquisition //
    //QString fname_times="times.dat";
    //saveData(fname_times, times);

    lInfo->setVisible(false);

    this->setEnabled(true);

    message(QString("Calibration of input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));
}

void MeasMethodDlg::testKernel()
{
    int sample;
    double dt, t= 0.0;

    QString num;
    QString fname_currents="testI.dat";
    QString fname_beforeCompV="testVb.dat";
    QString fname_afterCompV="testVa.dat";
    QString fname_controlV="testVc.dat";

    if(generalInit(true) == false) return;

    // Create input
    lInfo->setText("Initializing current input ...");
    lInfo->repaint();

    createInputCurrent(true);

    lInfo->setText("Kernel test in progress ...");
    lInfo->repaint();

    ////////////////////////////////////////////////
    // Time cricital loop begins
    board->reset_RTC();

    for (sample=0; sample<numberOfSamples; sample++)
    {
        // Channel read
        board->get_scan(inChn);

        // Acquiring control data, if necessery
        if(params.controlChannelNumber != -1)    controlVs[sample]= inChn[params.controlChannelNumber].V;

        // Compensation with the current I value
        DCT->aecChannels->value(0)->calcVe(outChn[params.outputChannelNumber].I);

        // TODO: implement AEC channel selection, instead of passively using the first channel DCT->aecChannels->value(0)
        // Save voltages
        beforeCompVs[sample]= inChn[params.inputChannelNumber].V;
        afterCompVs[sample]= beforeCompVs[sample] - DCT->aecChannels->value(0)->Ve;

        // Channel write
        outChn[params.outputChannelNumber].I= currents[sample];
        board->write_analog_out(outChn);


        // Wait till the next sampling period is over
        dt= 0;
        do
        {
            dt+= board->get_RTC();
        } while (dt < samplingPeriod);
        t+= dt;

        // Saving time
        times[sample]= t;

    }
    // Time cricital loop ends
    ////////////////////////////////////////////////

    // Reset board and AEC channel
    board->reset_board();

    // TODO: implement AEC channel selection, instead of passively using the first channel DCT->aecChannels->value(0)
    // Reset AEC channel
    DCT->aecChannels->value(0)->resetChannel();

    lInfo->setText("Writing out data ...");
    lInfo->repaint();

    saveData(fname_currents.insert(5,num.setNum(params.inputChannelNumber)), currents);
    saveData(fname_beforeCompV.insert(6,num.setNum(params.inputChannelNumber)), beforeCompVs);
    saveData(fname_afterCompV.insert(6,num.setNum(params.inputChannelNumber)), afterCompVs);
    if(params.controlChannelNumber != -1){
        saveData(fname_controlV.insert(6,num.setNum(params.controlChannelNumber)), controlVs);
    }

    lInfo->setText("Timing test calculation ...");
    lInfo->repaint();

    timeTest();

    lInfo->setVisible(false);

    this->setEnabled(true);

    message(QString("Kernel test of input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" finished."));

}

bool MeasMethodDlg::generalInit(bool kernelTest)
{
    board = ((MyMainWindow*) this->parent())->board;

    exportParams();

    // Checking parameters
    if(params.samplingRate <= 0.0){
         QMessageBox::warning(this, tr("Warning"), tr("Sampling rate must be positive"));
         return false;
    }

    if(params.testInputType == 3 && kernelTest == true){ // Square wave test input
        if(params.sqWaveLen <= 0.0){
            QMessageBox::warning(this, tr("Warning"), tr("Square wave length must be positive"));
            return false;
        }

        sqLen = (int) (params.sqWaveLen * params.samplingRate); // Square pulse length in sampling steps

        if(sqLen<=0){    // To avoid division by zero
            QMessageBox::warning(this, tr("Warning"), tr(" Sampling rate must be smaller than square square wave length in seconds"));
            return false;
        }                
    }

    numberOfSamples = (long) (params.injLen*params.samplingRate);
    samplingPeriod  = 1/params.samplingRate;

    // Reinit data storing objects
    currents.clear();
    currents.resize(numberOfSamples);
    times.clear();
    times.resize(numberOfSamples);

    if(kernelTest == false){
        voltages.clear();
        voltages.resize(numberOfSamples);
    }
    else{
        beforeCompVs.clear();
        afterCompVs.clear();
        controlVs.clear();
        beforeCompVs.resize(numberOfSamples);
        afterCompVs.resize(numberOfSamples);
        controlVs.resize(numberOfSamples);
    }

    if(board->initialized == false){
        QMessageBox::warning(this, tr("Warning"), tr("DAQ not initialized"));
        return -1;
    }

    this->setEnabled(false);

    lInfo->setVisible(true);
    lInfo->setText("Initializing channels ...");
    lInfo->repaint();

    // Do we need to monitor any control channel
    bool controlCh = (kernelTest == true) && (params.controlChannelNumber != -1);

    // Init channels
    return initChannels(controlCh);
}

bool MeasMethodDlg::initChannels(bool controlCh)
{
    int i;
    
    short int *inIdx;
    short int *outIdx;
    short int inNo;
    short int outNo;

    inChn  = new inChannel[board->inChnNo];
    outChn = new outChannel[board->outChnNo];

    inIdx  = new short int[board->inChnNo];
    outIdx = new short int[board->outChnNo];   

    inNo= 0;
    for (i= 0; i < board->inChnNo; i++) {
       if (inChnp[i].active) {
          inIdx[inNo++]= i;
          inChn[i].init(&inChnp[i]);
       }
    }

    outNo= 0;
    for (i=0; i < board->outChnNo; i++) {
        if (outChnp[i].active) {
           outIdx[outNo++]= i;
           outChn[i].init(&outChnp[i]);
        }
    }

    // Check validity of selected channels
    bool valid= false;

    for (i= 0; i < inNo; i++) {
        if(inIdx[i] == params.inputChannelNumber){
             valid= true;
        }
    }

    if (valid == false) {
             QMessageBox::warning(this, tr("Warning"), tr("Selected input channel is not an exsisting active channel"));
             lInfo->setVisible(false);
             this->setEnabled(true);
             return false;
    }

    valid= false;

    for (i= 0; i < outNo; i++) {
        if(outIdx[i] == params.outputChannelNumber){
             valid= true;
        }
    }

    if (valid == false) {
             QMessageBox::warning(this, tr("Warning"), tr("Selected output channel is not an exsisting active channel"));
             lInfo->setVisible(false);
             this->setEnabled(true);
             return false;
    }

    valid= false;

    // if there is a control input channel, let's check it
    if (controlCh){
        for (i= 0; i < inNo; i++) {
            if(inIdx[i] == params.controlChannelNumber){
                 valid= true;
            }
        }

        if (valid == false) {
                 QMessageBox::warning(this, tr("Warning"), tr("Selected control channel is not an exsisting active channel"));
                 lInfo->setVisible(false);
                 this->setEnabled(true);
                 return false;
        }
    }


    // if there is no control input channel
    if(controlCh == false){
        board->generate_scan_list(1,&(params.inputChannelNumber));
    }
    // if there is
    else {
        short int inputChnNumList[2] = {params.inputChannelNumber, params.controlChannelNumber};
        board->generate_scan_list(2,inputChnNumList);
    }

    board->generate_analog_out_list(1,&(params.outputChannelNumber));

    return true;
}


// Initializes the input current to save time in the time critical calibration loop
void MeasMethodDlg::createInputCurrent(bool testKernel)
{
    int sample;       

    // Calibration case
    if(testKernel == false){    // White noise
        double rnd;
        srand((unsigned) time(NULL));
        for (sample=0; sample<numberOfSamples; sample++){
            rnd = 2*(((double) rand())/RAND_MAX-0.5);    // Random number in [-1, 1]
            currents[sample]= params.injCalAmp*rnd;      // Generate a random current value
        }
    }
    else {      // Kernel test case
        switch(params.testInputType){
        case 0:     // White noise
            double rnd;
            srand((unsigned) time(NULL));
            for (sample=0; sample<numberOfSamples; sample++){
                rnd = 2*(((double) rand())/RAND_MAX-0.5);    // Random number in [-1, 1]
                currents[sample]= params.injTestAmp*rnd;    // Generate a random current value
            }
            break;

        case 1:     // Impulse
            for (sample=0; sample<numberOfSamples; sample++){
                if(sample == 0)
                    currents[sample]= params.injTestAmp;   // Generate the impulse value at the beginning
                else
                    currents[sample]= 0.0;
            }
            break;

        case 2:     // Step
            for (sample=0; sample<numberOfSamples; sample++){
                currents[sample]= params.injTestAmp;       // Generate the step value
            }
            break;

        case 3:     // Square waves            
            for (sample=0; sample<numberOfSamples; sample++){                                
                if((sample/sqLen) % 2 == 0)                    
                    currents[sample]= params.injTestAmp;   // Generate the positive wave
                else
                    currents[sample]= -params.injTestAmp;  // Generate the negative wave
            }
            break;
        }
    }
}


void MeasMethodDlg::saveData(QString &fname, QVector<double> data)
{
  ofstream os(fname.toAscii());
  
  for (int i= 0; i < data.size(); i++) {
    os << data[i] << "\n";   
  }
  
  os.close();
}


void MeasMethodDlg::loadKernel()
{    
    QString line;
    QVector<double> kernel;
    int lineNum= 0;
    double k= 0.0;
    bool ok= false;
    int i=0;

    QFile file(leKernelFileName->text());

    // Open the file
    if (file.open(QFile::ReadOnly) == false) {
        QMessageBox::warning(this, tr("Information"),
                                   tr("File can not be opened"),
                                   QMessageBox::Ok);
        return;
    }

    QTextStream in(&file);

    lwKernelDisplay->clear();

    // Read in the lines of the files, display them on the listwidget and save the kernel components to "kernel"
    while (!in.atEnd()) {
        lineNum++;

        line= in.readLine();

        lwKernelDisplay->addItem(line);

        k= line.toDouble(&ok);

        if (ok == false) {
            QMessageBox::warning(this, tr("Information"),
                                       tr("Line can not be converted to double: "),
                                       QMessageBox::Ok);
            return;
        }

        kernel.append(k);
    }

    exportParams();

    DCT= (((MyMainWindow*) this->parent())->DCT);

    // Check the exsistence of the selected io-channel combination among the AECChannels
    if (DCT->aecChannels != NULL)
    {
        int size= DCT->aecChannels->size();
        for (i=0; i<size; i++)
        {
            if (DCT->aecChannels->value(i)->inChnNum == params.inputChannelNumber &&
                DCT->aecChannels->value(i)->outChnNum == params.outputChannelNumber)
               break;
        }

        if (i < size) DCT->aecChannels->removeAt(i);
    }
    else{
        DCT->aecChannels = new QList<AECChannel*>;
        i= 0;
    }

    // Create a corresponding AECChannel to DCThread
    DCT->aecChannels->insert(i, new AECChannel(params.inputChannelNumber, params.outputChannelNumber));

    // Init the AECChannel with the kernel
    DCT->aecChannels->value(i)->initializeKernel(&kernel);
    
    message(QString("Kernel upload to input channel ")+QString::number(params.inputChannelNumber)+
            QString(" and output channel ")+QString::number(params.outputChannelNumber)+QString(" was successfull."));

    lwKernelDisplay->setEnabled(true);
    gbTest->setEnabled(true);

}

void MeasMethodDlg::timeTest()
{
    int i;
    double averSRate, stdSRate, minSRate, desPerMinRate, maxSTime;

    QString num;
    QVector<double> rates = QVector<double>(numberOfSamples-1);

    averSRate = 0.0;
    stdSRate = 0.0;
    minSRate = params.samplingRate;
    desPerMinRate = 0.0;
    maxSTime = 0.0;

    // Get sampling rates
    for(i=0; i<numberOfSamples-1; i++){
        if(times[i+1] == times[i]){
            QMessageBox::warning(this, tr("Information"),
                                       tr("Sampling times are equal, cannot calculate sampling rate"),
                                       QMessageBox::Ok);
            num.setNum(-1);
            leAverSamplRate->setText(num);
            leSTDSamplingRate->setText(num);
            leMinTimeB2S->setText(num);
            leDesPerMinRate->setText(num);
            return;           
        }

        rates[i] = 1 / (times[i+1] - times[i]);
    }

    // Average sampling rate (Hz)
    averSRate = 1 / (times[numberOfSamples-1]/(numberOfSamples-1));

    // Standard deviation of average sampling rate (Hz)
    for(i=0; i<numberOfSamples-1; i++){
        stdSRate += (rates[i] - averSRate) * (rates[i] - averSRate);
    }    
    stdSRate = sqrt(stdSRate/(numberOfSamples-1));

    // Minimal rate (Hz)
    for(i=0; i<numberOfSamples-1; i++){
        if(rates[i] < minSRate)   minSRate = rates[i];
    }    

    // Minimal / Desired rate ratio
    desPerMinRate = params.samplingRate / minSRate;

    // Maximal sampling time (msec)
    maxSTime = 1 / minSRate * 1000;


    // Display them
    num.setNum(averSRate);
    leAverSamplRate->setText(num);
    num.setNum(stdSRate);
    leSTDSamplingRate->setText(num);
    num.setNum(minSRate);
    leMinTimeB2S->setText(num);
    num.setNum(desPerMinRate);
    leDesPerMinRate->setText(num);
    num.setNum(maxSTime);
    leMaxSTime->setText(num);
}
