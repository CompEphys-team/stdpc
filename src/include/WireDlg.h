#pragma once

#include "ConductanceDlg.h"
#include <QAbstractButton>
#include "ui_WireDlg.h"
#include "ObjectDataTypes.h"

class WireDlg : public ConductanceDlg, private Ui::WireDlg
{
     Q_OBJECT

  public:
     WireDlg(int, QWidget *parent= nullptr);
     void exportData();
     void importData();
     void setIndex(size_t);

  public slots:
     void CloseClicked(QAbstractButton *);
};
