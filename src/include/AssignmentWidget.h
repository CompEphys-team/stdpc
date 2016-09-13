#ifndef ASSIGNMENTWIDGET_H
#define ASSIGNMENTWIDGET_H

#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include "ChannelListModel.h"
#include "ObjectDataTypes.h"

template <class Assignment>
struct Dropdown
{
    Dropdown(){}
    Dropdown(ChannelIndex Assignment::* c, ChannelListModel *m, QString const& l, int w) :
        channel(c), model(m), label(l), columnWidth(w)
    {}

    ChannelIndex Assignment::* channel;
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

protected slots:
    virtual void grow(bool = true) {}
};


template <class Assignment>
class AssignmentWidget : public AssignmentWidgetQ
{
public:
    AssignmentWidget(QWidget *parent = 0) : AssignmentWidgetQ(parent) {}
    ~AssignmentWidget() {}

    void init(QVector<Dropdown<Assignment>> &);

    void importData(std::vector<Assignment> const&);
    void exportData(std::vector<Assignment> &);

private:
    QMetaObject::Connection boxc;
    QVector<QCheckBox *> boxes;

    QVector<Dropdown<Assignment>> drops;

    void addRow(int row, QCheckBox *box);

protected:
    void grow(bool reactive = true);
};

#endif // ASSIGNMENTWIDGET_H
