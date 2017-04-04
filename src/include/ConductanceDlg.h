#ifndef CONDUCTANCEDLG_H
#define CONDUCTANCEDLG_H

#include <QDialog>

class ConductanceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConductanceDlg(size_t idx, QWidget *parent = 0) : QDialog(parent), idx(idx) {}
    virtual ~ConductanceDlg() {}

    virtual void importData() = 0;
    virtual void exportData() = 0;
    virtual void setIndex(size_t i) { idx = i; }

protected:
    size_t idx;
};

#endif // CONDUCTANCEDLG_H
