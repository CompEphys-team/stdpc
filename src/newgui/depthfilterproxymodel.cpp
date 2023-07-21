#include "depthfilterproxymodel.h"

bool DepthFilterProxyModel::filterAcceptsRow(int, const QModelIndex &source_parent) const
{
    if ( depth < 0 )
        return true;
    QModelIndex index = source_parent;
    int theDepth = 0;
    while ( index.isValid() ) {
        ++theDepth;
        index = index.parent();
    }
    return theDepth < depth;
}
