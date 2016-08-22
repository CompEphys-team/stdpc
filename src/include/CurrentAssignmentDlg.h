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
    void exportData(mhHHData &);
    void importData(mhHHData const&);

signals:
    void chnsChanged();

private:
    Ui::CurrentAssignmentDlg *ui;
    QMetaObject::Connection boxc, inc, outc;
    ChannelListModel inm, outm;
    QVector<QCheckBox*> boxes;
    QVector<QComboBox*> ins, outs;

    void addRow(int row, QCheckBox *box, QComboBox *in, QComboBox *out);

private slots:
    void growTable();
};

#endif // CURRENTASSIGNMENTDLG_H
