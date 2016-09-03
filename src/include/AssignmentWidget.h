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

template <class Assignment>
class AssignmentWidget : public QTableWidget
{
public:
    AssignmentWidget(QVector<Dropdown<Assignment>>, QWidget *parent = 0);

    void importData(std::vector<Assignment> &);
    void exportData(std::vector<Assignment> const&);

private:
    QMetaObject::Connection boxc;
    QVector<QCheckBox *> boxes;

    QVector<Dropdown<Assignment>> drops;

    void addRow(int row, QCheckBox *box);

private slots:
    void grow();
};

#endif // ASSIGNMENTWIDGET_H
