#ifndef ASSIGNMENTWIDGET_H
#define ASSIGNMENTWIDGET_H

#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include "ChannelListModel.h"

template <class Assignment>
struct Dropdown
{
    int Assignment::* channel;
    ChannelListModel *model;
    QString label;
    int columnWidth;

    QMetaObject::Connection connection;
    QVector<QComboBox *> combo;
};


class AssignmentWidgetQ : public QTableWidget
{
    Q_OBJECT
public:
    explicit AssignmentWidgetQ(QWidget *parent = 0) : QTableWidget(parent) {}
    ~AssignmentWidgetQ() {}

protected:
    void fixWidth(QComboBox *box, ChannelListModel *model) {
        connect(model, ChannelListModel::layoutChanged, [=](){ChannelListModel::fixComboBoxWidth(box);});
        ChannelListModel::fixComboBoxWidth(box);
    }

protected slots:
    virtual void grow() {}
};


template <class Assignment>
class AssignmentWidget : public AssignmentWidgetQ
{
public:
    AssignmentWidget(QVector<Dropdown<Assignment>>, QWidget *parent = 0);
    ~AssignmentWidget() {}

    void importData(std::vector<Assignment> const&);
    void exportData(std::vector<Assignment> &);

private:
    QMetaObject::Connection boxc;
    QVector<QCheckBox *> boxes;

    QVector<Dropdown<Assignment>> drops;

    void addRow(int row, QCheckBox *box);

protected:
    void grow();
};

#endif // ASSIGNMENTWIDGET_H
