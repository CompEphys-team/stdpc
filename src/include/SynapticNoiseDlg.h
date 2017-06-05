#ifndef SYNAPTICNOISEDLG_H
#define SYNAPTICNOISEDLG_H

#include "ConductanceDlg.h"

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

private:
    Ui::SynapticNoiseDlg *ui;
    
    QString label;
};

#endif // SYNAPTICNOISEDLG_H
