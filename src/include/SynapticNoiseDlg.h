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
    explicit SynapticNoiseDlg(int idx, QWidget *parent = 0);
    ~SynapticNoiseDlg();

    void importData();
    void exportData();
    void setIndex(int);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SynapticNoiseDlg *ui;
};

#endif // SYNAPTICNOISEDLG_H
