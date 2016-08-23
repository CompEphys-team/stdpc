#ifndef GAPJUNCTIONASSIGNMENTDLG_H
#define GAPJUNCTIONASSIGNMENTDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include "ChannelListModel.h"
#include "ObjectDataTypes.h"

namespace Ui {
class GapJunctionAssignmentDlg;
}

class GapJunctionAssignmentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GapJunctionAssignmentDlg(QWidget *parent = 0);
    ~GapJunctionAssignmentDlg();
    void exportData(std::vector<GapJunctionAssignment> &);
    void importData(std::vector<GapJunctionAssignment> const&);

public slots:
    virtual void open() override;
    void updateChns();

private:
    Ui::GapJunctionAssignmentDlg *ui;
    QMetaObject::Connection boxc, preinc, postinc, preoutc, postoutc;
    ChannelListModel inm, outm;
    QVector<QCheckBox*> boxes;
    QVector<QComboBox*> preins, postins, preouts, postouts;

    bool reloadChns;

    void addRow(int row, QCheckBox *box,
                QComboBox *prein, QComboBox *postin,
                QComboBox *preout, QComboBox *postout);

private slots:
    void growTable();
};

#endif // GAPJUNCTIONASSIGNMENTDLG_H
