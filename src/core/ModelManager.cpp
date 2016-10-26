#include "ModelManager.h"

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

void ModelManager::remove(ModelClass type, int idx)
{
    for ( int i = 0; i < activeModels.size(); i++ )
        if ( activeModels[i]->modelClass() == type && activeModels[i]->modelID == idx )
            activeModels.remove(i);

    if ( allModels[type].size() > idx ) {
        allModels[type].remove(idx);
        for ( int i = idx; i < allModels[type].size(); i++ )
            allModels[type][i]->modelID = i;
        emit removedModel(ChannelIndex(type, idx));
    }
}

inChannel *ModelManager::getInChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > dex.modelID
         && (int)allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->in);
    }
    return nullptr;
}

outChannel *ModelManager::getOutChan(const ChannelIndex &dex)
{
    if ( dex.isValid && dex.isVirtual
         && allModels[dex.modelClass].size() > dex.modelID
         && (int)allModels[dex.modelClass][dex.modelID]->instances().size() > dex.instID ) {
        return &(allModels[dex.modelClass][dex.modelID]->instances()[dex.instID]->out);
    }
    return nullptr;
}
