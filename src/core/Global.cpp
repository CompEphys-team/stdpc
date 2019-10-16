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

#include "Global.h"
#include "DeviceManager.h"
#include "ModelManager.h"
#include "ConductanceManager.h"

int LOADED_PROTOCOL_VERSION;
QString LEGACY_DAQ_CLASS;

BasePlotData Plotp;
PerformanceMonitorData PerfMonp;

dataSavingParams dataSavingPs;

// Attilas Sample-and-Hold
SampleHoldData SampleHoldp;

TriggerData Triggerp;
SettlingData Settlingp;
int ClampThreadPriority = 1;
GuiData GUIp;

// LU tables
tanhLUtable tanhLU;
expLUtable expLU;
expSigmoidLUtable expSigmoidLU;

Clock DAQClock;
DeviceManager Devices;
ModelManager Models;
ConductanceManager Conductances;

randutils::mt19937_rng RNG;
