#include "SpkGen.h"
#include "DCThread.h"


SpkGen::SpkGen(ModelPrototype *parent, int instID, DCThread *DCT) :
    Model(parent, instID, DCT),
    p(static_cast<const SGData *>(&(parent->params()))),
    instp(static_cast<const SgInstData *>(&params())),
    V(p->VRest),
    bdChn(DCT->getInChan(instp->bdChannel)),
    SGactive(p->bdType == 0),
    burstDetected(false),
    tOverThresh(0.),
    tUnderThresh(0.),
    onThreshold(false),
    burstNo(0),
    ISI_time(0.),
    period(p->period),
    initial(true),
    active(true)
{
    in.V = V;
    out.save = out.active = false;
    if (p->LUTables) {
        theExp= &expLU;
        expLU.require(-50.0, 50.0, 0.02);
        theTanh= &tanhLU;
        tanhLU.require(-50.0, 50.0, 0.02);
    } else {
        theExp= &expFunc;
        theTanh= &tanhFunc;
    }
}

void SpkGen::updateIn(double t)
{
    in.V = V;
    in.process(t);
}

void SpkGen::update(double t, double dt)
{
    if ( !active )
        return;

    if (initial) {
        ISI_time = t;
        initial = false;
    } else {
        ISI_time += dt;
    }

    if (p->bdType > 0 && !SGactive && bdChn ) { // taking care of the burst detection options
                                      // burst detection hasn't been completed yet ... spike generation inactive
        if (burstDetected) { // burst (first crossing) was detected, waiting for opposite crossing
            if (((p->bdType == 1) && (bdChn->V > instp->bdThresh)) ||
                ((p->bdType == 2) && (bdChn->V < instp->bdThresh))) {
                tUnderThresh += dt;     // "under" and "over" only phrases for first and second cond.
                if (tUnderThresh > p->bdtUnder) {
                    SGactive = true;
                    tOverThresh = 0.;
                    burstDetected = false;
                    ISI_time = 0.0;
                    period = p->SpikeT[burstNo].back() + (20.0/p->spkTimeScaling);
                }
            } else if ( p->bdtUnderCont ) {
                if ( !onThreshold &&
                    (((p->bdType == 1) && (bdChn->V == instp->bdThresh)) ||
                     ((p->bdType == 2) && (bdChn->V == instp->bdThresh)))) {
                    onThreshold = true;
                } else {
                    onThreshold = false;
                    tUnderThresh = 0.;
                    if ( p->bdStrictlyCont ) {
                        tOverThresh = 0.;
                        burstDetected = false;
                    }
                }
            }
        } else {  // burst (first crossing) not yet detected ...
            if (((p->bdType == 1) && (bdChn->V < instp->bdThresh)) ||
                ((p->bdType == 2) && (bdChn->V > instp->bdThresh))) {
                tOverThresh += dt;
                if (tOverThresh > p->bdtOver) {
                    tUnderThresh = 0.;
                    burstDetected = true;
                }
            } else if ( p->bdtOverCont ) {
                if ( !onThreshold &&
                    (((p->bdType == 1) && (bdChn->V == instp->bdThresh)) ||
                     ((p->bdType == 2) && (bdChn->V == instp->bdThresh)))) {
                    onThreshold = true;
                } else {
                    onThreshold = false;
                    tOverThresh = 0.;
                }
            }
        }
    }

    // calculating SpkGen voltage for all cases:
    if (SGactive) {// the spike generation is active
        // routine that calculates the membrane potential of the CN taking
        // in account all the spikes (superimposing) the voltage due to each spike
        V = 0.0;
        for ( double const& tSpike : p->SpikeT[burstNo] )
            V += VSpike((ISI_time - tSpike) * p->spkTimeScaling);
        V *= p->VSpike;
        V += p->VRest;

        if (ISI_time > period) {
            if (p->bdType > 0) {
                SGactive = false;
            } else {
                ISI_time -= (period = p->period);
            }
            if ( ++burstNo == (int)p->SpikeT.size() ) {
                if ( p->loopBursts ) {
                    burstNo = 0;
                } else {
                    message(QString("Spike time data exhausted ... stopping spike Generator"));
                    V = p->VRest;
                    active = false;
                }
            }
        }
    } else {
        V = p->VRest;
    }
    in.V = V;
}

// how to generate a spike
double SpkGen::VSpike(double t){
  #define A 1.0
  #define B 4.0
  #define TAU1 4.0
  #define TAU2 4.0
  #define T0 -0.576
  #define A1 2.0
  #define A2 2.0
  #define NORMALIZE 3.25394

  double f1, f2, f_spike;

  if ((t-T0)/TAU1 < 100.0)
    f1= (((*theTanh)(A1*(T0-t))+1.0)/2.0)*(*theExp)((t-T0)/TAU1);
  else f1= 0.0;

  if ((T0-t)/TAU2 < 100.0)
    f2= (((*theTanh)(A2*(t-T0))+1.0)/2.0)*(*theExp)((T0-t)/TAU2);
  else f2= 0.0;

  f_spike= (A*f1 + B*f2)/NORMALIZE;
  return(f_spike);

  #undef A
  #undef B
  #undef TAU1
  #undef TAU2
  #undef T0
  #undef A1
  #undef A2
  #undef NORMALIZE
}


void SpkGenPrototype::init(DCThread *DCT)
{
    inst.reserve(params().numInst());
    for ( size_t i = 0; i < params().numInst(); i++ )
        if ( params().instance(i).active )
            inst.emplace_back(new SpkGen(this, i, DCT));
}
