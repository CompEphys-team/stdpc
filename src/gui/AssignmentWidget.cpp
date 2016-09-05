#include "AssignmentWidget.h"
#include <QHBoxLayout>
#include <functional>

template class AssignmentWidget<CurrentAssignment>;
template class AssignmentWidget<SynapseAssignment>;
template class AssignmentWidget<GapJunctionAssignment>;

template <class A>
void AssignmentWidget<A>::init(QVector<Dropdown<A>> &drops)
{
    this->drops = drops;
    QStringList labels("Active");
    int i = 1;
    setColumnCount(drops.size() + 1);
    setColumnWidth(0, 47);
    for ( Dropdown<A> &d : drops ) {
        setColumnWidth(i++, d.columnWidth);
        labels.append(d.label);
    }
    setHorizontalHeaderLabels(labels);
    grow(false);
}

template <class A>
void AssignmentWidget<A>::exportData(std::vector<A> &p)
{
    A a;
    p.clear();
    for ( int i = 0; i < rowCount() - 1; i++ ) {
        a.active = boxes[i]->isChecked();
        for ( Dropdown<A> &d : drops )
            a.*(d.channel) = d.combo[i]->currentData().toPoint().x();
        p.push_back(a);
    }
}

template <class A>
void AssignmentWidget<A>::importData(std::vector<A> const& p)
{
    QCheckBox *box;

    boxes.clear();
    for ( Dropdown<A> &d : drops ) {
        d.combo.clear();
        disconnect(d.connection);
    }
    setRowCount(0);

    int i = 0;
    for ( A const& a : p ) {
        box = new QCheckBox(this);
        box->setChecked(a.active);
        addRow(i, box);
        for ( Dropdown<A> &d : drops ) {
            d.combo[i]->setCurrentIndex(d.model->index(a.*(d.channel)));
            ChannelListModel::fixComboBoxWidth(d.combo[i]);
        }
        ++i;
    }

    grow(false);
}

template <class A>
void AssignmentWidget<A>::addRow(int row, QCheckBox *box)
{
    insertRow(row);
    setRowHeight(row, 25);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(box);
    layout->setAlignment(Qt::AlignCenter);
    layout->setMargin(0);
    widget->setLayout(layout);
    setCellWidget(row, 0, widget);
    boxes.insert(row, box);

    int i = 1;
    for ( Dropdown<A> &d : drops ) {
        QComboBox *combo = new QComboBox(this);
        combo->setModel(d.model);
        setCellWidget(row, i++, combo);
        fixWidth(combo, d.model);
        d.combo.insert(row, combo);
    }
}

template <class A>
void AssignmentWidget<A>::grow(bool reactive)
{
    disconnect(boxc);
    QCheckBox *box = new QCheckBox();
    if ( reactive && !boxes.empty() )
        boxes.last()->setChecked(true);
    boxc = connect(box, SIGNAL(stateChanged(int)), this, SLOT(grow()));

    addRow(rowCount(), box);

    for ( Dropdown<A> &d : drops ) {
        disconnect(d.connection);
        d.connection = connect(d.combo.last(), SIGNAL(currentIndexChanged(int)), this, SLOT(grow()));
    }
}
