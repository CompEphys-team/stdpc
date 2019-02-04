#pragma once

#include "ui_VStepsDlg.h"
#include "VSteps.h"
#include "ModelDlg.h"
#include "ChannelListModel.h"

class VStepsDlg : public ModelDlg
{
    Q_OBJECT

public:
    explicit VStepsDlg(size_t idx, QWidget *parent = nullptr);
    ~VStepsDlg();

    void importData();
    void exportData();
    void setIndex(size_t);

private:
    Ui::VStepsDlg *ui;
    QString label;

    ChannelListModel clm;
};


