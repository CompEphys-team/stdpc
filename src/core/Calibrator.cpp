
// For testing
#include <fstream>

#include "Calibrator.h"
#include <QObject>

#include <time.h>
#include <cmath>

#include "SimulDAQ.h"
#ifdef NATIONAL_INSTRUMENTS
#include "Nidaq.h"
#endif


Calibrator::Calibrator() :
        QObject()
{
    currents = QVector<double>(0);
    voltages = QVector<double>(0);
    times    = QVector<double>(0);

    kerCalc = new KernelCalculator();

    elecKernelTauERatio = 10;  // The length of the electrode kernel is how many times the electrode time constant
    fullKernelTauMRatio = 1;   // The length of the full kernel is how many times the membrane time constant

}


void Calibrator::GeneralInit(int numOfSamp, double sampPer, DAQ *brd, DCThread *DCThrd)
{
    // Set general params
    numberOfSamples = numOfSamp;
    samplingPeriod = sampPer;

    // Reinit data storing objects
    currents.clear();
    currents.resize(numberOfSamples);
    voltages.clear();
    voltages.resize(numberOfSamples);
    times.clear();
    times.resize(numberOfSamples);

    board = brd;
    DCT = DCThrd;

    fname="test.dat"; // for testing results
}


int Calibrator::ChannelInit(int inChNum, int outChNum)
{
    inputChannelNumber = inChNum;
    outputChannelNumber = outChNum;

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
        if(inIdx[i] == inputChannelNumber){
             valid= true;
        }
    }

    if (valid == false) return 1;  // Return code for inactive input channel

    valid= false;

    for (i= 0; i < outNo; i++) {
        if(outIdx[i] == outputChannelNumber){
             valid= true;
        }
    }

    if (valid == false) return 2;  // Return code for inactive output channel

    board->generate_scan_list(1,&(inputChannelNumber));
    board->generate_analog_out_list(1,&(outputChannelNumber));

    return 0;   // Return code for successfull channel initialization
}


// Gets the steady state electrode resistance at different current levels,
// computes the variance in the R values for assessing electrode linearity
// and also estimates the electrode time constant
void Calibrator::ElectrodeMeasurement(int levelNum, double minI, double maxI)
{    
    int sample, level, i, samplesPerLevel;

    double vOffSet;
    double startSSRatio = 0.6;  // the ratio of the sampling time after which the response is considered to be steady state
    double tauFactor0 = 0.693;  // we are looking for the expected response level at (tauFactor * tau), 1-exp(-0.693) = 0.5 => halfway to the steady state response
    double tauFactor;
    double fraction;

    QVector<double> iLevels(0);   // Current levels
    QVector<double> vLevels(0);   // Steady state voltages
    QVector<double> tauE(0);      // Calculated time constants
    QVector<double> rLevels(0);   // Calculated resistances


    // Init current levels
    int zeroOcc = 0;
    double ilevel;
    for ( i=0; i<levelNum; i++ )
    {
        ilevel = minI + i*(maxI-minI)/(levelNum-1);
        if ( ilevel != 0.0 )   iLevels.append(ilevel);
        else                   zeroOcc++;    // Skip injection at zero level
    }
    levelNum -= zeroOcc;

    vLevels.resize(levelNum);
    tauE.resize(levelNum);
    rLevels.resize(levelNum);

    samplesPerLevel = numberOfSamples/levelNum;
    numberOfSamples = samplesPerLevel * levelNum; // make numberOfSamples/levelNum an integer number for easier data processing


    // Generate input current
    for ( level=0; level<levelNum; level++ )
        for ( sample=0; sample<samplesPerLevel; sample++ )
            currents[level*samplesPerLevel+sample] = iLevels[level];

    //------------------- Injection & Recording ----------------//
    // Offset measurement
    vOffSet = VoltageOffsetMeasurement();

    // Current injection and voltage recording
    InjectAndRecord(numberOfSamples);
    //----------------------------------------------------------//

    // Process each current level
    for ( level=0; level<levelNum; level++ )
    {
        int startIndex = level   * samplesPerLevel;     // Startindex of the current level
        int endIndex = (level+1) * samplesPerLevel;     // Endindex of the current level (excluded)

        // Calculate average voltage response from the end of the current level
        for ( i = startIndex + (int) (startSSRatio*samplesPerLevel); i<endIndex; i++ )  vLevels[level] += voltages[i];
        vLevels[level] /= (samplesPerLevel - (int) (startSSRatio*samplesPerLevel));

        // Calculate time constant
        double prevVLevel = (level == 0) ? vOffSet : vLevels[level-1];
        double vJump = vLevels[level]-prevVLevel;
        double startTime  = (level == 0) ? 0.0 : times[startIndex-1];
        tauFactor = tauFactor0;
        fraction = 1-exp(-tauFactor0);

        for ( i=startIndex; i<endIndex; i++ )
        {
            if ( (voltages[i]-prevVLevel)/vJump > fraction )    // get the sample which exceeds the predefined ratio
            {
                fraction = (voltages[i]-prevVLevel)/vJump;      // recalculate fraction and tauFactor to reduce digitalization error
                tauFactor = -log(1-fraction);
                break;   // get the sample which exceeds the predefined ratio                
            }
        }
        if ( i==endIndex ) tauE[level] = 0.0; // erroreous case, sampling freq can be too slow
        else               tauE[level] = (times[i]-startTime) / tauFactor; // calculate tau
    }

    // Resistances
    for ( i=0; i<levelNum; i++ )  rLevels[i] = (vLevels[i]-vOffSet)/iLevels[i];

    // Average resistance
    elecRes = 0.0;
    for ( i=0; i<levelNum; i++ )  elecRes += rLevels[i];
    elecRes /= levelNum;

    // Standard deviation in resistance
    elecResStd = 0.0;
    for ( i=0; i<levelNum; i++ )  elecResStd += (rLevels[i] - elecRes) * (rLevels[i] - elecRes);
    elecResStd = sqrt(elecResStd/levelNum);

    // Average time constant
    elecTau = 0.0;
    for ( i=0; i<levelNum; i++ )  elecTau += tauE[i];
    elecTau /= levelNum;

    // Standard deviation in time constant
    elecTauStd = 0.0;
    for ( i=0; i<levelNum; i++ )  elecTauStd += (tauE[i] - elecTau) * (tauE[i] - elecTau);
    elecTauStd = sqrt(elecTauStd/levelNum);


    //-----------------Only for testing---------------//
    fname = QString("iLevelsElec_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, iLevels);
    fname = QString("vLevelsElec_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, vLevels);
    fname = QString("rLevelsElec_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, rLevels);
    fname = QString("tauE_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, tauE);
    //-----------------Only for testing---------------//
}


// Measures the passive membrane response (time constant and steady state resistance)
// while stepping the input current from 0 to maxI,
// and averages the calculated R_m and Tau_m values over 'repeat' number of repeats
void Calibrator::MembraneMeasurement(int repeat, double iStep)
{
    int sample, rep, i, samplesPerRepeat;

    double startSSRatio = 0.8; // The ratio of the sampling time after which the response is considered to be steady state
    double tauFactor0 = 0.693;  // we are looking for the expected response level at (tauFactor * tau), 1-exp(-0.693) = 0.5 => halfway to the steady state response
    double tauFactor;
    double fraction;

    QVector<double> vOffSets(repeat);   // Voltage offsets
    QVector<double> vLevels(repeat);   // Steady state voltages
    QVector<double> tauM(repeat);      // Calculated time constants
    QVector<double> rLevels(repeat);   // Calculated resistances

    samplesPerRepeat = numberOfSamples/repeat;
    numberOfSamples = samplesPerRepeat * repeat; // make numberOfSamples/repeat an integer number for easier data processing

    // Generate input current
    for ( rep=0; rep<repeat; rep++ )
        for ( sample=0; sample<samplesPerRepeat; sample++ )
            if ( sample<samplesPerRepeat/2.0 ) currents[rep*samplesPerRepeat+sample] = 0.0;   // first half: zero
            else                             currents[rep*samplesPerRepeat+sample] = iStep; // second half: maxI

    //------------------- Injection & Recording ----------------//
    // Current injection and voltage recording
    InjectAndRecord(numberOfSamples);
    //----------------------------------------------------------//


    // Process each repeat
    int currInitIndex = (int) ceil(samplesPerRepeat/2.0); // when the current level is initiated, relative to the beginning of each repeat
    int numOfInjSamples = samplesPerRepeat - currInitIndex; // number of samples during which the current level was on

    for ( rep=0; rep<repeat; rep++ )
    {
        // Calculate average voltage offset from the end of the first half of the repeat
        int startIndex = rep * samplesPerRepeat;                 // Startindex of the zero current level
        int endIndex =   rep * samplesPerRepeat + currInitIndex; // Endindex of the zero current level (excluded)

        for ( i = startIndex + (int) (startSSRatio*currInitIndex); i < endIndex; i++ )  vOffSets[rep] += voltages[i];
        vOffSets[rep] /= (currInitIndex - (int) (startSSRatio*currInitIndex));

        // Calculate average voltage response from the end of second half of the repeat
        startIndex = rep   * samplesPerRepeat + currInitIndex; // Startindex of the current level
        endIndex = (rep+1) * samplesPerRepeat;                 // Endindex of the current level (excluded)

        for ( i = startIndex + (int) (startSSRatio*numOfInjSamples); i < endIndex; i++ )  vLevels[rep] += voltages[i];
        vLevels[rep] /= (numOfInjSamples - (int) (startSSRatio*numOfInjSamples));

        // Calculate time constant
        double vJump = vLevels[rep]-vOffSets[rep]-elecRes*iStep; // membrane voltage jump (already compensated for the electrode jump)
        double vMembZeroLevel = vOffSets[rep]+elecRes*iStep;    // from which the potential difference is attributed to the membrane (assuming steady state electrode response)
        double startTime = times[startIndex-1];
        tauFactor = tauFactor0;
        fraction = 1-exp(-tauFactor0);


        for ( i = startIndex; i < endIndex; i++ ){
            if ( (voltages[i]-vMembZeroLevel)/vJump > fraction )    // get the sample which exceeds the predefined ratio
            {
                fraction = (voltages[i]-vMembZeroLevel)/vJump;      // recalculate fraction and tauFactor to reduce digitalization error
                tauFactor = -log(1-fraction);
                break;   // get the sample which exceeds the predefined ratio
            }
        }

        if ( i==endIndex ) tauM[rep] = 0.0; // erroreous case, sampling freq can be too slow
        else               tauM[rep] = (times[i]-startTime) / tauFactor; // calculate tau

    }

    // Resistances
    for(i=0; i<repeat; i++)  rLevels[i] = (vLevels[i]-vOffSets[i])/iStep - elecRes;

    // Average resistance
    membRes = 0.0;
    for(i=0; i<repeat; i++)  membRes += rLevels[i];
    membRes /= repeat;

    // Standard deviation in resistance
    membResStd = 0.0;
    for ( i=0; i<repeat; i++ )  membResStd += (rLevels[i] - membRes) * (rLevels[i] - membRes);
    membResStd = sqrt(membResStd/repeat);

    // Average time constant
    membTau = 0.0;
    for(i=0; i<repeat; i++) membTau += tauM[i];
    membTau /= repeat;

    // Standard deviation in time constant
    membTauStd = 0.0;
    for ( i=0; i<repeat; i++ )  membTauStd += (tauM[i] - membTau) * (tauM[i] - membTau);
    membTauStd = sqrt(membTauStd/repeat);


    //-----------------Only for testing---------------//
    fname = QString("iLevelsMemb_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, *(new QVector<double>(iStep)));
    fname = QString("vLevelsMemb_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, vLevels);
    fname = QString("rLevelsMemb_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, rLevels);
    fname = QString("tauM_")+QString::number(inputChannelNumber)+QString(".dat");
    saveData(fname, tauM);
    //-----------------Only for testing---------------//
}


void Calibrator::Calibration(int elecNum, int fullKLen, int elecKLen, double injAmp, double hyperpolCurr)
{
    fullKerLen = fullKLen;
    elecKerLen = elecKLen;

    // Create current input
    int sample;
    double rnd;
    srand((unsigned) time(NULL));
    for (sample=0; sample<numberOfSamples-fullKerLen; sample++){
        rnd = 2*(((double) rand())/RAND_MAX-0.5);     // Random number in [-1, 1]
        currents[sample]= injAmp*rnd+hyperpolCurr;    // Generate a random current value
    }
    for (sample=numberOfSamples-fullKerLen-1; sample<numberOfSamples; sample++)
        currents[sample]= 0.0+hyperpolCurr; // Zero current at the last bit of the input (for fast kernel calculation)

    //------------------- Injection & Recording ----------------//
    // Obtain current-voltage pairs
    InjectAndRecord(numberOfSamples);        
    //----------------------------------------------------------//

    // Subtract hyperpolarizing current used
    for (sample=0; sample<numberOfSamples; sample++)
        currents[sample] -= hyperpolCurr;

    kerCalc->SetParams(fullKerLen, elecKerLen, 1.0/samplingPeriod, currents, voltages);

    kerCalc->CalculateKernel();

    calMembRes = kerCalc->GetRM();
    calMembTau = kerCalc->GetTauM();
    calElecRes = kerCalc->GetRE();
    calElecTau = kerCalc->GetTauE();


//    //-----------------------------------
//    // FullKerLen and ElecKerLen tester code
//    QString fre = "re.dat";
//    QString frm = "rm.dat";
//    QString ftaum = "taum.dat";
//    QString ftaue = "taue.dat";
//
//    ofstream os1(fre.toAscii());
//    ofstream os2(frm.toAscii());
//    ofstream os3(ftaum.toAscii());
//    ofstream os4(ftaue.toAscii());
//
//    QVector<double> Re, TauE, Rm, TauM, Kf;
//
//    int minKeLen = 5;
//    int maxKeLen = 80;
//    int minKfLen = 100;
//    int maxKfLen = 500;
//
//    for(int i=minKfLen; i<maxKfLen; i++)  {
//
//        Re.clear();
//        Rm.clear();
//        TauM.clear();
//        TauE.clear();
//
//        Re.resize(maxKeLen-minKeLen);
//        Rm.resize(maxKeLen-minKeLen);
//        TauM.resize(maxKeLen-minKeLen);
//        TauE.resize(maxKeLen-minKeLen);
//
//        for(int j=minKeLen; j<maxKeLen; j++){
//            if(j<i){
//                kerCalc->SetParams(i, j, 1.0/samplingPeriod, currents, voltages);
//                kerCalc->CalculateKernel();
//
//                Re[j-minKeLen] = kerCalc->GetRE();
//                if(Re[j-minKeLen] > 1e9) Re[j-minKeLen] = 1e9;
//                if(Re[j-minKeLen] < -1e9) Re[j-minKeLen] = -1e9;
//                Rm[j-minKeLen] = kerCalc->GetRM();
//                if(Rm[j-minKeLen] > 1e9) Rm[j-minKeLen] = 1e9;
//                if(Rm[j-minKeLen] < -1e9) Rm[j-minKeLen] = -1e9;
//                TauM[j-minKeLen] = kerCalc->GetTauM();
//                if(TauM[j-minKeLen] > 1e5) TauM[j-minKeLen] = 1e5;
//                if(TauM[j-minKeLen] < -1e5) TauM[j-minKeLen] = -1e5;
//                TauE[j-minKeLen] = kerCalc->GetTauE();
//                if(TauE[j-minKeLen] > 1e5) TauE[j-minKeLen] = 1e5;
//                if(TauE[j-minKeLen] < -1e5) TauE[j-minKeLen] = -1e5;
//            }
//        }
//
//        for (int k= minKeLen; k < maxKeLen; k++) {
//            os1 << Re[k-minKeLen] << " ";
//            os2 << Rm[k-minKeLen] << " ";
//            os3 << TauM[k-minKeLen] << " ";
//            os4 << TauE[k-minKeLen] << " ";
//        }
//
//        os1 << "\n";
//        os2 << "\n";
//        os3 << "\n";
//        os4 << "\n";
//    }
//
//    os1.close();
//    os2.close();
//    os3.close();
//    os4.close();
//    //-----------------------------------

    // Init AECChannel with the kernel
    DCT->aecChannels[elecNum]->Initialize(inputChannelNumber, outputChannelNumber, samplingPeriod, kerCalc->GetElecKernel());

    // Save the kernel for testing   
    QString fname = QString("kernel_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, kerCalc->GetElecKernel());

    // To test
    fname = QString("full_kernel_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, kerCalc->fullKernel);
    fname = QString("currents_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, currents);
    fname = QString("voltages_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, voltages);
    fname = QString("times_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, times);

}


// Completes the sampling cycle by waiting till the end of the sampling period
double Calibrator::WaitTillNextSampling(double time)
{
    double t = 0.0;

    do
    {
        t += board->get_RTC();
    } while (t < time);

    return t;
}


// Injects the current and records the voltage
// for 'numOfSamples' samples
void Calibrator::InjectAndRecord(int numOfSamples)
{
    int sample;
    double t, time;

    // Error handling
    if ( currents.size() < numOfSamples ) currents.resize(numOfSamples);
    if ( voltages.size() < numOfSamples ) voltages.resize(numOfSamples);
    if ( times.size()    < numOfSamples ) times.resize(numOfSamples);

    t = 0.0; // init time

    // Create tolerance limits
    double higherLimit = inChnp[inputChannelNumber].minVoltage + 0.9*(inChnp[inputChannelNumber].maxVoltage-inChnp[inputChannelNumber].minVoltage);
    double lowerLimit =  inChnp[inputChannelNumber].minVoltage + 0.1*(inChnp[inputChannelNumber].maxVoltage-inChnp[inputChannelNumber].minVoltage);
    bool limitWarningEmitted = false;

    board->reset_board();

    for ( sample=1; sample<numOfSamples; sample++ )
    {

        // --- Write --- //
        outChn[outputChannelNumber].I= currents[sample];
        board->write_analog_out(outChn);
        // --- Write end --- //


        // --- Wait --- //
        time = (sample+1)*samplingPeriod - t; // wait till the next sampling period is over
        if ( time < 0.0 ) time = 0.0; // in case there was an OS interruption and we've already overrun: let's not wait any time
        t += WaitTillNextSampling(time);
        times[sample] = t; // save the time stamp
        // --- Wait end --- //


        // --- Read --- //
        board->get_scan(inChn);
        voltages[sample]= inChn[inputChannelNumber].V; // save voltage
        if ( voltages[sample] > higherLimit || voltages[sample] < lowerLimit ) // check channel limits
            if ( limitWarningEmitted == false){
                  emit CloseToLimit(QString("Voltage"), inputChannelNumber, inChnp[inputChannelNumber].minVoltage, inChnp[inputChannelNumber].maxVoltage, voltages[sample]);
                  limitWarningEmitted = true;
            }
        // --- Read end --- //

    }

    // Reset board
    board->reset_board();
}


// Measures and returns the voltage offset
double Calibrator::VoltageOffsetMeasurement()
{
    int sample;
    double vOffset = 0.0;
    double offsetMeasLen = 5.0; // in msec
    int offsetSamplingNum = (int) (offsetMeasLen*1e-3/samplingPeriod);  // how many sampling steps

    board->reset_RTC();

    // Measure the voltage offset
    for (sample=0; sample<offsetSamplingNum; sample++)
    {
        // Channel read
        board->get_scan(inChn);

        // Save voltage
        vOffset += inChn[inputChannelNumber].V;

        WaitTillNextSampling(samplingPeriod);
    }
    vOffset /= offsetSamplingNum;

    return vOffset;
}


// Calculate some statistics about the previous sampling
void Calibrator::CalcSamplingStats()
{
    int i;

    if ( times.size() == 0 ) return;

    QVector<double> rates = QVector<double>(times.size()-1);

    // Get the sampling rates
    for ( i=0; i<rates.size(); i++ )
    {
        if ( times[i+1] == times[i] ) return;   // we don't want to devide by zero

        rates[i] = 1.0/(times[i+1]-times[i]);
    }

    // Average sampling rate (Hz)
    averSampRate = 0.0;
    for ( int i=0; i<rates.size(); i++ ) averSampRate += rates[i];
    averSampRate /= rates.size();

    // Standard deviation of average sampling rate (Hz)
    stdSampRate = 0.0;
    for ( i=0; i<rates.size(); i++ ) stdSampRate += (rates[i] - averSampRate) * (rates[i] - averSampRate);
    stdSampRate = sqrt(stdSampRate/rates.size());

    // Minimal rate (Hz)
    minSampRate = 1.0/samplingPeriod;
    for ( i=0; i<rates.size(); i++ ) if ( rates[i] < minSampRate ) minSampRate = rates[i];

    // Minimal / Desired rate ratio
    desPerMinRate = 1.0 / samplingPeriod / minSampRate;

    // Maximal sampling time (msec)
    maxSampTime = 1.0 / minSampRate;

// Testing the timeing
//    QString timefn = "time.dat";
//    saveData(timefn, times);
//    QString ratefn = "rate.dat";
//    saveData(ratefn, rates);
}


// Turns on/off the compensation on the corresponding electrode
void Calibrator::SetChannelActivation(int elecNum, bool state)
{
    if ( state == true ) DCT->aecChannels[elecNum]->Activate();
    else                 DCT->aecChannels[elecNum]->Inactivate();
}


// For testing and checking purposes
void Calibrator::saveData(QString &fname, QVector<double> data)
{
  ofstream os(fname.toAscii());

  for (int i= 0; i < data.size(); i++) {
    os << data[i] << "\n";
  }

  os.close();
}

