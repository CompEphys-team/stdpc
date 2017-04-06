#include "Daq.h"
#include "DeviceManager.h"

DAQ::DAQ(size_t devID, DAQProxy *proxy) :
    devID(devID),
    proxy(proxy),
    t(DAQClock.t)
{
  initialized= false;
}


DAQ::~DAQ()
{
}

DAQData *DAQ::params()
{
     return &proxy->param(devID);
}


//---------------------------------------------------------------------------
void DAQ::init_chans()
{
    DAQData *p = params();
    short int No = 0;
    short int Chns[p->inChn.size()];
    in.resize(p->inChn.size());
    vHiLim.resize(p->inChn.size());
    vLoLim.resize(p->inChn.size());
    inChnLabels.resize(p->inChn.size());
    for ( size_t i = 0; i < p->inChn.size(); i++ ) {
        if ( p->inChn[i].active ) {
            in[i].init(&p->inChn[i]);
            Chns[No++] = i;
            vHiLim[i] = p->inChn[i].minVoltage + upTolFac*(p->inChn[i].maxVoltage - p->inChn[i].minVoltage);
            vLoLim[i] = p->inChn[i].minVoltage + loTolFac*(p->inChn[i].maxVoltage - p->inChn[i].minVoltage);
        } else {
            in[i].V = 0.0;
            in[i].active = false;
        }
    }
    generate_scan_list(No, Chns);

    No = 0;
    out.resize(p->outChn.size());
    iHiLim.resize(p->outChn.size());
    iLoLim.resize(p->outChn.size());
    outChnLabels.resize(p->outChn.size());
    for ( size_t i = 0; i < p->outChn.size(); i++ ) {
        if ( p->outChn[i].active ) {
            out[i].init(&p->outChn[i]);
            Chns[No++] = i;
            iHiLim[i] = p->outChn[i].minCurrent+ upTolFac*(p->outChn[i].maxCurrent- p->outChn[i].minCurrent);
            iLoLim[i] = p->outChn[i].minCurrent + loTolFac*(p->outChn[i].maxCurrent- p->outChn[i].minCurrent);
        } else {
            out[i].I = 0.0;
            out[i].active = false;
        }
    }
    generate_analog_out_list(No, Chns);
}


//---------------------------------------------------------------------------
void DAQ::reset_chans()
{
    for ( outChannel &o : out )
        o.I = 0.0;
}


//---------------------------------------------------------------------------
void DAQ::process_scan(double t)
{
    DAQData *p = params();
    for ( int i = 0; i < actOutChnNo; i++ )
        out[outIdx[i]].I = p->outChn[outIdx[i]].bias;
    for ( int i = 0; i < actInChnNo; i++ ) {
        in[inIdx[i]].V += p->inChn[inIdx[i]].bias;
        in[inIdx[i]].process(t);
    }
}

//---------------------------------------------------------------------------
QPair<QVector<QString>, QVector<const double *>> DAQ::valuesToSave()
{
    QVector<QString> labels;
    QVector<const double *> values;
    for ( int i = 0; i < actInChnNo; i++ ) {
        if ( in[inIdx[i]].save ) {
            labels.push_back(QString("%1_%2_V%3").arg(proxy->daqClass()).arg(devID).arg(inIdx[i]));
            values.push_back(&in[inIdx[i]].V);
        }
    }
    for ( int i = 0; i < actOutChnNo; i++ ) {
        if ( out[outIdx[i]].save ) {
            labels.push_back(QString("%1_%2_I%3").arg(proxy->daqClass()).arg(devID).arg(outIdx[i]));
            values.push_back(&out[outIdx[i]].I);
        }
    }
    return qMakePair(labels, values);
}

//---------------------------------------------------------------------------
QVector<AECChannel*> DAQ::aecChans()
{
    QVector<AECChannel*> chans;
    for ( int i = 0; i < actInChnNo; i++ ) {
        if ( in[inIdx[i]].aec.IsActive() ) {
            chans.push_back(&in[inIdx[i]].aec);
        }
    }
    return chans;
}


//---------------------------------------------------------------------------
bool DAQ::check_limits(bool checkV_and_warn, ChannelLimitWarning &w)
{
    bool warn = false;
    for ( int i = 0; i < actOutChnNo; i++ ) {
        if( out[outIdx[i]].I > iHiLim[outIdx[i]] ) {
            if ( checkV_and_warn ) {
                w = {"Current", outChnLabels[outIdx[i]], iHiLim[outIdx[i]], iLoLim[outIdx[i]], out[outIdx[i]].I};
                warn = true;
            }
            out[outIdx[i]].I = iHiLim[outIdx[i]];
        } else if ( out[outIdx[i]].I < iLoLim[outIdx[i]] ) {
            if ( checkV_and_warn ) {
                w = {"Current", outChnLabels[outIdx[i]], iHiLim[outIdx[i]], iLoLim[outIdx[i]], out[outIdx[i]].I};
                warn = true;
            }
            out[outIdx[i]].I = iLoLim[outIdx[i]];
        }
    }
    if ( checkV_and_warn ) for ( int i = 0; i < actInChnNo && !warn; i++ ) {
        if (in[inIdx[i]].V > vHiLim[inIdx[i]] ) {
            w = {"Voltage", inChnLabels[inIdx[i]], vHiLim[inIdx[i]], vLoLim[inIdx[i]], in[inIdx[i]].V};
            warn = true;
        } else if (in[inIdx[i]].V < vLoLim[inIdx[i]] ) {
            w = {"Voltage", inChnLabels[inIdx[i]], vHiLim[inIdx[i]], vLoLim[inIdx[i]], in[inIdx[i]].V};
            warn = true;
        }
    }
    return warn;
}
