#ifndef KERNELCALCULATOR_H
#define KERNELCALCULATOR_H

#include <QVector>

class KernelCalculator
{

public:

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

