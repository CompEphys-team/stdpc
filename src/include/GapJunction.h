#ifndef GAPJUNCTION_H
#define GAPJUNCTION_H

#include "Global_func.h"
#include "Channels.h"
#include "ConductanceManager.h"

class GapJunction;

struct GJunctData : public ConductanceData {
    int type;
    double gSyn;
    inline const GapJunctionAssignment &assignment(size_t i) const { return assign[i]; }
    inline size_t numAssignments() const { return assign.size(); }
    std::vector<GapJunctionAssignment> assign;
    int legacy_PreIn = -1;
    int legacy_PostIn= -1;
    int legacy_PreOut = -1;
    int legacy_PostOut = -1;
};

class GapJunctionProxy : public ConductanceProxy
{
public:
    GapJunctionProxy();
    inline GJunctData &param(size_t i) const { return p[i]; }
    inline size_t size() { return p.size(); }
    inline void resize(size_t sz) { p.resize(sz); }
    inline void remove(size_t i) { p.erase(p.begin() + i); }

    inline QString conductanceClass() { return "ESyn"; }
    inline QString prettyName() { return "Gap Junction"; }

    void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                     std::vector<Conductance*> &preD,
                     std::vector<Conductance*> &inD,
                     std::vector<Conductance*> &postD);

    ConductanceDlg *createDialog(size_t condID, QWidget *parent=nullptr);

    static std::vector<GJunctData> p;
};

class GapJunction : public Conductance {
  private:
    const GJunctData *p;
    const GapJunctionAssignment *a;
    
  protected:
    double I;
    inChannel *pre;
    inChannel *post;
    outChannel *outpre;
    outChannel *outpost;
        
  public:
    GapJunction(size_t condID, size_t assignID, size_t multiID, inChannel *pre, outChannel *outpre, inChannel *post, outChannel *outpost);
    void step(double t, double dt, bool settling);
    inline void RK4(double, double, size_t, bool) {}

    inline const GapJunctionAssignment &assignment() const { return params().assignment(assignID); }
    inline const GJunctData &params() const { return GapJunctionProxy::p[condID]; }
    GapJunctionProxy *proxy() const;
};

#endif
