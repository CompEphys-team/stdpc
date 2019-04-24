#ifndef TREEWIDGETITEM_DATASOURCES_H
#define TREEWIDGETITEM_DATASOURCES_H

#include <QWidget>

namespace Ui {
class TreeWidgetItem_DataSources;
}

class TreeWidgetItem_DataSources : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidgetItem_DataSources(QWidget *parent = nullptr);
    ~TreeWidgetItem_DataSources();

public slots:
    void importData(bool activeOnly = false);
    void exportData(bool ignoreDAQ = false);

private:
    Ui::TreeWidgetItem_DataSources *ui;
};

#endif // TREEWIDGETITEM_DATASOURCES_H
