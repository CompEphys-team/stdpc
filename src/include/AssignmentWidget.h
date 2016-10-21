#ifndef ASSIGNMENTWIDGET_H
#define ASSIGNMENTWIDGET_H

#include <QTableWidget>
#include <QCheckBox>
#include "ChannelListModel.h"
#include "ObjectDataTypes.h"
#include "AssignmentCell.h"


class AssignmentWidgetQ : public QTableWidget
{
    Q_OBJECT
public:
    explicit AssignmentWidgetQ(QWidget *parent = 0) : QTableWidget(parent) {}
    ~AssignmentWidgetQ() {}

protected slots:
    virtual void addRow(int = -1) {}
};


template <class Assignment>
class AssignmentWidget : public AssignmentWidgetQ
{
public:
    AssignmentWidget(QWidget *parent = 0) : AssignmentWidgetQ(parent) {}
    ~AssignmentWidget()
    {
        for ( AssignmentCellBase<Assignment> *d : cell )
            delete d;
    }

    void init(QVector<AssignmentCellBase<Assignment>*> &cells)
    {
        cell = cells;
        QStringList labels;
        setColumnCount(cell.size());
        int i = 0;
        for ( AssignmentCellBase<Assignment> *d : cell ) {
            setColumnWidth(i++, d->columnWidth);
            labels.append(d->label);
            connect(d, SIGNAL(grow()), this, SLOT(addRow()));
        }
        setHorizontalHeaderLabels(labels);
        addRow();
    }

    void importData(std::vector<Assignment> const& p)
    {
        for ( AssignmentCellBase<Assignment> *d : cell ) {
            d->clear();
        }
        setRowCount(0);

        int i = 0;
        for ( Assignment const& a : p ) {
            addRow(i);
            for ( AssignmentCellBase<Assignment> *d : cell ) {
                d->importData(i, a);
            }
            ++i;
        }

        addRow();
    }

    void exportData(std::vector<Assignment> &p)
    {
        Assignment a;
        p.clear();
        for ( int i = 0; i < rowCount() - 1; i++ ) {
            for ( AssignmentCellBase<Assignment> *d : cell )
                d->exportData(i, a);
            p.push_back(a);
        }
    }

private:
    QVector<AssignmentCellBase<Assignment> *> cell;

protected:
    void addRow(int row = -1)
    {
        if ( row < 0 )
            row = rowCount();
        insertRow(row);
        int i = 0;
        for ( AssignmentCellBase<Assignment> *d : cell ) {
            setCellWidget(row, i++, d->extend(row));
        }
    }
};

#endif // ASSIGNMENTWIDGET_H
