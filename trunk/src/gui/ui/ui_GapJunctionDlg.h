/********************************************************************************
** Form generated from reading ui file 'GapJunctionDlg.ui'
**
** Created: Wed Sep 24 11:44:55 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GAPJUNCTIONDLG_H
#define UI_GAPJUNCTIONDLG_H

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

class Ui_GapJunctionDlg
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *postInChannelCombo;
    QLabel *label_3;
    QLabel *gapJunctDlgLabel;
    QComboBox *preInChannelCombo;
    QLabel *label_2;
    QComboBox *typeCombo;
    QLabel *label_4;
    QLabel *label_9;
    QLineEdit *gSynE;
    QComboBox *preOutChannelCombo;
    QLabel *label_5;
    QComboBox *postOutChannelCombo;
    QLabel *label_6;

    void setupUi(QDialog *GapJunctionDlg)
    {
    if (GapJunctionDlg->objectName().isEmpty())
        GapJunctionDlg->setObjectName(QString::fromUtf8("GapJunctionDlg"));
    GapJunctionDlg->resize(354, 222);
    buttonBox = new QDialogButtonBox(GapJunctionDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(180, 180, 160, 32));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    buttonBox->setFont(font);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    postInChannelCombo = new QComboBox(GapJunctionDlg);
    postInChannelCombo->setObjectName(QString::fromUtf8("postInChannelCombo"));
    postInChannelCombo->setGeometry(QRect(170, 50, 40, 22));
    label_3 = new QLabel(GapJunctionDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(20, 50, 129, 16));
    gapJunctDlgLabel = new QLabel(GapJunctionDlg);
    gapJunctDlgLabel->setObjectName(QString::fromUtf8("gapJunctDlgLabel"));
    gapJunctDlgLabel->setEnabled(true);
    gapJunctDlgLabel->setGeometry(QRect(260, 20, 76, 20));
    QFont font1;
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setWeight(75);
    gapJunctDlgLabel->setFont(font1);
    gapJunctDlgLabel->setAutoFillBackground(false);
    gapJunctDlgLabel->setFrameShape(QFrame::Panel);
    gapJunctDlgLabel->setFrameShadow(QFrame::Raised);
    gapJunctDlgLabel->setLineWidth(1);
    gapJunctDlgLabel->setMidLineWidth(0);
    preInChannelCombo = new QComboBox(GapJunctionDlg);
    preInChannelCombo->setObjectName(QString::fromUtf8("preInChannelCombo"));
    preInChannelCombo->setGeometry(QRect(170, 20, 40, 22));
    label_2 = new QLabel(GapJunctionDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 20, 125, 16));
    typeCombo = new QComboBox(GapJunctionDlg);
    typeCombo->setObjectName(QString::fromUtf8("typeCombo"));
    typeCombo->setGeometry(QRect(20, 150, 151, 22));
    label_4 = new QLabel(GapJunctionDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 180, 24, 16));
    label_9 = new QLabel(GapJunctionDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(130, 180, 16, 16));
    gSynE = new QLineEdit(GapJunctionDlg);
    gSynE->setObjectName(QString::fromUtf8("gSynE"));
    gSynE->setGeometry(QRect(60, 180, 61, 20));
    gSynE->setMaxLength(8);
    gSynE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    preOutChannelCombo = new QComboBox(GapJunctionDlg);
    preOutChannelCombo->setObjectName(QString::fromUtf8("preOutChannelCombo"));
    preOutChannelCombo->setGeometry(QRect(170, 80, 40, 22));
    label_5 = new QLabel(GapJunctionDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 80, 133, 16));
    postOutChannelCombo = new QComboBox(GapJunctionDlg);
    postOutChannelCombo->setObjectName(QString::fromUtf8("postOutChannelCombo"));
    postOutChannelCombo->setGeometry(QRect(170, 110, 40, 22));
    label_6 = new QLabel(GapJunctionDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(20, 110, 133, 16));

    retranslateUi(GapJunctionDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), GapJunctionDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), GapJunctionDlg, SLOT(reject()));

    postInChannelCombo->setCurrentIndex(1);
    postOutChannelCombo->setCurrentIndex(1);


    QMetaObject::connectSlotsByName(GapJunctionDlg);
    } // setupUi

    void retranslateUi(QDialog *GapJunctionDlg)
    {
    GapJunctionDlg->setWindowTitle(QApplication::translate("GapJunctionDlg", "Gap Junction", 0, QApplication::UnicodeUTF8));
    postInChannelCombo->clear();
    postInChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("GapJunctionDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("GapJunctionDlg", "Postynaptic input Channel", 0, QApplication::UnicodeUTF8));
    gapJunctDlgLabel->setStyleSheet(QString());
    gapJunctDlgLabel->setText(QApplication::translate("GapJunctionDlg", "Synapse X", 0, QApplication::UnicodeUTF8));
    preInChannelCombo->clear();
    preInChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("GapJunctionDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "SG", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("GapJunctionDlg", "Presynaptic input Channel", 0, QApplication::UnicodeUTF8));
    typeCombo->clear();
    typeCombo->insertItems(0, QStringList()
     << QApplication::translate("GapJunctionDlg", "Normal (symmetric)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "Rectifying (pre->post only)", 0, QApplication::UnicodeUTF8)
    );
    label_4->setText(QApplication::translate("GapJunctionDlg", "gSyn", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("GapJunctionDlg", "nS", 0, QApplication::UnicodeUTF8));
    gSynE->setText(QApplication::translate("GapJunctionDlg", "10.0", 0, QApplication::UnicodeUTF8));
    preOutChannelCombo->clear();
    preOutChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("GapJunctionDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "None", 0, QApplication::UnicodeUTF8)
    );
    label_5->setText(QApplication::translate("GapJunctionDlg", "Presynaptic output Channel", 0, QApplication::UnicodeUTF8));
    postOutChannelCombo->clear();
    postOutChannelCombo->insertItems(0, QStringList()
     << QApplication::translate("GapJunctionDlg", "0", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "1", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GapJunctionDlg", "none", 0, QApplication::UnicodeUTF8)
    );
    label_6->setText(QApplication::translate("GapJunctionDlg", "Postynaptic output Channel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(GapJunctionDlg);
    } // retranslateUi

};

namespace Ui {
    class GapJunctionDlg: public Ui_GapJunctionDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAPJUNCTIONDLG_H
