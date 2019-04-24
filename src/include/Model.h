#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include <QPair>
#include <QVector>
#include <QString>
#include <QObject>
#include "Channels.h"

class ModelProxy;
class ModelPrototype;

class Model : public QObject
{
    Q_OBJECT
public:
    Model(ModelPrototype *parent, size_t instID, DCThread *DCT);
    virtual ~Model() {}

    /// Process channels:
    /// Applies bias to out.I and calls in.process(t).
    virtual inline void updateIn(double t) { in.process(t); }
    virtual inline void updateOut(double /*t*/) { out.I = params().outChn.bias; }

    /// Retain out.I (which includes bias + any a2d contributions) prior to entering RK4 iterations
    virtual inline void retainCurrent(double /*t*/) { retainedI = out.I; }

    /// Reset out.I to the retained value at the start of each new RK4 iteration (note: full iteration, not step)
    virtual inline void restoreCurrent(double /*t*/) { out.I = retainedI; }

    /// Compute Runke-Kutta step n (n = 0..3), in this order:
    ///  - Compute k_n based on the previous intermediate state (= null state, when n=0)
    ///  - Provide output to channels based on the previous intermediate state
    ///  - Update intermediate state based on k_n and null state
    ///  - at n=3, update null state based on k_{0..3} (nullstate += (k0 + 2k1 + 2k2 + k3)/6 * dt)
    /// Note, out.I must be reset to the retained value at the end of every step, except for the last one (n=3).
    virtual void RK4(double t, double dt, size_t n, bool settling) = 0;

    vInstData &params() const;
    inline size_t id() const { return instID; }

    inChannel in;
    outChannel out;

signals:
    void message(const QString &);

protected:
    const ModelPrototype *parent;
    const size_t instID;

    double retainedI;
};


class ModelPrototype
{
public:
    ModelPrototype(size_t modelID) : modelID(modelID) {}
    virtual ~ModelPrototype() {}

    /// Prepare for action: Populate inst with newly constructed Models
    virtual void init(DCThread *) = 0;

    /// Post-init call for any additional setup that requires other models to be present
    virtual void post_init(DCThread *) {}

    /// Return a reference to the specific parameter set used for this model
    virtual ModelData &params() const = 0;

    /// Return this model's registered proxy (ideally, point to a static local instance)
    virtual ModelProxy *proxy() const = 0;

    /// Return a (unique) prefix to identify this model class
    virtual QString prefix() const = 0;

    /// Clears out inst
    virtual void reset();

    /// Processes associated channels via Model::updateIn(t) and Model::updateOut(t)
    virtual void updateChannels(double t);

    /// Delegates to Model::retainCurrent(t)
    virtual void retainCurrent(double t);

    /// Delegates to Model::restoreCurrent(t)
    virtual void restoreCurrent(double t);

    /// Delegates to Model::RK4(t,dt,n)
    virtual void RK4(double t, double dt, size_t n, bool settling);

    QString getStatus() const;
    QPair<QVector<ChannelIndex>, QVector<const double *>> valuesToSave()const;

    inline std::vector<std::shared_ptr<Model>> const& instances() const { return inst; }

protected:
    size_t modelID;
    std::vector<std::shared_ptr<Model>> inst;

    friend class ModelManager;
};

#endif // MODEL_H
