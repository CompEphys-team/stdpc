#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QDialog>
#include <memory>
#include "Model.h"
#include "ModelDlg.h"

class ModelProxy {
public:
    /// Parameter access and manipulation functions:
    /// Return a reference to the indicated parameter set
    virtual ModelData& param(size_t i) = 0;
    /// Return the number of parameter sets present
    virtual size_t size() = 0;
    /// Change the number of parameter sets
    virtual void resize(size_t) = 0;
    /// Remove a parameter set. Subsequent sets are expected to move up into the freed space.
    virtual void remove(size_t) = 0;

    /// Return an identifier for this model class (e.g. "HH"). Must not contain any whitespace or "/" characters.
    virtual QString modelClass() = 0;

    /// Return a human-readable identifier for this model class (e.g. "Hodgkin-Huxley neuron") for use in the GUI
    virtual inline QString prettyName() { return modelClass(); }

    /// Create a new model object
    virtual ModelPrototype *createPrototype(size_t modelID) = 0;

    /// Create a new model dialog
    virtual ModelDlg *createDialog(size_t modelID, QWidget *parent=nullptr) = 0;
};


class ModelManager : public QObject
{
    Q_OBJECT
public:
    ModelManager() {}
    ~ModelManager() {}

    /// Initialise all active models for end use
    void initActive(DCThread *);

    /// Remove all models and parameter sets e.g. in preparation for parameter import
    void clear();

    /// Returns false if at least one registered parameter set is active (active prototype with at least one active instance)
    bool empty() const;

    /// Create a model of the given type from proxy->param(idx)
    void initSingle(ModelProxy *proxy, size_t idx);

    inChannel *getInChan(ChannelIndex const&);
    outChannel *getOutChan(ChannelIndex const&);

    typedef QHash<QString, QVector<std::shared_ptr<ModelPrototype>>> map_type;

    inline QVector<std::shared_ptr<ModelPrototype>> const& active() const { return activeModels; }
    inline map_type const& all() const { return allModels; }

    static QMap<QString, ModelProxy*> &Register() { static QMap<QString, ModelProxy*> r; return r; }
    static inline void RegisterModel(QString modelName, ModelProxy *proxy) { Register()[modelName] = proxy; }

private:
    QVector<std::shared_ptr<ModelPrototype>> activeModels;
    map_type allModels;
};

#endif // MODELMANAGER_H
