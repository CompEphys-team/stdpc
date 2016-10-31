#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <memory>
#include "Model.h"
#include "ObjectDataTypes.h"

class ModelProxy {
public:
    virtual ModelData& param(size_t i) = 0;
    virtual size_t size() = 0;
    virtual QString modelClass() = 0;
    virtual ModelPrototype *createPrototype(int modelID) = 0;
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

    /// Remove all models e.g. in preparation for parameter import
    void clear();

    /// Create a model of the given type from proxy->param(idx)
    void initSingle(ModelProxy *proxy, size_t idx);

    /// Remove a model from active use, freeing its space and moving up all subsequent models of the same type.
    /// Emits removedModel with an appropriate ChannelIndex
    //void remove(QString type, size_t idx);

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
