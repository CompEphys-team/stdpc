#ifndef AECCHANNEL_H
#define AECCHANNEL_H

#include <QVector>

class AECChannel
{
public:
    short int inChnNum;
    short int outChnNum;

    long int freq;
    bool active;
    int kernelSize;
    int tbufferSize;
    double Ve;

    QVector<double> Kernel;
    QVector<double> Ibuffer;
    QVector<double> Tbuffer;

    AECChannel();
    AECChannel(short int inChnNum, short int outChnNum);

    void initializeKernel(QVector<double> *kernel);
    void resetChannel();
    double calcVe(double current);
};

#endif // AECCHANNEL_H
