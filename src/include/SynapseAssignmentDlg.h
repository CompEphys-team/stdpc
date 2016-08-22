#ifndef SYNAPSEASSIGNMENTDLG_H
#define SYNAPSEASSIGNMENTDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include "ChannelListModel.h"
#include "ObjectDataTypes.h"

namespace Ui {
class SynapseAssignmentDlg;
}

class SynapseAssignmentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SynapseAssignmentDlg(QWidget *parent = 0);
    ~SynapseAssignmentDlg();
    void exportData(std::vector<SynapseAssignment> &);
    void importData(std::vector<SynapseAssignment> const&);

public slots:
    virtual void open() override;
    void updateChns();

private:
    Ui::SynapseAssignmentDlg *ui;
    QMetaObject::Connection boxc, prec, postc, outc;
    ChannelListModel inm, outm;
    QVector<QCheckBox*> boxes;
    QVector<QComboBox*> pres, posts, outs;

    bool reloadChns;

    void addRow(int row, QCheckBox *box, QComboBox *pre, QComboBox *post, QComboBox *out);

private slots:
    void growTable();
};

#endif // SYNAPSEASSIGNMENTDLG_H
