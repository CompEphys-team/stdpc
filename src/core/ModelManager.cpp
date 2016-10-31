#include "ModelManager.h"

QHash<QString, ModelProxy*> ModelManager::Register;

void ModelManager::initActive(DCThread *DCT)
{
    for ( std::shared_ptr<ModelPrototype> &m : activeModels )
        m->init(DCT);
}

void ModelManager::clear()
{
    activeModels.clear();
    allModels.clear();
}

void ModelManager::initSingle(ModelProxy *proxy, size_t idx)
{
    QString const& mc = proxy->modelClass();

    // Clear existing active model
    for ( int i = 0; i < activeModels.size(); i++ )
        if ( activeModels[i]->proxy()->modelClass() == mc && activeModels[i]->modelID == idx )
            activeModels.remove(i);

    // Ensure enough space in allModels
    if ( allModels[mc].size() <= (int)idx )
        allModels[mc].resize(idx+1);

    // Assign new to allModels
    allModels[mc][idx].reset(proxy->createPrototype(idx));

    // Assign to activeModels if appropriate
    if ( proxy->param(idx).active ) {
        for ( size_t i = 0; i < proxy->param(idx).numInst(); i++ ) {
            if ( proxy->param(idx).instance(i).active ) {
                activeModels.push_back(allModels[mc][idx]);
                break;
            }
        }
    }
}

//void ModelManager::remove(QString type, size_t idx)
//{
//    if ( !allModels.contains(type) || allModels[type].size() <= (int)idx )
//        return;

//    for ( int i = 0; i < activeModels.size(); i++ )
//        if ( activeModels[i]->proxy()->modelClass() == type && activeModels[i]->modelID == idx )
//            activeModels.remove(i);

//    if ( allModels.value(type).size() > (int)idx ) {
//        allModels[type][idx]->proxy()->remove(idx);
//        allModels[type].remove(idx);
//        for ( size_t i = idx; (int)i < allModels[type].size(); i++ )
//            allModels[type][i]->modelID = i;
//        emit removedModel(ChannelIndex(ChannelIndex::Prototype, type, idx));
//    }
//}

inChannel *ModelManager::getInChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > (int)dex.modelID
         && allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->in);
    }
    return nullptr;
}

outChannel *ModelManager::getOutChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > (int)dex.modelID
         && allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->out);
    }
    return nullptr;
}
