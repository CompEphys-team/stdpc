#ifndef MODELDLG_H
#define MODELDLG_H

#include <QDialog>

class ModelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ModelDlg(size_t idx, QWidget *parent = nullptr) : QDialog(parent), idx(idx) {}
    virtual ~ModelDlg() {}

    virtual void importData() = 0;
    virtual void exportData() = 0;
    virtual void setIndex(size_t i) { idx = i; }

public slots:
    inline void accept() { exportData(); QDialog::accept(); }
    inline void reject() { importData(); QDialog::reject(); }

signals:
    void channelsChanged();
    void modelStatusChanged();

protected:
    size_t idx;
};

#endif // MODELDLG_H
