#include <QVector>
#include "ObjectDataTypes.h"
#include "DCThread.h"
#include "KernelCalculator.h"
#include <QObject>

#ifndef CALIBRATOR_H
#define CALIBRATOR_H


class Calibrator: public QObject
{
    Q_OBJECT

private:

    QString fname; // for testing results    

    // Referenced objects
    DAQ *board;
    KernelCalculator *kerCalc;

    // Channel related objects
    inChannel *inChn;
    outChannel *outChn;
    short int inputChannelNumber;
    short int outputChannelNumber;

    // General params
    double samplingPeriod;
    long int numberOfSamples;

    // Kernel params
    int fullKerLen;
    int elecKerLen;           

    // Data storing objects
    QVector<double> currents;
    QVector<double> voltages;
    QVector<double> times;    

    // Inner functions
    bool InitChannels();   
    double WaitTillNextSampling(double);
    void InjectAndRecord(int);
    double VoltageOffsetMeasurement();

    // Test and checker functions
    void saveData(QString &fname, QVector<double> data);

public:

    DCThread *DCT;

    int incorrectMeasurement; // for error checking

    // Kernel params
    double fullKernelTauMRatio;  // The length of the full kernel is how many times the membrane time constant
    double elecKernelTauERatio;  // The length of the electrode kernel is how many times the electrode time constant

    // Measured electrode params
    double elecRes;
    double elecResStd;
    double elecTau;
    double elecTauStd;

    // Measured membrane params
    double membRes;
    double membResStd;
    double membTau;
    double membTauStd;

    // Calculated electrode and membrane params
    double calElecRes;
    double calElecTau;
    double calMembRes;
    double calMembTau;    

    // Timing stats
    double averSampRate;
    double minSampRate;    
    double maxSampTime;
    double stdSampRate;
    double desPerMinRate;
    double stdPerAverRate;

    Calibrator();
    void GeneralInit(DAQ*, DCThread*);
    int  ChannelInit(int, int);
    void ElectrodeMeasurement(double, double, int, double, double);
    void MembraneMeasurement(double, double, int, double);
    void Calibration(double, double, int, int, int, double, double);
    void CalcSamplingStats();
    void SetChannelActivation(int, bool);

signals:
    void CloseToLimit(QString, int, double, double, double);

};

#endif // CALIBRATOR_H

