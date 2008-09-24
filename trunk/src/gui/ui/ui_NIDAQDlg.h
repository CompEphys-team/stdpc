/********************************************************************************
** Form generated from reading ui file 'NIDAQDlg.ui'
**
** Created: Wed Sep 24 11:44:57 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NIDAQDLG_H
#define UI_NIDAQDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_NIDAQDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *DeviceNameE;
    QLabel *BaseAddressL;

    void setupUi(QDialog *NIDAQDlg)
    {
    if (NIDAQDlg->objectName().isEmpty())
        NIDAQDlg->setObjectName(QString::fromUtf8("NIDAQDlg"));
    NIDAQDlg->resize(400, 127);
    buttonBox = new QDialogButtonBox(NIDAQDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(211, 80, 160, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    DeviceNameE = new QLineEdit(NIDAQDlg);
    DeviceNameE->setObjectName(QString::fromUtf8("DeviceNameE"));
    DeviceNameE->setGeometry(QRect(110, 30, 113, 20));
    BaseAddressL = new QLabel(NIDAQDlg);
    BaseAddressL->setObjectName(QString::fromUtf8("BaseAddressL"));
    BaseAddressL->setGeometry(QRect(30, 30, 71, 16));

    retranslateUi(NIDAQDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), NIDAQDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), NIDAQDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(NIDAQDlg);
    } // setupUi

    void retranslateUi(QDialog *NIDAQDlg)
    {
    NIDAQDlg->setWindowTitle(QApplication::translate("NIDAQDlg", "NIDAQ Setup", 0, QApplication::UnicodeUTF8));
    DeviceNameE->setText(QApplication::translate("NIDAQDlg", "Dev1", 0, QApplication::UnicodeUTF8));
    BaseAddressL->setText(QApplication::translate("NIDAQDlg", "Device Name", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(NIDAQDlg);
    } // retranslateUi

};

namespace Ui {
    class NIDAQDlg: public Ui_NIDAQDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NIDAQDLG_H
