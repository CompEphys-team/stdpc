/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QVector>
#include "ObjectDataTypes.h"
#include "DCThread.h"
#include "KernelCalculator.h"
#include "ChannelIndex.h"
#include <QObject>

#ifndef CALIBRATOR_H
#define CALIBRATOR_H


class Calibrator: public QObject
{
    Q_OBJECT

private:

    QString fname; // for testing results    

    // Referenced objects
    DAQ *inBoard, *outBoard;
    KernelCalculator *kerCalc;

    // Channel related objects
    inChannel *inChn;
    outChannel *outChn;
    ChannelIndex inputChannelNumber;
    ChannelIndex outputChannelNumber;

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
    void InjectAndRecord(int, bool);
    double VoltageOffsetMeasurement();

    // Test and checker functions
    void saveData(QString &fname, QVector<double> data);

public:

    int incorrectMeasurement; // for error checking

    // Kernel params
    double fullKernelTauMRatio;  // The length of the full kernel is how many times the membrane time constant
    double elecKernelTauERatio;  // The length of the electrode kernel is how many times the electrode time constant
    double maxElecKFullKRation;  // To avoid to too high (electrode kernel length) / (full kernel length) ratios

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
    int  ChannelInit(ChannelIndex, ChannelIndex);
    void ElectrodeMeasurement(double, double, int, double, double);
    void MembraneMeasurement(double, double, int, double);
    void Calibration(double, double, int, int, double, double);
    void CalcSamplingStats();
    void SetChannelActivation(bool);

signals:
    void CloseToLimit(QString, QString, double, double, double);

};

#endif // CALIBRATOR_H

