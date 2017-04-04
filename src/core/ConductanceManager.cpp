#include "ConductanceManager.h"
#include "DCThread.h"

ConductanceManager::~ConductanceManager()
{
    for ( Conductance *c : preD )
        delete c;
    for ( Conductance *c : inD )
        delete c;
    for ( Conductance *c : postD )
        delete c;
}

void ConductanceManager::init(DCThread *DCT)
{
    for ( Conductance *c : preD )
        delete c;
    for ( Conductance *c : inD )
        delete c;
    for ( Conductance *c : postD )
        delete c;
    preD.clear();
    inD.clear();
    postD.clear();

    for ( ConductanceProxy *proxy : ConductanceManager::Register() ) {
        for ( size_t j = 0; j < proxy->size(); j++ ) {
            if ( proxy->param(j).active ) {
                for ( size_t i = 0; i < proxy->param(j).numAssignments(); i++ ) {
                    if ( proxy->param(j).assignment(i).active ) {
                        proxy->instantiate(j, i, DCT, preD, inD, postD);
                    }
                }
            }
        }
    }
}

const double *match(const std::vector<Conductance*> ref, const ChannelIndex &dex)
{
    for ( Conductance *c : ref )
        if ( c->proxy()->conductanceClass() == dex.conductanceClass &&
             c->conductanceID() == dex.conductanceID &&
             c->assignmentID() == dex.assignID )
            return &c->conductance();
    return nullptr;
}

const double *ConductanceManager::conductance(const ChannelIndex &dex)
{
    const double *ret = nullptr;
    if ( !dex.isConductance )
        return ret;
    if ( (ret = match(preD, dex)) || (ret = match(inD, dex)) || (ret = match(postD, dex)) )
        return ret;
    return ret;
}
