#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include <QPair>
#include <QVector>
#include <QString>
#include <QObject>
#include "Channels.h"

class ModelPrototype;

class Model : public QObject
{
    Q_OBJECT
public:
    Model(ModelPrototype *parent, int instID, DCThread *DCT);
    virtual ~Model() {}

    /// Process channels
    virtual void updateIn(double t) = 0;
    virtual void updateOut(double t) = 0;

    /// Integrate inputs, prepare outputs
    virtual void update(double t, double dt) = 0;

    vInstData &params() const;
    inline int id() const { return instID; }

    inChannel in;
    outChannel out;

signals:
    void message(const QString &);

protected:
    const ModelPrototype *parent;
    const int instID;
};


class ModelPrototype
{
public:
    ModelPrototype(int modelID) : modelID(modelID) {}
    virtual ~ModelPrototype() {}

    /// Prepare for action: Populate inst with newly constructed Models
    virtual void init(DCThread *) = 0;

    /// Return a reference to the specific parameter set used for this model
    virtual ModelData &params() const = 0;

    /// Return this model's ModelClass value
    virtual ModelClass modelClass() const = 0;

    /// Return a (unique) prefix to identify this model class
    virtual QString prefix() const = 0;

    /// Clears out inst
    virtual void reset();

    /// Processes associated channels via Model::updateIn(t) and Model::updateOut(t)
    virtual void updateChannels(double t);

    /// Integrates all inputs and prepares all outputs via Model::update(t,dt)
    virtual void updateInstances(double t, double dt);

    QString getStatus() const;
    QPair<QVector<QString>, QVector<inChannel*>> inChans_to_save() const;
    QPair<QVector<QString>, QVector<outChannel*>> outChans_to_save()const;

    inline std::vector<std::shared_ptr<Model>> const& instances() const { return inst; }

protected:
    int modelID;
    std::vector<std::shared_ptr<Model>> inst;

    friend class ModelManager;
};

#endif // MODEL_H
