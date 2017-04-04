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
