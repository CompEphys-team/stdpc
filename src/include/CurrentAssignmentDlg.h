#ifndef CURRENTASSIGNMENTDLG_H
#define CURRENTASSIGNMENTDLG_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include "ChannelListModel.h"
#include "ObjectDataTypes.h"

namespace Ui {
class CurrentAssignmentDlg;
}

class CurrentAssignmentDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CurrentAssignmentDlg(QWidget *parent = 0);
    ~CurrentAssignmentDlg();
    void exportData(std::vector<CurrentAssignment> &);
    void importData(std::vector<CurrentAssignment> const&);

public slots:
    virtual void open() override;
    void updateChns();

private:
    Ui::CurrentAssignmentDlg *ui;
    QMetaObject::Connection boxc, inc, outc;
    ChannelListModel inm, outm;
    QVector<QCheckBox*> boxes;
    QVector<QComboBox*> ins, outs;

    bool reloadChns;

    void addRow(int row, QCheckBox *box, QComboBox *in, QComboBox *out);

private slots:
    void growTable();
};

#endif // CURRENTASSIGNMENTDLG_H
