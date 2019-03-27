#ifndef SYNAPTICNOISEDLG_H
#define SYNAPTICNOISEDLG_H

#include "ConductanceDlg.h"
#include <QAbstractButton>

namespace Ui {
class SynapticNoiseDlg;
}

class SynapticNoiseDlg : public ConductanceDlg
{
    Q_OBJECT

public:
    explicit SynapticNoiseDlg(size_t idx, QWidget *parent = nullptr);
    ~SynapticNoiseDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SynapticNoiseDlg *ui;
};

#endif // SYNAPTICNOISEDLG_H
