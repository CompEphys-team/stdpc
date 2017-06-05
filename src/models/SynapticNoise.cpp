#include "SynapticNoise.h"
#include "SynapticNoiseDlg.h"
#include "AP.h"

static SynapticNoiseProxy prox;
std::vector<SynapticNoiseData> SynapticNoiseProxy::p;
SynapticNoiseProxy *SynapticNoise::proxy() const { return &prox; }
ConductanceDlg *SynapticNoiseProxy::createDialog(size_t condID, QWidget *parent) { return new SynapticNoiseDlg(condID, parent); }
IonicCurrent *SynapticNoiseProxy::createAssigned(size_t conductanceID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) {
    return new SynapticNoise(conductanceID, assignID, multiID, in, out);
}

SynapticNoiseProxy::SynapticNoiseProxy()
{
    ConductanceManager::RegisterCurrent(this);

    addAP("SynapticNoisep[#].active", &p, &SynapticNoiseData::active);
    addAP("SynapticNoisep[#].Vrev", &p, &SynapticNoiseData::Vrev);
    addAP("SynapticNoisep[#].tau", &p, &SynapticNoiseData::tau);
    addAP("SynapticNoisep[#].g0", &p, &SynapticNoiseData::g0);
    addAP("SynapticNoisep[#].D", &p, &SynapticNoiseData::D);
    addAP("SynapticNoisep[#].assign[#].active", &p, &SynapticNoiseData::assign, &CurrentAssignment::active);
    addAP("SynapticNoisep[#].assign[#].VChannel", &p, &SynapticNoiseData::assign, &CurrentAssignment::VChannel);
    addAP("SynapticNoisep[#].assign[#].IChannel", &p, &SynapticNoiseData::assign, &CurrentAssignment::IChannel);
    addAP("SynapticNoisep[#].assign[#].save", &p, &SynapticNoiseData::assign, &CurrentAssignment::save);
}

SynapticNoise::SynapticNoise(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out) :
    IonicCurrent(condID, assignID, multiID, in, out),
    p(&params()),
    a(&assignment())
{
    m_conductance = params().g0;
}

void SynapticNoise::step(double, double dt)
{
    m_conductance = next(dt);
    if ( !p->ignoreNegative || m_conductance > 0 )
        out->I += m_conductance * (p->Vrev - in->V);
}

void SynapticNoise::RK4(double, double dt, size_t n)
{
    if ( n == 0 ) // RK0: Estimate gradient at the step's starting point, m_conductance unchanged from previous
        gNext = next(2*dt); // 2*dt, because dt on RK step 0 is half the full step length
    else if ( n == 1 ) // RK1, RK2: Estimate gradient at (temporal) midpoint
        m_conductance = 0.5 * (m_conductance + gNext);
    else if ( n == 3 ) // RK3: Estimate gradient at endpoint
        m_conductance = gNext;

    if ( !p->ignoreNegative || m_conductance > 0 )
        out->I += m_conductance * (p->Vrev - in->V);
}

double SynapticNoise::next(double dt)
{
    double tmp = std::exp(-dt/p->tau);
    double A = std::sqrt(p->D * p->tau / 2 * (1 - tmp*tmp));
    return p->g0 + (m_conductance - p->g0) * tmp + A * RNG.variate<double>(0, 1);
}
