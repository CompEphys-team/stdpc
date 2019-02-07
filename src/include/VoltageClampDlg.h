#pragma once

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_VoltageClampDlg.h"
#include "ObjectDataTypes.h"

class VoltageClampDlg : public ConductanceDlg, private Ui::VoltageClampDlg
{
     Q_OBJECT

  public:
     VoltageClampDlg(int, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);

  public slots:
     void CloseClicked(QAbstractButton *);
};
