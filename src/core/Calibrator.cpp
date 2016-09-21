
// For testing
#include <fstream>

#include "Calibrator.h"
#include <QObject>

#include <time.h>
#include <cmath>

#include "DeviceManager.h"


Calibrator::Calibrator() :
        QObject()
{
    currents = QVector<double>(0);
    voltages = QVector<double>(0);
    times    = QVector<double>(0);

    kerCalc = new KernelCalculator();

    elecKernelTauERatio = 10;  // The length of the electrode kernel is how many times the electrode time constant
    fullKernelTauMRatio = 1.5; // The length of the full kernel is how many times the membrane time constant
    maxElecKFullKRation = 2.0; // To avoid to too high (electrode kernel length) / (full kernel length) ratios

}


void Calibrator::GeneralInit(DCThread *DCThrd)
{
    DCT = DCThrd;
}


int Calibrator::ChannelInit(ChannelIndex inChNum, ChannelIndex outChNum)
{
    inputChannelNumber = inChNum;
    outputChannelNumber = outChNum;

    for ( DAQ *b : Devices.actdev )
        b->init_chans();

    inBoard = Devices.getDevice(inputChannelNumber);
    inChn = Devices.getInChan(inputChannelNumber);
    if ( !inBoard || !inChn || !inChn->active )
        return 1; // Return code for inactive input channel

    outBoard = Devices.getDevice(outputChannelNumber);
    outChn = Devices.getOutChan(outputChannelNumber);
    if ( !outBoard || !outChn || !outChn->active )
        return 2; // Return code for inactive output channel

    return 0;   // Return code for successfull channel initialization
}


// Gets the steady state electrode resistance at different current levels,
// computes the variance in the R values for assessing electrode linearity
// and also estimates the electrode time constant
void Calibrator::ElectrodeMeasurement(double injLenPerLevel, double sampRate, int levelNum, double minI, double maxI)
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


    // Set general params   
    samplingPeriod = 1.0/sampRate;
    samplesPerLevel = (int) (injLenPerLevel*sampRate);
    numberOfSamples = samplesPerLevel * levelNum;
    incorrectMeasurement = 0;


    // Reinit data storing objects
    currents.clear();
    currents.resize(numberOfSamples);
    voltages.clear();
    voltages.resize(numberOfSamples);
    times.clear();
    times.resize(numberOfSamples);

    vLevels.resize(levelNum);
    tauE.resize(levelNum);
    rLevels.resize(levelNum);


    // Generate input current
    for ( level=0; level<levelNum; level++ )
        for ( sample=0; sample<samplesPerLevel; sample++ )
            currents[level*samplesPerLevel+sample] = iLevels[level];

    //------------------- Injection & Recording ----------------//
    // Offset measurement
    vOffSet = VoltageOffsetMeasurement();

    // Current injection and voltage recording
    InjectAndRecord(numberOfSamples, false);
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

                if ( fraction < 1 )  tauFactor = -log(1-fraction);
                else                 i=endIndex;  // error, voltage jump is too big (interruption?)

                break;   // get the sample which exceeds the predefined ratio                
            }
        }
        if ( i==endIndex )
        {
            tauE[level] = 0.0; // erroreous case, sampling freq can be too slow
            incorrectMeasurement++;
        }
        else tauE[level] = (times[i]-startTime) / tauFactor; // calculate tau
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
  #ifdef TEST_VERSION
    QString inChnName = inputChannelNumber.toString('-');
    fname = QString("iLevelsElec_")+inChnName+QString(".dat");
    saveData(fname, iLevels);
    fname = QString("vLevelsElec_")+inChnName+QString(".dat");
    saveData(fname, vLevels);
    fname = QString("rLevelsElec_")+inChnName+QString(".dat");
    saveData(fname, rLevels);
    fname = QString("tauE_")+inChnName+QString(".dat");
    saveData(fname, tauE);
 #endif
    //-----------------Only for testing---------------//
}


// Measures the passive membrane response (time constant and steady state resistance)
// while stepping the input current from 0 to maxI,
// and averages the calculated R_m and Tau_m values over 'repeat' number of repeats
void Calibrator::MembraneMeasurement(double injLenPerRep, double sampRate, int repeat, double iStep)
{
    int sample, rep, i, samplesPerRepeat;

    double startSSRatio = 0.8; // The ratio of the sampling time after which the response is considered to be steady state
    double tauFactor0 = 0.693;  // we are looking for the expected response level at (tauFactor * tau), 1-exp(-0.693) = 0.5 => halfway to the steady state response
    double tauFactor;
    double fraction;

    QVector<double> vOffSets(repeat);  // Voltage offsets
    QVector<double> iLevels(repeat);   // Current levels
    QVector<double> vLevels(repeat);   // Steady state voltages
    QVector<double> tauM(repeat);      // Calculated time constants
    QVector<double> rLevels(repeat);   // Calculated resistances


    // Init current levels
    for ( i=0; i<repeat; i++ ) iLevels[i] = iStep;

    // Set general params
    samplingPeriod = 1.0/sampRate;
    samplesPerRepeat = (int) (2*injLenPerRep*sampRate);  // multiplied by 2 to allow 'steady state' recording at both levels
    numberOfSamples = samplesPerRepeat * repeat; // make numberOfSamples/repeat an integer number for easier data processing
    incorrectMeasurement = 0;

    // Reinit data storing objects
    currents.clear();
    currents.resize(numberOfSamples);
    voltages.clear();
    voltages.resize(numberOfSamples);
    times.clear();
    times.resize(numberOfSamples);


    // Generate input current
    for ( rep=0; rep<repeat; rep++ )
        for ( sample=0; sample<samplesPerRepeat; sample++ )
            if ( sample<samplesPerRepeat/2.0 ) currents[rep*samplesPerRepeat+sample] = 0.0;   // first half: zero
            else                               currents[rep*samplesPerRepeat+sample] = iStep; // second half: maxI

    //------------------- Injection & Recording ----------------//
    // Current injection and voltage recording
    InjectAndRecord(numberOfSamples, false);
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

                if ( fraction < 1 )  tauFactor = -log(1-fraction);
                else                 i=endIndex;  // error, voltage jump is too big (interruption?)

                break;   // get the sample which exceeds the predefined ratio
            }
        }

        if ( i==endIndex )
        {
            tauM[rep] = 0.0; // erroreous case, sampling freq can be too slow
            incorrectMeasurement++;
        }
        else tauM[rep] = (times[i]-startTime) / tauFactor; // calculate tau

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

  #ifdef TEST_VERSION
    //-----------------Only for testing---------------//
    QString inChnName = inputChannelNumber.toString('-');
    fname = QString("iLevelsMemb_")+inChnName+QString(".dat");
    saveData(fname, iLevels);
    fname = QString("vLevelsMemb_")+inChnName+QString(".dat");
    saveData(fname, vLevels);
    fname = QString("rLevelsMemb_")+inChnName+QString(".dat");
    saveData(fname, rLevels);
    fname = QString("tauM_")+inChnName+QString(".dat");
    saveData(fname, tauM);  
    //-----------------Only for testing---------------//
  #endif
}


void Calibrator::Calibration(double injCalLen, double sampRate, int elecNum, int fullKLen, int elecKLen, double injAmp, double hyperpolCurr)
{
    fullKerLen = fullKLen;
    elecKerLen = elecKLen;   

    // Set general params
    samplingPeriod = 1.0/sampRate;
    numberOfSamples = (long) (injCalLen * sampRate);

    // Reinit data storing objects
    currents.clear();
    currents.resize(numberOfSamples);
    voltages.clear();
    voltages.resize(numberOfSamples);
    times.clear();
    times.resize(numberOfSamples);


    // Create current input
    int sample;
    double rnd;
    srand((unsigned) time(NULL));
    currents[0] = 0.0;  // To be able to neglect the first high jitter in the sampling times
    for (sample=1; sample<numberOfSamples-fullKerLen; sample++){
        rnd = 2*(((double) rand())/RAND_MAX-0.5);     // Random number in [-1, 1]
        currents[sample]= injAmp*rnd+hyperpolCurr;    // Generate a random current value
    }
    for (sample=numberOfSamples-fullKerLen-1; sample<numberOfSamples; sample++)
        currents[sample]= 0.0+hyperpolCurr; // Zero current at the last bit of the input (for fast kernel calculation)

    //------------------- Injection & Recording ----------------//
    // Obtain current-voltage pairs
    InjectAndRecord(numberOfSamples, true);
    //----------------------------------------------------------//

    // Get rid of the first elements (high jitter in sampling time)
    for ( sample=0; sample<numberOfSamples-1; sample++ )
    {
        currents[sample] = currents[sample+1];
        voltages[sample] = voltages[sample+1];
        times[sample] = times[sample+1];
    }
    numberOfSamples -= 1;
    currents.resize(numberOfSamples);
    voltages.resize(numberOfSamples);
    times.resize(numberOfSamples);

    // Subtract hyperpolarizing current used
    for ( sample=0; sample<currents.size(); sample++ )
        currents[sample] -= hyperpolCurr;

    // Recalculate sampling rate based on the actual sampling times
    double actSampRate = 0.0;
    for ( sample=0; sample<times.size()-1; sample++ )
        if ( times[sample+1] != times[sample] )
            actSampRate += 1.0/(times[sample+1]-times[sample]);
    actSampRate /= times.size()-1;

    kerCalc->SetParams(fullKerLen, elecKerLen, actSampRate, currents, voltages);

    kerCalc->CalculateKernel();

    calMembRes = kerCalc->GetRM();
    calMembTau = kerCalc->GetTauM();
    calElecRes = kerCalc->GetRE();
    calElecTau = kerCalc->GetTauE();

    //-----------------Only for testing---------------//
  #ifdef TEST_VERSION
    fname = QString("full_kernel_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, kerCalc->fullKernel);
    fname = QString("currents_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, currents);
    fname = QString("voltages_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, voltages);
    fname = QString("times_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, times);
  #endif
    //-----------------Only for testing---------------//

    // Init AECChannel with the kernel
    DCT->aecChannels[elecNum]->Initialize(inputChannelNumber, outputChannelNumber, 1.0/actSampRate, kerCalc->GetElecKernel());

    // Save the kernel for testing
    QString fname = QString("kernel_")+QString::number(elecNum)+QString(".dat");
    saveData(fname, kerCalc->GetElecKernel());

}


/*
// Completes the sampling cycle by waiting till the end of the sampling period
double Calibrator::WaitTillNextSampling(double time)
{
    double t = 0.0;
    // it appears that in (at least) the release version, this function is
    // churning so fast that it gets confused about the actual time that
    // has elapsed. In debug mode it seems slow enough not to encounter this.
    // (alternatively it could also be a question of unknown optimisations in
    // ther release version causing this).
    // As a fix, we slow down the churning by doing some (useless) math.
    // 20 iterations appeared to be the sweet spot where the repeats were slow enough
    // to get maeningful readings from the windows high precision counter and yet not be too coarse
    // with the precision of achieved update time steps.
    // (TN 2015-08-06)
#ifdef QT_NO_DEBUG
    double y= 2.0;
    double z= 0.0;
#endif
    do
    {
        t += board->get_RTC();
#ifdef QT_NO_DEBUG
        for (int i= 0; i < 20; i++) {
            z= sqrt(y);
            y= z*z;
        }
#endif
    } while (t < time);

    return t;
}
*/


// Injects the current and records the voltage
// for 'numOfSamples' samples
void Calibrator::InjectAndRecord(int numOfSamples, bool isCalibration)
{
    int sample;
    double t;

    // Error handling
    if ( currents.size() < numOfSamples ) currents.resize(numOfSamples);
    if ( voltages.size() < numOfSamples ) voltages.resize(numOfSamples);
    if ( times.size()    < numOfSamples ) times.resize(numOfSamples);

    t = 0.0; // init time

    // Create tolerance limits
    double higherLimit = inChn->p->minVoltage + 0.9*(inChn->p->maxVoltage-inChn->p->minVoltage);
    double lowerLimit =  inChn->p->minVoltage + 0.1*(inChn->p->maxVoltage-inChn->p->minVoltage);
    bool limitWarningEmitted = false;

    // board->reset_board();
    DAQClock.reset_RTC();
    inBoard->start();
    outBoard->start();;

    if( isCalibration == true ) // read -- wait -- write order in case of calibration
        for ( sample=1; sample<numOfSamples; sample++ )
        {

            // --- Read --- //
            inBoard->get_scan();
            voltages[sample]= inChn->V; // save voltage
            if ( voltages[sample] > higherLimit || voltages[sample] < lowerLimit ) // check channel limits
                if ( limitWarningEmitted == false){
                      emit CloseToLimit(QString("Voltage"), inputChannelNumber.prettyName(), inChn->p->minVoltage, inChn->p->maxVoltage, voltages[sample]);
                      limitWarningEmitted = true;
                }
            // --- Read end --- //


            // --- Wait --- //
            t += DAQClock.wait_till_elapsed(samplingPeriod);
            times[sample] = t; // save the time stamp
            // --- Wait end --- //


            // --- Write --- //
            outChn->I= currents[sample];
            outBoard->write_analog_out();
            // --- Write end --- //

        }

    else    // write -- wait -- read order in case of electrode and membrane measurement
        for ( sample=1; sample<numOfSamples; sample++ )
        {

            // --- Write --- //
            outChn->I= currents[sample];
            outBoard->write_analog_out();
            // --- Write end --- //


            // --- Wait --- //
            t += DAQClock.wait_till_elapsed(samplingPeriod);
            times[sample] = t; // save the time stamp
            // --- Wait end --- //


            // --- Read --- //
            inBoard->get_scan();
            voltages[sample]= inChn->V; // save voltage
            if ( voltages[sample] > higherLimit || voltages[sample] < lowerLimit ) // check channel limits
                if ( limitWarningEmitted == false){
                      emit CloseToLimit(QString("Voltage"), inputChannelNumber.prettyName(), inChn->p->minVoltage, inChn->p->maxVoltage, voltages[sample]);
                      limitWarningEmitted = true;
                }
            // --- Read end --- //

        }

    // Shift the time vector to zero beginning to get rid of the initial sampling jitter
    if ( times[1] > samplingPeriod )
    {
        double shift = times[1] - samplingPeriod;
        for ( sample=1; sample<numOfSamples; sample++ )
            times[sample] -= shift;
    }

    // Reset board
    for ( DAQ *b : Devices.actdev )
        b->reset_board();
}


// Measures and returns the voltage offset
double Calibrator::VoltageOffsetMeasurement()
{
    int sample;
    double vOffset = 0.0;
    double offsetMeasLen = 5.0; // in msec
    int offsetSamplingNum = (int) (offsetMeasLen*1e-3/samplingPeriod);  // how many sampling steps

    DAQClock.reset_RTC();
    inBoard->start();

    // Measure the voltage offset
    for (sample=0; sample<offsetSamplingNum; sample++)
    {
        // Channel read
        inBoard->get_scan();

        // Save voltage
        vOffset += inChn->V;

        DAQClock.wait_till_elapsed(samplingPeriod);
    }
    vOffset /= offsetSamplingNum;

    inBoard->reset_board();

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

    // Std / Average rate ratio
    stdPerAverRate= stdSampRate / averSampRate;

    // Minimal rate (Hz)
    minSampRate = 1.0/samplingPeriod;
    for ( i=0; i<rates.size(); i++ ) if ( rates[i] < minSampRate ) minSampRate = rates[i];

    // Minimal / Desired rate ratio
    desPerMinRate = 1.0 / samplingPeriod / minSampRate;

    // Maximal sampling time (msec)
    maxSampTime = 1.0 / minSampRate;

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
  ofstream os(fname.toLatin1());

  for (int i= 0; i < data.size(); i++) {
    os << data[i] << "\n";
  }

  os.close();
}

