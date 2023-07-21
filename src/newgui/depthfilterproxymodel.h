#ifndef DEPTHFILTERPROXYMODEL_H
#define DEPTHFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class DepthFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit DepthFilterProxyModel(QObject* parent = nullptr) : QSortFilterProxyModel(parent) {}

    inline void setDepth(int newDepth = -1) {
        depth = newDepth;
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    int depth;
};

#endif // DEPTHFILTERPROXYMODEL_H
