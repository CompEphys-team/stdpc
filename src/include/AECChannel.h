#ifndef AECCHANNEL_H
#define AECCHANNEL_H

#include <QVector>
#include "ChannelIndex.h"

class AECChannel
{
public:
    ChannelIndex inChnNum;
    ChannelIndex outChnNum;

    bool active;

    double kerSampPer;
    double kernelTime;
    double v_e;
    int buffInd;

    QVector<double> kernel;
    QVector<double> iBuffer;
    QVector<double> tBuffer;    

    AECChannel();

    void Activate();
    void Inactivate();
    bool IsActive();

    void Initialize(ChannelIndex inChnNum, ChannelIndex outChnNum, double kerSampPer, QVector<double> kernel);
    void ResetChannel();
    double CalculateVe(double current, double dt);

};

#endif // AECCHANNEL_H
