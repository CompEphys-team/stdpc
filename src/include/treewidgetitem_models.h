#ifndef TREEWIDGETITEM_MODELS_H
#define TREEWIDGETITEM_MODELS_H

#include <QWidget>
#include "ConductanceManager.h"

namespace Ui {
class TreeWidgetItem_Models;
}

class TreeWidgetItem_Models : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidgetItem_Models(const QMap<QString, ConductanceProxy *> &proxies, QWidget *parent = nullptr);
    ~TreeWidgetItem_Models();

public slots:
    void importData(bool activeOnly = false);
    void exportData();

private:
    Ui::TreeWidgetItem_Models *ui;
};

#endif // TREEWIDGETITEM_MODELS_H
