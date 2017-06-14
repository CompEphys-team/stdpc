#ifndef TRIGGERDLG_H
#define TRIGGERDLG_H

#include <QDialog>

namespace Ui {
class TriggerDlg;
}

class TriggerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TriggerDlg(QWidget *parent = 0);
    ~TriggerDlg();
    void exportData();
    void importData();

public slots:
    void updateChannels();

private:
    Ui::TriggerDlg *ui;
};

#endif // TRIGGERDLG_H
