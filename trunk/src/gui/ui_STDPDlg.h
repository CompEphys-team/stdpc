/********************************************************************************
** Form generated from reading ui file 'STDPDlg.ui'
**
** Created: Mon Aug 18 17:20:45 2008
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STDPDLG_H
#define UI_STDPDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_STDPDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *synSTDPLabel;
    QLabel *label_2;
    QLineEdit *APlusE;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_6;
    QLabel *label_10;
    QLineEdit *tauMinusE;
    QLineEdit *AMinusE;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *tauPlusE;
    QLabel *label_7;
    QLabel *label_14;
    QLineEdit *ExponentE;
    QLineEdit *ShiftE;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLineEdit *HistoryE;
    QLabel *label_18;
    QComboBox *TableCombo;
    QLabel *label_19;
    QLabel *tableDtL;
    QLineEdit *tableDtE;
    QLineEdit *tableDgMinE;
    QLabel *label_21;
    QLabel *TableDgMinL;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_13;
    QLineEdit *gMidE;
    QLineEdit *gSlopeE;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLineEdit *gMaxE;
    QLabel *label_26;
    QLineEdit *sigmoidTableMaxEntryE;
    QLabel *label_27;
    QLabel *sigmoidTableMaxEntryL;
    QLineEdit *sigmoidTableDgE;
    QLabel *label_29;
    QComboBox *sigmoidTableCombo;
    QLabel *sigmoidTableDgL;

    void setupUi(QDialog *STDPDlg)
    {
    if (STDPDlg->objectName().isEmpty())
        STDPDlg->setObjectName(QString::fromUtf8("STDPDlg"));
    STDPDlg->resize(391, 404);
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    STDPDlg->setFont(font);
    buttonBox = new QDialogButtonBox(STDPDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(210, 360, 161, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::NoButton|QDialogButtonBox::Reset);
    synSTDPLabel = new QLabel(STDPDlg);
    synSTDPLabel->setObjectName(QString::fromUtf8("synSTDPLabel"));
    synSTDPLabel->setEnabled(true);
    synSTDPLabel->setGeometry(QRect(290, 20, 76, 20));
    QFont font1;
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setWeight(75);
    synSTDPLabel->setFont(font1);
    synSTDPLabel->setAutoFillBackground(false);
    synSTDPLabel->setFrameShape(QFrame::Panel);
    synSTDPLabel->setFrameShadow(QFrame::Raised);
    synSTDPLabel->setLineWidth(1);
    synSTDPLabel->setMidLineWidth(0);
    label_2 = new QLabel(STDPDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 40, 81, 16));
    APlusE = new QLineEdit(STDPDlg);
    APlusE->setObjectName(QString::fromUtf8("APlusE"));
    APlusE->setGeometry(QRect(70, 130, 61, 20));
    QFont font2;
    font2.setBold(false);
    font2.setWeight(50);
    APlusE->setFont(font2);
    APlusE->setMaxLength(8);
    APlusE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_4 = new QLabel(STDPDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 70, 34, 16));
    label_4->setFont(font2);
    label_5 = new QLabel(STDPDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 100, 43, 16));
    label_5->setFont(font2);
    label_9 = new QLabel(STDPDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(140, 70, 16, 16));
    label_9->setFont(font2);
    label_6 = new QLabel(STDPDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 130, 26, 16));
    label_6->setFont(font2);
    label_10 = new QLabel(STDPDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(140, 100, 16, 16));
    label_10->setFont(font2);
    tauMinusE = new QLineEdit(STDPDlg);
    tauMinusE->setObjectName(QString::fromUtf8("tauMinusE"));
    tauMinusE->setGeometry(QRect(70, 100, 61, 20));
    tauMinusE->setFont(font2);
    tauMinusE->setMaxLength(8);
    tauMinusE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    AMinusE = new QLineEdit(STDPDlg);
    AMinusE->setObjectName(QString::fromUtf8("AMinusE"));
    AMinusE->setGeometry(QRect(70, 70, 61, 20));
    AMinusE->setFont(font2);
    AMinusE->setMaxLength(8);
    AMinusE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_11 = new QLabel(STDPDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(140, 130, 16, 16));
    label_11->setFont(font2);
    label_12 = new QLabel(STDPDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(140, 160, 16, 16));
    label_12->setFont(font2);
    tauPlusE = new QLineEdit(STDPDlg);
    tauPlusE->setObjectName(QString::fromUtf8("tauPlusE"));
    tauPlusE->setGeometry(QRect(70, 160, 61, 20));
    tauPlusE->setFont(font2);
    tauPlusE->setMaxLength(8);
    tauPlusE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_7 = new QLabel(STDPDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(20, 160, 35, 16));
    label_7->setFont(font2);
    label_14 = new QLabel(STDPDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(20, 190, 46, 16));
    label_14->setFont(font2);
    ExponentE = new QLineEdit(STDPDlg);
    ExponentE->setObjectName(QString::fromUtf8("ExponentE"));
    ExponentE->setGeometry(QRect(83, 190, 48, 20));
    ExponentE->setFont(font2);
    ExponentE->setMaxLength(8);
    ExponentE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ShiftE = new QLineEdit(STDPDlg);
    ShiftE->setObjectName(QString::fromUtf8("ShiftE"));
    ShiftE->setGeometry(QRect(270, 70, 61, 20));
    ShiftE->setFont(font2);
    ShiftE->setMaxLength(8);
    ShiftE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_15 = new QLabel(STDPDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(210, 70, 46, 16));
    label_15->setFont(font2);
    label_16 = new QLabel(STDPDlg);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setGeometry(QRect(340, 70, 16, 16));
    label_16->setFont(font2);
    label_17 = new QLabel(STDPDlg);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setGeometry(QRect(210, 100, 46, 16));
    label_17->setFont(font2);
    HistoryE = new QLineEdit(STDPDlg);
    HistoryE->setObjectName(QString::fromUtf8("HistoryE"));
    HistoryE->setGeometry(QRect(270, 100, 61, 20));
    HistoryE->setFont(font2);
    HistoryE->setMaxLength(8);
    HistoryE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_18 = new QLabel(STDPDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(340, 100, 38, 16));
    label_18->setFont(font2);
    TableCombo = new QComboBox(STDPDlg);
    TableCombo->setObjectName(QString::fromUtf8("TableCombo"));
    TableCombo->setGeometry(QRect(210, 130, 121, 22));
    TableCombo->setFont(font2);
    label_19 = new QLabel(STDPDlg);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setEnabled(false);
    label_19->setGeometry(QRect(340, 160, 16, 16));
    label_19->setFont(font2);
    tableDtL = new QLabel(STDPDlg);
    tableDtL->setObjectName(QString::fromUtf8("tableDtL"));
    tableDtL->setEnabled(false);
    tableDtL->setGeometry(QRect(210, 160, 46, 16));
    tableDtL->setFont(font2);
    tableDtE = new QLineEdit(STDPDlg);
    tableDtE->setObjectName(QString::fromUtf8("tableDtE"));
    tableDtE->setEnabled(false);
    tableDtE->setGeometry(QRect(270, 160, 61, 20));
    tableDtE->setFont(font2);
    tableDtE->setMaxLength(8);
    tableDtE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    tableDgMinE = new QLineEdit(STDPDlg);
    tableDgMinE->setObjectName(QString::fromUtf8("tableDgMinE"));
    tableDgMinE->setEnabled(false);
    tableDgMinE->setGeometry(QRect(270, 190, 61, 20));
    tableDgMinE->setFont(font2);
    tableDgMinE->setMaxLength(8);
    tableDgMinE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_21 = new QLabel(STDPDlg);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setEnabled(false);
    label_21->setGeometry(QRect(340, 190, 16, 16));
    label_21->setFont(font2);
    TableDgMinL = new QLabel(STDPDlg);
    TableDgMinL->setObjectName(QString::fromUtf8("TableDgMinL"));
    TableDgMinL->setEnabled(false);
    TableDgMinL->setGeometry(QRect(210, 190, 46, 16));
    TableDgMinL->setFont(font2);
    label_3 = new QLabel(STDPDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(20, 230, 128, 16));
    label_8 = new QLabel(STDPDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(20, 290, 43, 16));
    label_8->setFont(font2);
    label_13 = new QLabel(STDPDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(140, 290, 16, 16));
    label_13->setFont(font2);
    gMidE = new QLineEdit(STDPDlg);
    gMidE->setObjectName(QString::fromUtf8("gMidE"));
    gMidE->setGeometry(QRect(70, 290, 61, 20));
    gMidE->setFont(font2);
    gMidE->setMaxLength(8);
    gMidE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    gSlopeE = new QLineEdit(STDPDlg);
    gSlopeE->setObjectName(QString::fromUtf8("gSlopeE"));
    gSlopeE->setGeometry(QRect(70, 320, 61, 20));
    gSlopeE->setFont(font2);
    gSlopeE->setMaxLength(8);
    gSlopeE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_23 = new QLabel(STDPDlg);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setGeometry(QRect(20, 320, 32, 16));
    label_23->setFont(font2);
    label_24 = new QLabel(STDPDlg);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setGeometry(QRect(20, 260, 34, 16));
    label_24->setFont(font2);
    label_25 = new QLabel(STDPDlg);
    label_25->setObjectName(QString::fromUtf8("label_25"));
    label_25->setGeometry(QRect(140, 260, 16, 16));
    label_25->setFont(font2);
    gMaxE = new QLineEdit(STDPDlg);
    gMaxE->setObjectName(QString::fromUtf8("gMaxE"));
    gMaxE->setGeometry(QRect(70, 260, 61, 20));
    gMaxE->setFont(font2);
    gMaxE->setMaxLength(8);
    gMaxE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_26 = new QLabel(STDPDlg);
    label_26->setObjectName(QString::fromUtf8("label_26"));
    label_26->setGeometry(QRect(140, 320, 22, 16));
    label_26->setFont(font2);
    sigmoidTableMaxEntryE = new QLineEdit(STDPDlg);
    sigmoidTableMaxEntryE->setObjectName(QString::fromUtf8("sigmoidTableMaxEntryE"));
    sigmoidTableMaxEntryE->setEnabled(false);
    sigmoidTableMaxEntryE->setGeometry(QRect(270, 320, 61, 20));
    sigmoidTableMaxEntryE->setFont(font2);
    sigmoidTableMaxEntryE->setMaxLength(8);
    sigmoidTableMaxEntryE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_27 = new QLabel(STDPDlg);
    label_27->setObjectName(QString::fromUtf8("label_27"));
    label_27->setEnabled(false);
    label_27->setGeometry(QRect(340, 320, 16, 16));
    label_27->setFont(font2);
    sigmoidTableMaxEntryL = new QLabel(STDPDlg);
    sigmoidTableMaxEntryL->setObjectName(QString::fromUtf8("sigmoidTableMaxEntryL"));
    sigmoidTableMaxEntryL->setEnabled(false);
    sigmoidTableMaxEntryL->setGeometry(QRect(210, 320, 46, 16));
    sigmoidTableMaxEntryL->setFont(font2);
    sigmoidTableDgE = new QLineEdit(STDPDlg);
    sigmoidTableDgE->setObjectName(QString::fromUtf8("sigmoidTableDgE"));
    sigmoidTableDgE->setEnabled(false);
    sigmoidTableDgE->setGeometry(QRect(270, 290, 61, 20));
    sigmoidTableDgE->setFont(font2);
    sigmoidTableDgE->setMaxLength(8);
    sigmoidTableDgE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_29 = new QLabel(STDPDlg);
    label_29->setObjectName(QString::fromUtf8("label_29"));
    label_29->setEnabled(false);
    label_29->setGeometry(QRect(340, 290, 16, 16));
    label_29->setFont(font2);
    sigmoidTableCombo = new QComboBox(STDPDlg);
    sigmoidTableCombo->setObjectName(QString::fromUtf8("sigmoidTableCombo"));
    sigmoidTableCombo->setGeometry(QRect(210, 260, 121, 22));
    sigmoidTableCombo->setFont(font2);
    sigmoidTableDgL = new QLabel(STDPDlg);
    sigmoidTableDgL->setObjectName(QString::fromUtf8("sigmoidTableDgL"));
    sigmoidTableDgL->setEnabled(false);
    sigmoidTableDgL->setGeometry(QRect(210, 290, 46, 16));
    sigmoidTableDgL->setFont(font2);

    retranslateUi(STDPDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), STDPDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), STDPDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(STDPDlg);
    } // setupUi

    void retranslateUi(QDialog *STDPDlg)
    {
    STDPDlg->setWindowTitle(QApplication::translate("STDPDlg", "STDP", 0, QApplication::UnicodeUTF8));
    synSTDPLabel->setStyleSheet(QString());
    synSTDPLabel->setText(QApplication::translate("STDPDlg", "Synapse X", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("STDPDlg", "STDP Function", 0, QApplication::UnicodeUTF8));
    APlusE->setText(QApplication::translate("STDPDlg", "0.04", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("STDPDlg", "AMinus", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("STDPDlg", "tauMinus", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("STDPDlg", "APlus", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("STDPDlg", "ms", 0, QApplication::UnicodeUTF8));
    tauMinusE->setText(QApplication::translate("STDPDlg", "30.0", 0, QApplication::UnicodeUTF8));
    AMinusE->setText(QApplication::translate("STDPDlg", "0.02", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("STDPDlg", "ms", 0, QApplication::UnicodeUTF8));
    tauPlusE->setText(QApplication::translate("STDPDlg", "15.0", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("STDPDlg", "tauPlus", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("STDPDlg", "Exponent", 0, QApplication::UnicodeUTF8));
    ExponentE->setText(QApplication::translate("STDPDlg", "1", 0, QApplication::UnicodeUTF8));
    ShiftE->setText(QApplication::translate("STDPDlg", "10", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("STDPDlg", "Shift", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("STDPDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("STDPDlg", "History", 0, QApplication::UnicodeUTF8));
    HistoryE->setText(QApplication::translate("STDPDlg", "1", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("STDPDlg", "Spike(s)", 0, QApplication::UnicodeUTF8));
    TableCombo->clear();
    TableCombo->insertItems(0, QStringList()
     << QApplication::translate("STDPDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("STDPDlg", "Lookup Table", 0, QApplication::UnicodeUTF8)
    );
    label_19->setText(QApplication::translate("STDPDlg", "ms", 0, QApplication::UnicodeUTF8));
    tableDtL->setText(QApplication::translate("STDPDlg", "table dt", 0, QApplication::UnicodeUTF8));
    tableDtE->setText(QApplication::translate("STDPDlg", "0.05", 0, QApplication::UnicodeUTF8));
    tableDgMinE->setText(QApplication::translate("STDPDlg", "0.1", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    TableDgMinL->setText(QApplication::translate("STDPDlg", "dgMin", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("STDPDlg", "Sigmoid Filter Function", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("STDPDlg", "gMid", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    gMidE->setText(QApplication::translate("STDPDlg", "30.0", 0, QApplication::UnicodeUTF8));
    gSlopeE->setText(QApplication::translate("STDPDlg", "0.04", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("STDPDlg", "gSlope", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("STDPDlg", "gMax", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    gMaxE->setText(QApplication::translate("STDPDlg", "0.02", 0, QApplication::UnicodeUTF8));
    label_26->setText(QApplication::translate("STDPDlg", "1/nS", 0, QApplication::UnicodeUTF8));
    sigmoidTableMaxEntryE->setText(QApplication::translate("STDPDlg", "100", 0, QApplication::UnicodeUTF8));
    label_27->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    sigmoidTableMaxEntryL->setText(QApplication::translate("STDPDlg", "maxEntry", 0, QApplication::UnicodeUTF8));
    sigmoidTableDgE->setText(QApplication::translate("STDPDlg", "0.1", 0, QApplication::UnicodeUTF8));
    label_29->setText(QApplication::translate("STDPDlg", "nS", 0, QApplication::UnicodeUTF8));
    sigmoidTableCombo->clear();
    sigmoidTableCombo->insertItems(0, QStringList()
     << QApplication::translate("STDPDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("STDPDlg", "Lookup Table", 0, QApplication::UnicodeUTF8)
    );
    sigmoidTableDgL->setText(QApplication::translate("STDPDlg", "table dg", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(STDPDlg);
    } // retranslateUi

};

namespace Ui {
    class STDPDlg: public Ui_STDPDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDPDLG_H
