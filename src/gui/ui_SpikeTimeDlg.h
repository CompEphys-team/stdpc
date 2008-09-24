/********************************************************************************
** Form generated from reading ui file 'SpikeTimeDlg.ui'
**
** Created: Mon Aug 18 17:20:46 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SPIKETIMEDLG_H
#define UI_SPIKETIMEDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_SpikeTimeDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *isi2E;
    QLabel *label_4;
    QLineEdit *isi4E;
    QLabel *label_13;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_10;
    QLineEdit *isi1E;
    QLineEdit *isi0E;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *isi3E;
    QLabel *label_7;
    QLineEdit *isi9E;
    QLabel *label_15;
    QLabel *label_23;
    QLineEdit *isi7E;
    QLabel *label_18;
    QLabel *label_21;
    QLabel *label_16;
    QLineEdit *isi6E;
    QLabel *label_17;
    QLabel *label_20;
    QLineEdit *isi5E;
    QLabel *label_14;
    QLabel *label_22;
    QLineEdit *isi8E;
    QLabel *label_19;

    void setupUi(QDialog *SpikeTimeDlg)
    {
    if (SpikeTimeDlg->objectName().isEmpty())
        SpikeTimeDlg->setObjectName(QString::fromUtf8("SpikeTimeDlg"));
    SpikeTimeDlg->resize(359, 226);
    buttonBox = new QDialogButtonBox(SpikeTimeDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(181, 180, 160, 32));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    buttonBox->setFont(font);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    isi2E = new QLineEdit(SpikeTimeDlg);
    isi2E->setObjectName(QString::fromUtf8("isi2E"));
    isi2E->setGeometry(QRect(70, 80, 61, 20));
    isi2E->setMaxLength(8);
    isi2E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_4 = new QLabel(SpikeTimeDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 20, 34, 16));
    isi4E = new QLineEdit(SpikeTimeDlg);
    isi4E->setObjectName(QString::fromUtf8("isi4E"));
    isi4E->setGeometry(QRect(70, 140, 61, 20));
    isi4E->setMaxLength(8);
    isi4E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_13 = new QLabel(SpikeTimeDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(140, 140, 16, 16));
    label_5 = new QLabel(SpikeTimeDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 50, 34, 16));
    label_9 = new QLabel(SpikeTimeDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(140, 20, 16, 16));
    label_6 = new QLabel(SpikeTimeDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 80, 34, 16));
    label_8 = new QLabel(SpikeTimeDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(20, 140, 34, 16));
    label_10 = new QLabel(SpikeTimeDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(140, 50, 16, 16));
    isi1E = new QLineEdit(SpikeTimeDlg);
    isi1E->setObjectName(QString::fromUtf8("isi1E"));
    isi1E->setGeometry(QRect(70, 50, 61, 20));
    isi1E->setMaxLength(8);
    isi1E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    isi0E = new QLineEdit(SpikeTimeDlg);
    isi0E->setObjectName(QString::fromUtf8("isi0E"));
    isi0E->setGeometry(QRect(70, 20, 61, 20));
    isi0E->setMaxLength(8);
    isi0E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_11 = new QLabel(SpikeTimeDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(140, 80, 16, 16));
    label_12 = new QLabel(SpikeTimeDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(140, 110, 16, 16));
    isi3E = new QLineEdit(SpikeTimeDlg);
    isi3E->setObjectName(QString::fromUtf8("isi3E"));
    isi3E->setGeometry(QRect(70, 110, 61, 20));
    isi3E->setMaxLength(8);
    isi3E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_7 = new QLabel(SpikeTimeDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(20, 110, 34, 16));
    isi9E = new QLineEdit(SpikeTimeDlg);
    isi9E->setObjectName(QString::fromUtf8("isi9E"));
    isi9E->setGeometry(QRect(250, 140, 61, 20));
    isi9E->setMaxLength(8);
    isi9E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_15 = new QLabel(SpikeTimeDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(200, 80, 34, 16));
    label_23 = new QLabel(SpikeTimeDlg);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setGeometry(QRect(320, 140, 16, 16));
    isi7E = new QLineEdit(SpikeTimeDlg);
    isi7E->setObjectName(QString::fromUtf8("isi7E"));
    isi7E->setGeometry(QRect(250, 80, 61, 20));
    isi7E->setMaxLength(8);
    isi7E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_18 = new QLabel(SpikeTimeDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(200, 20, 34, 16));
    label_21 = new QLabel(SpikeTimeDlg);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setGeometry(QRect(200, 110, 34, 16));
    label_16 = new QLabel(SpikeTimeDlg);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setGeometry(QRect(200, 140, 37, 16));
    isi6E = new QLineEdit(SpikeTimeDlg);
    isi6E->setObjectName(QString::fromUtf8("isi6E"));
    isi6E->setGeometry(QRect(250, 50, 61, 20));
    isi6E->setMaxLength(8);
    isi6E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_17 = new QLabel(SpikeTimeDlg);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setGeometry(QRect(320, 80, 16, 16));
    label_20 = new QLabel(SpikeTimeDlg);
    label_20->setObjectName(QString::fromUtf8("label_20"));
    label_20->setGeometry(QRect(320, 50, 16, 16));
    isi5E = new QLineEdit(SpikeTimeDlg);
    isi5E->setObjectName(QString::fromUtf8("isi5E"));
    isi5E->setGeometry(QRect(250, 20, 61, 20));
    isi5E->setMaxLength(8);
    isi5E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_14 = new QLabel(SpikeTimeDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(200, 50, 34, 16));
    label_22 = new QLabel(SpikeTimeDlg);
    label_22->setObjectName(QString::fromUtf8("label_22"));
    label_22->setGeometry(QRect(320, 110, 16, 16));
    isi8E = new QLineEdit(SpikeTimeDlg);
    isi8E->setObjectName(QString::fromUtf8("isi8E"));
    isi8E->setGeometry(QRect(250, 110, 61, 20));
    isi8E->setMaxLength(8);
    isi8E->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_19 = new QLabel(SpikeTimeDlg);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setGeometry(QRect(320, 20, 16, 16));

    retranslateUi(SpikeTimeDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), SpikeTimeDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), SpikeTimeDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(SpikeTimeDlg);
    } // setupUi

    void retranslateUi(QDialog *SpikeTimeDlg)
    {
    SpikeTimeDlg->setWindowTitle(QApplication::translate("SpikeTimeDlg", "Explicit Spike Times", 0, QApplication::UnicodeUTF8));
    isi2E->setText(QApplication::translate("SpikeTimeDlg", "68.0", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("SpikeTimeDlg", "Spike 1", 0, QApplication::UnicodeUTF8));
    isi4E->setText(QApplication::translate("SpikeTimeDlg", "112.0", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("SpikeTimeDlg", "Spike 2", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("SpikeTimeDlg", "Spike 3", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("SpikeTimeDlg", "Spike 5", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    isi1E->setText(QApplication::translate("SpikeTimeDlg", "50.0", 0, QApplication::UnicodeUTF8));
    isi0E->setText(QApplication::translate("SpikeTimeDlg", "30.0", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    isi3E->setText(QApplication::translate("SpikeTimeDlg", "88.0", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("SpikeTimeDlg", "Spike 4", 0, QApplication::UnicodeUTF8));
    isi9E->setText(QApplication::translate("SpikeTimeDlg", "308.0", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("SpikeTimeDlg", "Spike 8", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    isi7E->setText(QApplication::translate("SpikeTimeDlg", "204.0", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("SpikeTimeDlg", "Spike 6", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("SpikeTimeDlg", "Spike 9", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("SpikeTimeDlg", "Spike10", 0, QApplication::UnicodeUTF8));
    isi6E->setText(QApplication::translate("SpikeTimeDlg", "168.0", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    isi5E->setText(QApplication::translate("SpikeTimeDlg", "138.0", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("SpikeTimeDlg", "Spike 7", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    isi8E->setText(QApplication::translate("SpikeTimeDlg", "248.0", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("SpikeTimeDlg", "ms", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SpikeTimeDlg);
    } // retranslateUi

};

namespace Ui {
    class SpikeTimeDlg: public Ui_SpikeTimeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPIKETIMEDLG_H
