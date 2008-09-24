/********************************************************************************
** Form generated from reading ui file 'DigiDataDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIGIDATADLG_H
#define UI_DIGIDATADLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DigiDataDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *BaseAddressE;
    QLabel *BaseAddressL;
    QLabel *xlb;

    void setupUi(QDialog *DigiDataDlg)
    {
    if (DigiDataDlg->objectName().isEmpty())
        DigiDataDlg->setObjectName(QString::fromUtf8("DigiDataDlg"));
    DigiDataDlg->resize(400, 127);
    buttonBox = new QDialogButtonBox(DigiDataDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(211, 80, 160, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    BaseAddressE = new QLineEdit(DigiDataDlg);
    BaseAddressE->setObjectName(QString::fromUtf8("BaseAddressE"));
    BaseAddressE->setGeometry(QRect(130, 30, 113, 20));
    BaseAddressL = new QLabel(DigiDataDlg);
    BaseAddressL->setObjectName(QString::fromUtf8("BaseAddressL"));
    BaseAddressL->setGeometry(QRect(30, 30, 64, 16));
    xlb = new QLabel(DigiDataDlg);
    xlb->setObjectName(QString::fromUtf8("xlb"));
    xlb->setGeometry(QRect(110, 32, 16, 16));
    xlb->setLayoutDirection(Qt::RightToLeft);

    retranslateUi(DigiDataDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), DigiDataDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), DigiDataDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(DigiDataDlg);
    } // setupUi

    void retranslateUi(QDialog *DigiDataDlg)
    {
    DigiDataDlg->setWindowTitle(QApplication::translate("DigiDataDlg", "DigiData 1200/1200A Setup", 0, QApplication::UnicodeUTF8));
    BaseAddressE->setText(QApplication::translate("DigiDataDlg", "320", 0, QApplication::UnicodeUTF8));
    BaseAddressL->setText(QApplication::translate("DigiDataDlg", "Base address", 0, QApplication::UnicodeUTF8));
    xlb->setText(QApplication::translate("DigiDataDlg", "0x", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(DigiDataDlg);
    } // retranslateUi

};

namespace Ui {
    class DigiDataDlg: public Ui_DigiDataDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIGIDATADLG_H
