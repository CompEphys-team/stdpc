#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QVector>
#include <unordered_map>
#include <memory>
#include "Model.h"
#include "ObjectDataTypes.h"

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

    /// Create a model of the given type from params[idx]
    template <typename param_type>
    void initSingle(std::vector<param_type> &params, int idx)
    {
        ModelClass mc = param_type::modelClass;
        // Clear existing active model
        for ( int i = 0; i < activeModels.size(); i++ )
            if ( activeModels[i]->modelClass() == mc && activeModels[i]->modelID == idx )
                activeModels.remove(i);

        // Ensure enough space in allModels
        if ( allModels[mc].size() <= idx )
            allModels[mc].resize(idx+1);

        // Assign new to allModels
        allModels[mc][idx].reset(new typename param_type::ModelType(idx));

        // Assign to activeModels if appropriate
        if ( params[idx].active ) {
            for ( size_t i = 0; i < params[idx].numInst(); i++ ) {
                if ( params[idx].instance(idx).active ) {
                    activeModels.push_back(allModels[mc][idx]);
                    break;
                }
            }
        }
    }

    /// Remove a model from active use, freeing its space and moving up all subsequent models of the same type.
    /// The corresponding parameter set is moved to the end of the param vector.
    /// Emits removedModel with an appropriate ChannelIndex
    void remove(ModelClass type, int idx);

    inChannel *getInChan(ChannelIndex const&);
    outChannel *getOutChan(ChannelIndex const&);

    typedef std::unordered_map<ModelClass, QVector<std::shared_ptr<ModelPrototype>>, EnumClassHash> map_type;

    inline QVector<std::shared_ptr<ModelPrototype>> const& active() const { return activeModels; }
    inline map_type const& all() const { return allModels; }

signals:
    void removedModel(ChannelIndex dex);

private:
    QVector<std::shared_ptr<ModelPrototype>> activeModels;
    map_type allModels;
};

#endif // MODELMANAGER_H
