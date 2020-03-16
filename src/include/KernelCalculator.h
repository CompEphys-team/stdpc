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

#ifndef KERNELCALCULATOR_H
#define KERNELCALCULATOR_H

#include <QVector>

class KernelCalculator
{

public:

    double tFac; // to scale up the timevector in order to achieve better precision

    // Membrane and electrode parameters (tau time constant and R resistance)
    double tau_m;
    double tau_e;
    double R_m;
    double R_e;

    double samplingRate;

    QVector<double> iVec;
    QVector<double> vVec;
    QVector<double> fullKernel;
    QVector<double> elecKernel;

public:

    KernelCalculator();
    void SetParams(int, int, double, QVector<double>, QVector<double>);
    void CalculateKernel();
    void CalcFullKernel();
    QVector<double> LevonsinDurbin(QVector<double>, QVector<double>);
    void CalcElecKernel();
    QVector<double> ExpFit(QVector<double>, QVector<double>);
    void OptimizeKe();
    double RemoveKm(double);

    QVector<double> GetFullKernel();
    void SetFullKernel(QVector<double>);
    QVector<double> GetElecKernel();
    void SetElecKernel(QVector<double>);
    double GetTauM();
    double GetRM();
    double GetTauE();
    double GetRE();

};

#endif // KERNELCALCULATOR_H

