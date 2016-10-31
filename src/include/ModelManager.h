#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QDialog>
#include <memory>
#include "Model.h"
#include "ModelDlg.h"

class ModelProxy {
public:
    virtual ModelData& param(size_t i) = 0;
    virtual size_t size() = 0;
    virtual void resize(size_t) = 0;
    virtual void remove(size_t) = 0;

    virtual QString modelClass() = 0;
    virtual inline QString prettyName() { return modelClass(); }

    virtual ModelPrototype *createPrototype(size_t modelID) = 0;
    virtual ModelDlg *createDialog(size_t modelID, QWidget *parent=nullptr) = 0;
};


// Helper to allow using an `enum class` as unordered_map key
struct EnumClassHash
{
    template <typename T>
    size_t operator()(T t) const
    {
        return static_cast<size_t>(t);
    }
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

    static QHash<QString, ModelProxy*> Register;
    static inline void RegisterModel(QString modelName, ModelProxy *proxy) { Register[modelName] = proxy; }

signals:
    void removedModel(ChannelIndex dex);

private:
    QVector<std::shared_ptr<ModelPrototype>> activeModels;
    map_type allModels;
};

#endif // MODELMANAGER_H
