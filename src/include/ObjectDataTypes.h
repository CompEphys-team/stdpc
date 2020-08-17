/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
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

#ifndef OBJECTDATATYPES_H
#define OBJECTDATATYPES_H

#include <QString>
#include <iostream>
#include <QColor>
#include "ChannelIndex.h"
#include "Util.h"

using namespace std;

struct AssignmentData {
    bool active;
    bool save = false;
};

struct SynapseAssignment : public AssignmentData {
    ChannelIndex PreSynChannel;
    ChannelIndex PostSynChannel;
    ChannelIndex OutSynChannel;
    double delay;
};

struct GapJunctionAssignment : public AssignmentData {
    ChannelIndex preInChannel;
    ChannelIndex postInChannel;
    ChannelIndex preOutChannel;
    ChannelIndex postOutChannel;
};

struct CurrentAssignment : public AssignmentData {
    ChannelIndex VChannel;
    ChannelIndex IChannel;
};

struct ConductanceData {
    bool active;
    bool activeSettling = false;
    QuotedString label;
    virtual const AssignmentData &assignment(size_t i) const = 0;
    virtual size_t numAssignments() const = 0;
    ConductanceData() : active(false) {}
    virtual ~ConductanceData() = default;
};

struct SynapseData : public ConductanceData {
    inline const SynapseAssignment &assignment(size_t i) const { return assign[i]; }
    inline size_t numAssignments() const { return assign.size(); }
    std::vector<SynapseAssignment> assign;
    int legacy_PreSyn = -1;
    int legacy_PostSyn = -1;
    int legacy_OutSyn = -1;
};

struct CurrentData : public ConductanceData {
    inline const CurrentAssignment &assignment(size_t i) const { return assign[i]; }
    inline size_t numAssignments() const { return assign.size(); }
    std::vector<CurrentAssignment> assign;
    int legacy_V = -1;
    int legacy_I = -1;
};

struct STPlast {
  double AMinus;
  double tauMinus;
  double APlus;
  double tauPlus;
  int Exponent;
  double Shift;
  int History;
  bool Table;
  double tableDt;
  double tableDgMin;
  double gMax;
  double gMid;
  double gSlope;
  bool sigmoidTable;
  double sigmoidTableDg;
  double sigmoidTableMaxEntry;
};

struct ODEPlast {
  double InitialP;
  double InitialD;
  double betaP;
  double betaD;
  double gamma;
  int eta;
  double highP;
  double lowP;
  double highD;
  double lowD;
  double gMax;
  double gMid;
  double gSlope;
  bool sigmoidTable;
  double sigmoidTableDg;
  double sigmoidTableMaxEntry;
};

struct elecCalibParams {
  // channel copy parameters
  bool copyChnOn;
  ChannelIndex copyChn;

  // General params
  double samplingRate;
  ChannelIndex outputChannelNumber;

  // Electrode measurement params
  double iMaxElec;
  double iMinElec;
  int numberOfLevels;
  double injLenPerLevel;

  // Membrane measurement params
  double iMembStep;
  int numberOfRepeats;
  double injLenPerRepeat;

  // Calibration params
  double hyperpolCurr;
  double injCalAmp;
  double injCalLen;
  double fullKernelLen;
  double electrodeKernelLen;

  int legacy_in = -1;
  int legacy_out = -1;
};

struct dataSavingParams {
  bool enabled;
  QuotedString fileName = "data_%n.dat";
  double savingFreq = 20000;
  bool isBinary = false;

  bool binaryLittleEndian = true;
  bool binaryDoublePrecision = true;

  QuotedString asciiSeparator = "\t";
  QuotedString asciiHeaderPrefix = "% ";
  bool asciiCRLF = true;

};
 
struct inChnData {
  bool active;
  int gain;
  double gainFac = 1.0;
  bool spkDetect;
  double spkDetectThresh = -0.01;
  double bias;
  double minVoltage;
  double maxVoltage;
  bool chnlSaving;
  elecCalibParams calib;
};

struct outChnData {
  bool active;
  int gain;
  double gainFac = 1.0;
  double bias;
  double minCurrent;
  double maxCurrent;
  bool chnlSaving;
};

class DAQData {
public:
    bool active;
    QuotedString label;
    std::vector<inChnData> inChn;
    std::vector<outChnData> outChn;
    DAQData() : active(false) {}
};

struct GraphData {
    bool active;
    ChannelIndex chan;
    bool isVoltage;
    int unitMod = 1;
    QColor color;

    GraphData() : active(true) {}
};

struct PlotData {
    std::vector<GraphData> graph;
    int height = 400;
    double yLower = -100;
    double yUpper = 100;
};

struct BasePlotData {
    bool active;
    double interval;
    int bufferExp;
    double xRange = 10;
    std::vector<PlotData> plot;
    BasePlotData() : interval(1e-3), bufferExp(4) {}
};

struct PerformanceMonitorData {
    bool active;
    double interval;
    PerformanceMonitorData() : active(false), interval(1) {}
};

struct SampleHoldData {
  bool active;
  double threshV;
  ChannelIndex trigChn;
};

struct vInstData {
    bool active;
    inChnData inChn;
    outChnData outChn;
};

struct ModelData {
    bool active;
    QuotedString label;
    virtual vInstData &instance(size_t i) = 0;
    virtual size_t numInst() const = 0;
    ModelData() : active(false) {}
    virtual ~ModelData() = default;
};

struct TriggerData {
    bool active;
    ChannelIndex channel;
};

struct SettlingData {
    bool active = false;
    double duration = 0.0;
};

struct GuiData {
    bool openDAQ = true;
    bool openTools = false;
    bool openSynapses = false;
    bool openCurrents = false;
};

#endif
