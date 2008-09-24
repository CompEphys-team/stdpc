/********************************************************************************
** Form generated from reading ui file 'HHDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_HHDLG_H
#define UI_HHDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_HHDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *HHDlgLabel;
    QComboBox *VChannelCombo;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_9;
    QLineEdit *gMaxE;
    QLabel *label_5;
    QLineEdit *VrevE;
    QLabel *label_10;
    QLabel *label_14;
    QLabel *label_15;
    QFrame *line;
    QLineEdit *VmE;
    QLabel *label_6;
    QLineEdit *taumE;
    QLabel *label_13;
    QLabel *label_8;
    QLabel *label_11;
    QLineEdit *mExpoE;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *smE;
    QLabel *label_19;
    QLabel *label_28;
    QLabel *label_27;
    QLabel *label_20;
    QLabel *label_26;
    QLineEdit *VtaumE;
    QLineEdit *staumE;
    QLabel *label_21;
    QLineEdit *taumAmplE;
    QLineEdit *tauhAmplE;
    QLabel *label_12;
    QLineEdit *VhE;
    QLineEdit *stauhE;
    QLabel *label_29;
    QLabel *label_22;
    QLineEdit *hExpoE;
    QLabel *label_30;
    QLineEdit *VtauhE;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_31;
    QLineEdit *tauhE;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QLineEdit *shE;
    QLabel *label_3;
    QComboBox *IChannelCombo;
    QComboBox *LUCombo;
    QLabel *MethodL;
    QLineEdit *CmE;
    QLabel *CmL;
    QLineEdit *ChE;
    QLabel *ChL;

    void setupUi(QDialog *HHDlg)
    {
    if (HHDlg->objectName().isEmpty())
        HHDlg->setObjectName(QString::fromUtf8("HHDlg"));
    HHDlg->resize(384, 439);
    QFont font;
    font.setPointSize(8);
    font.setBold(false);
    font.setWeight(50);
    HHDlg->setFont(font);
    buttonBox = new QDialogButtonBox(HHDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(211, 400, 160, 32));
    QFont font1;
    font1.setBold(true);
    font1.setWeight(75);
    buttonBox->setFont(font1);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    HHDlgLabel = new QLabel(HHDlg);
    HHDlgLabel->setObjectName(QString::fromUtf8("HHDlgLabel"));
    HHDlgLabel->setEnabled(true);
    HHDlgLabel->setGeometry(QRect(310, 20, 51, 20));
    QFont font2;
    font2.setPointSize(10);
    font2.setBold(true);
    font2.setWeight(75);
    HHDlgLabel->setFont(font2);
    HHDlgLabel->setAutoFillBackground(false);
    HHDlgLabel->setFrameShape(QFrame::Panel);
    HHDlgLabel->setFrameShadow(QFrame::Raised);
    HHDlgLabel->setLineWidth(1);
    HHDlgLabel->setMidLineWidth(0);
    VChannelCombo = new QComboBox(HHDlg);
    VChannelCombo->setObjectName(QString::fromUtf8("VChannelCombo"));
    VChannelCombo->setGeometry(QRect(90, 20, 40, 22));
    VChannelCombo->setFont(font);
    label_2 = new QLabel(HHDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 20, 61, 16));
    label_2->setFont(font);
    label_4 = new QLabel(HHDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 50, 31, 16));
    label_4->setFont(font);
    label_9 = new QLabel(HHDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(140, 50, 16, 16));
    label_9->setFont(font);
    gMaxE = new QLineEdit(HHDlg);
    gMaxE->setObjectName(QString::fromUtf8("gMaxE"));
    gMaxE->setGeometry(QRect(70, 50, 61, 20));
    gMaxE->setFont(font);
    gMaxE->setMaxLength(8);
    gMaxE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_5 = new QLabel(HHDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 80, 31, 16));
    label_5->setFont(font);
    VrevE = new QLineEdit(HHDlg);
    VrevE->setObjectName(QString::fromUtf8("VrevE"));
    VrevE->setGeometry(QRect(70, 80, 61, 20));
    VrevE->setFont(font);
    VrevE->setMaxLength(8);
    VrevE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_10 = new QLabel(HHDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(140, 80, 16, 16));
    label_10->setFont(font);
    label_14 = new QLabel(HHDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(20, 120, 58, 16));
    label_14->setFont(font1);
    label_15 = new QLabel(HHDlg);
    label_15->setObjectName(QString::fromUtf8("label_15"));
    label_15->setGeometry(QRect(220, 120, 69, 16));
    label_15->setFont(font1);
    line = new QFrame(HHDlg);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(180, 150, 20, 231));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    VmE = new QLineEdit(HHDlg);
    VmE->setObjectName(QString::fromUtf8("VmE"));
    VmE->setGeometry(QRect(80, 180, 61, 20));
    VmE->setMaxLength(8);
    VmE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_6 = new QLabel(HHDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 150, 46, 16));
    taumE = new QLineEdit(HHDlg);
    taumE->setObjectName(QString::fromUtf8("taumE"));
    taumE->setGeometry(QRect(80, 270, 61, 20));
    taumE->setMaxLength(8);
    taumE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_13 = new QLabel(HHDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(150, 270, 16, 16));
    label_8 = new QLabel(HHDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(20, 180, 38, 16));
    label_11 = new QLabel(HHDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(20, 270, 32, 16));
    mExpoE = new QLineEdit(HHDlg);
    mExpoE->setObjectName(QString::fromUtf8("mExpoE"));
    mExpoE->setGeometry(QRect(80, 150, 61, 20));
    mExpoE->setMaxLength(8);
    mExpoE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_17 = new QLabel(HHDlg);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setGeometry(QRect(150, 180, 16, 16));
    label_18 = new QLabel(HHDlg);
    label_18->setObjectName(QString::fromUtf8("label_18"));
    label_18->setGeometry(QRect(150, 210, 16, 16));
    smE = new QLineEdit(HHDlg);
    smE->setObjectName(QString::fromUtf8("smE"));
    smE->setGeometry(QRect(80, 210, 61, 20));
    smE->setMaxLength(8);
    smE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_19 = new QLabel(HHDlg);
    label_19->setObjectName(QString::fromUtf8("label_19"));
    label_19->setGeometry(QRect(20, 210, 32, 16));
    label_28 = new QLabel(HHDlg);
    label_28->setObjectName(QString::fromUtf8("label_28"));
    label_28->setGeometry(QRect(20, 360, 29, 16));
    label_27 = new QLabel(HHDlg);
    label_27->setObjectName(QString::fromUtf8("label_27"));
    label_27->setGeometry(QRect(150, 360, 16, 16));
    label_20 = new QLabel(HHDlg);
    label_20->setObjectName(QString::fromUtf8("label_20"));
    label_20->setGeometry(QRect(20, 300, 47, 16));
    label_26 = new QLabel(HHDlg);
    label_26->setObjectName(QString::fromUtf8("label_26"));
    label_26->setGeometry(QRect(150, 330, 16, 16));
    VtaumE = new QLineEdit(HHDlg);
    VtaumE->setObjectName(QString::fromUtf8("VtaumE"));
    VtaumE->setGeometry(QRect(80, 330, 61, 20));
    VtaumE->setMaxLength(8);
    VtaumE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    staumE = new QLineEdit(HHDlg);
    staumE->setObjectName(QString::fromUtf8("staumE"));
    staumE->setGeometry(QRect(80, 360, 61, 20));
    staumE->setMaxLength(8);
    staumE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_21 = new QLabel(HHDlg);
    label_21->setObjectName(QString::fromUtf8("label_21"));
    label_21->setGeometry(QRect(20, 330, 30, 16));
    taumAmplE = new QLineEdit(HHDlg);
    taumAmplE->setObjectName(QString::fromUtf8("taumAmplE"));
    taumAmplE->setGeometry(QRect(80, 300, 61, 20));
    taumAmplE->setMaxLength(8);
    taumAmplE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    tauhAmplE = new QLineEdit(HHDlg);
    tauhAmplE->setObjectName(QString::fromUtf8("tauhAmplE"));
    tauhAmplE->setGeometry(QRect(280, 300, 61, 20));
    tauhAmplE->setMaxLength(8);
    tauhAmplE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_12 = new QLabel(HHDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(220, 150, 46, 16));
    VhE = new QLineEdit(HHDlg);
    VhE->setObjectName(QString::fromUtf8("VhE"));
    VhE->setGeometry(QRect(280, 180, 61, 20));
    VhE->setMaxLength(8);
    VhE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    stauhE = new QLineEdit(HHDlg);
    stauhE->setObjectName(QString::fromUtf8("stauhE"));
    stauhE->setGeometry(QRect(280, 360, 61, 20));
    stauhE->setMaxLength(8);
    stauhE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_29 = new QLabel(HHDlg);
    label_29->setObjectName(QString::fromUtf8("label_29"));
    label_29->setGeometry(QRect(220, 360, 31, 16));
    label_22 = new QLabel(HHDlg);
    label_22->setObjectName(QString::fromUtf8("label_22"));
    label_22->setGeometry(QRect(350, 180, 16, 16));
    hExpoE = new QLineEdit(HHDlg);
    hExpoE->setObjectName(QString::fromUtf8("hExpoE"));
    hExpoE->setGeometry(QRect(280, 150, 61, 20));
    hExpoE->setMaxLength(8);
    hExpoE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_30 = new QLabel(HHDlg);
    label_30->setObjectName(QString::fromUtf8("label_30"));
    label_30->setGeometry(QRect(350, 330, 16, 16));
    VtauhE = new QLineEdit(HHDlg);
    VtauhE->setObjectName(QString::fromUtf8("VtauhE"));
    VtauhE->setGeometry(QRect(280, 330, 61, 20));
    VtauhE->setMaxLength(8);
    VtauhE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_23 = new QLabel(HHDlg);
    label_23->setObjectName(QString::fromUtf8("label_23"));
    label_23->setGeometry(QRect(220, 180, 38, 16));
    label_24 = new QLabel(HHDlg);
    label_24->setObjectName(QString::fromUtf8("label_24"));
    label_24->setGeometry(QRect(220, 330, 31, 16));
    label_25 = new QLabel(HHDlg);
    label_25->setObjectName(QString::fromUtf8("label_25"));
    label_25->setGeometry(QRect(220, 270, 32, 16));
    label_31 = new QLabel(HHDlg);
    label_31->setObjectName(QString::fromUtf8("label_31"));
    label_31->setGeometry(QRect(220, 300, 45, 16));
    tauhE = new QLineEdit(HHDlg);
    tauhE->setObjectName(QString::fromUtf8("tauhE"));
    tauhE->setGeometry(QRect(280, 270, 61, 20));
    tauhE->setMaxLength(8);
    tauhE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_32 = new QLabel(HHDlg);
    label_32->setObjectName(QString::fromUtf8("label_32"));
    label_32->setGeometry(QRect(350, 210, 16, 16));
    label_33 = new QLabel(HHDlg);
    label_33->setObjectName(QString::fromUtf8("label_33"));
    label_33->setGeometry(QRect(220, 210, 32, 16));
    label_34 = new QLabel(HHDlg);
    label_34->setObjectName(QString::fromUtf8("label_34"));
    label_34->setGeometry(QRect(350, 270, 16, 16));
    label_35 = new QLabel(HHDlg);
    label_35->setObjectName(QString::fromUtf8("label_35"));
    label_35->setGeometry(QRect(350, 360, 16, 16));
    shE = new QLineEdit(HHDlg);
    shE->setObjectName(QString::fromUtf8("shE"));
    shE->setGeometry(QRect(280, 210, 61, 20));
    shE->setMaxLength(8);
    shE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    label_3 = new QLabel(HHDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(150, 20, 61, 16));
    label_3->setFont(font);
    IChannelCombo = new QComboBox(HHDlg);
    IChannelCombo->setObjectName(QString::fromUtf8("IChannelCombo"));
    IChannelCombo->setGeometry(QRect(220, 20, 40, 22));
    IChannelCombo->setFont(font);
    LUCombo = new QComboBox(HHDlg);
    LUCombo->setObjectName(QString::fromUtf8("LUCombo"));
    LUCombo->setGeometry(QRect(220, 80, 121, 22));
    LUCombo->setFont(font);
    MethodL = new QLabel(HHDlg);
    MethodL->setObjectName(QString::fromUtf8("MethodL"));
    MethodL->setGeometry(QRect(220, 60, 69, 16));
    MethodL->setFont(font1);
    CmE = new QLineEdit(HHDlg);
    CmE->setObjectName(QString::fromUtf8("CmE"));
    CmE->setGeometry(QRect(80, 240, 61, 20));
    CmE->setMaxLength(8);
    CmE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    CmL = new QLabel(HHDlg);
    CmL->setObjectName(QString::fromUtf8("CmL"));
    CmL->setGeometry(QRect(20, 240, 32, 16));
    ChE = new QLineEdit(HHDlg);
    ChE->setObjectName(QString::fromUtf8("ChE"));
    ChE->setGeometry(QRect(280, 240, 61, 20));
    ChE->setMaxLength(8);
    ChE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ChL = new QLabel(HHDlg);
    ChL->setObjectName(QString::fromUtf8("ChL"));
    ChL->setGeometry(QRect(220, 240, 32, 16));

    retranslateUi(HHDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), HHDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), HHDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(HHDlg);
    } // setupUi

    void retranslateUi(QDialog *HHDlg)
    {
    HHDlg->setWindowTitle(QApplication::translate("HHDlg", "HH m,h,tau Formalism", 0, QApplication::UnicodeUTF8));
    HHDlgLabel->setStyleSheet(QString());
    HHDlgLabel->setText(QApplication::translate("HHDlg", "HH X", 0, QApplication::UnicodeUTF8));
    VChannelCombo->clear();
    VChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("HHDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("HHDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("HHDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("HHDlg", "V inChannel", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("HHDlg", "gMax", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("HHDlg", "nS", 0, QApplication::UnicodeUTF8));
    gMaxE->setText(QApplication::translate("HHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("HHDlg", "Vrev", 0, QApplication::UnicodeUTF8));
    VrevE->setText(QApplication::translate("HHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("HHDlg", "Activation", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("HHDlg", "Inactivation", 0, QApplication::UnicodeUTF8));
    VmE->setText(QApplication::translate("HHDlg", "-50.0", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("HHDlg", "Exponent", 0, QApplication::UnicodeUTF8));
    taumE->setText(QApplication::translate("HHDlg", "20.0", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("HHDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("HHDlg", "Vm", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("HHDlg", "taum", 0, QApplication::UnicodeUTF8));
    mExpoE->setText(QApplication::translate("HHDlg", "3", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    smE->setText(QApplication::translate("HHDlg", "40.0", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("HHDlg", "sm", 0, QApplication::UnicodeUTF8));
    label_28->setText(QApplication::translate("HHDlg", "staum", 0, QApplication::UnicodeUTF8));
    label_27->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("HHDlg", "taumAmpl", 0, QApplication::UnicodeUTF8));
    label_26->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    VtaumE->setText(QApplication::translate("HHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    staumE->setText(QApplication::translate("HHDlg", "50.0", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("HHDlg", "Vtaum", 0, QApplication::UnicodeUTF8));
    taumAmplE->setText(QApplication::translate("HHDlg", "1.0", 0, QApplication::UnicodeUTF8));
    tauhAmplE->setText(QApplication::translate("HHDlg", "1.0", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("HHDlg", "Exponent", 0, QApplication::UnicodeUTF8));
    VhE->setText(QApplication::translate("HHDlg", "-50.0", 0, QApplication::UnicodeUTF8));
    stauhE->setText(QApplication::translate("HHDlg", "50.0", 0, QApplication::UnicodeUTF8));
    label_29->setText(QApplication::translate("HHDlg", "stauh", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    hExpoE->setText(QApplication::translate("HHDlg", "1", 0, QApplication::UnicodeUTF8));
    label_30->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    VtauhE->setText(QApplication::translate("HHDlg", "10.0", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("HHDlg", "Vh", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("HHDlg", "Vtauh", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("HHDlg", "tauh", 0, QApplication::UnicodeUTF8));
    label_31->setText(QApplication::translate("HHDlg", "tauhAmpl", 0, QApplication::UnicodeUTF8));
    tauhE->setText(QApplication::translate("HHDlg", "20.0", 0, QApplication::UnicodeUTF8));
    label_32->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    label_33->setText(QApplication::translate("HHDlg", "sh", 0, QApplication::UnicodeUTF8));
    label_34->setText(QApplication::translate("HHDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_35->setText(QApplication::translate("HHDlg", "mV", 0, QApplication::UnicodeUTF8));
    shE->setText(QApplication::translate("HHDlg", "40.0", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("HHDlg", "I outChannel", 0, QApplication::UnicodeUTF8));
    IChannelCombo->clear();
    IChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("HHDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("HHDlg", "1", 0, QApplication::UnicodeUTF8)
    );
    LUCombo->clear();
    LUCombo->insertItems(0, QStringList()
     << QApplication::translate("HHDlg", "Direct Functions", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("HHDlg", "LookupTables", 0, QApplication::UnicodeUTF8)
    );
    MethodL->setText(QApplication::translate("HHDlg", "Method", 0, QApplication::UnicodeUTF8));
    CmE->setText(QApplication::translate("HHDlg", "0.0", 0, QApplication::UnicodeUTF8));
    CmL->setText(QApplication::translate("HHDlg", "Cm", 0, QApplication::UnicodeUTF8));
    ChE->setText(QApplication::translate("HHDlg", "0.0", 0, QApplication::UnicodeUTF8));
    ChL->setText(QApplication::translate("HHDlg", "Ch", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(HHDlg);
    } // retranslateUi

};

namespace Ui {
    class HHDlg: public Ui_HHDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HHDLG_H
