#ifndef INCLUDESYNAPTICNOISE_H
#define INCLUDESYNAPTICNOISE_H

#include "IonicCurrent.h"

/// Note: SynapticNoise follows the formalism laid out in:
///  Destexhe, A, M Rudolph, J. -M Fellous, and T. J Sejnowski.
///  ‘Fluctuating Synaptic Conductances Recreate in Vivo-like Activity in Neocortical Neurons’.
///  Neuroscience 107, no. 1 (9 November 2001): 13–24. doi:10.1016/S0306-4522(01)00344-X.
/// However, any one SynapticNoise instance only covers one synapse type (excitatory or inhibitory), not both.

struct SynapticNoiseData : public CurrentData
{
    double Vrev; //!< Reversal potential
    double tau; //!< Time constant
    double g0; //!< Average conductance
    double D; //!< Noise diffusion coefficient
    bool ignoreNegative;
};

class SynapticNoiseProxy : public IonicCurrentProxy
{
public:
    SynapticNoiseProxy();
    inline SynapticNoiseData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "SynapticNoise"; }
    inline QString prettyName() { return "Syn. noise"; }

    IonicCurrent *createAssigned(size_t conductanceID, size_t assignID, size_t multiID,
                                 inChannel *in, outChannel *out);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<SynapticNoiseData> p;
};

class SynapticNoise : public IonicCurrent
{
public:
    SynapticNoise(size_t condID, size_t assignID, size_t multiID, inChannel *in, outChannel *out);

    void step(double t, double dt, bool settling);
    void RK4(double t, double dt, size_t n, bool settling);
    
    inline const SynapticNoiseData &params() const { return SynapticNoiseProxy::p[condID]; }
    SynapticNoiseProxy *proxy() const;

protected:
    const SynapticNoiseData *p;
    const CurrentAssignment *a;
    double gNext;
    
    double next(double dt);
};

#endif // INCLUDESYNAPTICNOISE_H
