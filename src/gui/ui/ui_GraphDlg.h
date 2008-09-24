/********************************************************************************
** Form generated from reading ui file 'GraphDlg.ui'
**
** Created: Wed Sep 24 11:44:56 2008
**      by: Qt User Interface Compiler version 4.4.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GRAPHDLG_H
#define UI_GRAPHDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_GraphDlg
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QFrame *line;
    QLabel *label_10;
    QLineEdit *xRangeE;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *dtE;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *xTicksE;
    QLabel *label_16;
    QLineEdit *yTicksE;

    void setupUi(QDialog *GraphDlg)
    {
    if (GraphDlg->objectName().isEmpty())
        GraphDlg->setObjectName(QString::fromUtf8("GraphDlg"));
    GraphDlg->resize(507, 322);
    buttonBox = new QDialogButtonBox(GraphDlg);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(140, 270, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    label = new QLabel(GraphDlg);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(40, 20, 46, 14));
    label_2 = new QLabel(GraphDlg);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 40, 16, 16));
    label_3 = new QLabel(GraphDlg);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(20, 70, 16, 16));
    label_4 = new QLabel(GraphDlg);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(20, 100, 16, 16));
    label_5 = new QLabel(GraphDlg);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(20, 130, 16, 16));
    label_6 = new QLabel(GraphDlg);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(150, 20, 16, 16));
    label_7 = new QLabel(GraphDlg);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(220, 20, 20, 16));
    label_8 = new QLabel(GraphDlg);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(290, 20, 19, 16));
    label_9 = new QLabel(GraphDlg);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(380, 20, 25, 16));
    line = new QFrame(GraphDlg);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(10, 170, 471, 16));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    label_10 = new QLabel(GraphDlg);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(50, 200, 51, 20));
    label_10->setLayoutDirection(Qt::RightToLeft);
    xRangeE = new QLineEdit(GraphDlg);
    xRangeE->setObjectName(QString::fromUtf8("xRangeE"));
    xRangeE->setGeometry(QRect(120, 200, 48, 20));
    label_11 = new QLabel(GraphDlg);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(180, 200, 16, 16));
    label_12 = new QLabel(GraphDlg);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(20, 230, 86, 16));
    label_12->setLayoutDirection(Qt::RightToLeft);
    dtE = new QLineEdit(GraphDlg);
    dtE->setObjectName(QString::fromUtf8("dtE"));
    dtE->setGeometry(QRect(120, 230, 48, 20));
    label_13 = new QLabel(GraphDlg);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(180, 230, 17, 16));
    label_14 = new QLabel(GraphDlg);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(230, 200, 44, 16));
    xTicksE = new QLineEdit(GraphDlg);
    xTicksE->setObjectName(QString::fromUtf8("xTicksE"));
    xTicksE->setGeometry(QRect(290, 200, 48, 20));
    label_16 = new QLabel(GraphDlg);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setGeometry(QRect(230, 230, 46, 16));
    yTicksE = new QLineEdit(GraphDlg);
    yTicksE->setObjectName(QString::fromUtf8("yTicksE"));
    yTicksE->setGeometry(QRect(290, 230, 48, 20));

    retranslateUi(GraphDlg);
    QObject::connect(buttonBox, SIGNAL(accepted()), GraphDlg, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), GraphDlg, SLOT(reject()));

    QMetaObject::connectSlotsByName(GraphDlg);
    } // setupUi

    void retranslateUi(QDialog *GraphDlg)
    {
    GraphDlg->setWindowTitle(QApplication::translate("GraphDlg", "GraphDialog", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("GraphDlg", "Data", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("GraphDlg", "1.", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("GraphDlg", "2.", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("GraphDlg", "3.", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("GraphDlg", "4.", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("GraphDlg", "Min", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("GraphDlg", "Max", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("GraphDlg", "Unit", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("GraphDlg", "Color", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("GraphDlg", "time range", 0, QApplication::UnicodeUTF8));
    xRangeE->setText(QApplication::translate("GraphDlg", "5000", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("GraphDlg", "ms", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("GraphDlg", "update frequency", 0, QApplication::UnicodeUTF8));
    dtE->setText(QApplication::translate("GraphDlg", "50", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("GraphDlg", "Hz", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("GraphDlg", "time ticks", 0, QApplication::UnicodeUTF8));
    xTicksE->setText(QApplication::translate("GraphDlg", "10", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("GraphDlg", "data ticks", 0, QApplication::UnicodeUTF8));
    yTicksE->setText(QApplication::translate("GraphDlg", "8", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(GraphDlg);
    } // retranslateUi

};

namespace Ui {
    class GraphDlg: public Ui_GraphDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHDLG_H
