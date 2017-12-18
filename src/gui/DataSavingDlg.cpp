#include "DataSavingDlg.h"
#include "ui_DataSavingDlg.h"
#include <QFileDialog>
#include "Global.h"


DataSavingDlg::DataSavingDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QTextOption opt;
    opt.setFlags(QTextOption::ShowTabsAndSpaces | QTextOption::ShowLineAndParagraphSeparators);
    asciiPrefix->document()->setDefaultTextOption(opt);
    asciiSeparator->document()->setDefaultTextOption(opt);

    connect(fmtAscii, &QGroupBox::toggled, this, [=](bool on){
        fmtBinary->setChecked(!on);
    });
    connect(fmtBinary, &QGroupBox::toggled, this, [=](bool on){
        fmtAscii->setChecked(!on);
    });
}


DataSavingDlg::~DataSavingDlg()
{
}

void DataSavingDlg::exportData()
{
    dataSavingPs.fileName = leFileName->text();
    double sFreq = 1000 * leSavingFreq->text().toDouble();   // kHz -> Hz
    if ( sFreq <= 0.0 ) sFreq = 1000; // set to 1kHz by default
    dataSavingPs.savingFreq = sFreq;
    dataSavingPs.isBinary = fmtBinary->isChecked();

    dataSavingPs.asciiHeaderPrefix = asciiPrefix->document()->toPlainText(); // Interprets line endings as \n
    dataSavingPs.asciiSeparator = asciiSeparator->document()->toPlainText();
    dataSavingPs.asciiCRLF = asciiCRLF->isChecked();
    if ( dataSavingPs.asciiCRLF ) {
        dataSavingPs.asciiHeaderPrefix.replace(QChar('\n'), "\r\n");
        dataSavingPs.asciiSeparator.replace(QChar('\n'), "\r\n");
    }

    dataSavingPs.binaryLittleEndian = binLittleE->isChecked();
    dataSavingPs.binaryDoublePrecision = binDouble->isChecked();
}

void DataSavingDlg::importData()
{
    QString sFreq;
    leFileName->setText(dataSavingPs.fileName);
    sFreq.setNum(dataSavingPs.savingFreq / 1000);   // Hz -> kHz
    leSavingFreq->setText(sFreq);
    fmtBinary->setChecked(dataSavingPs.isBinary);

    asciiPrefix->setPlainText(dataSavingPs.asciiHeaderPrefix);
    asciiSeparator->setPlainText(dataSavingPs.asciiSeparator);
    asciiCRLF->setChecked(dataSavingPs.asciiCRLF);

    binLittleE->setChecked(dataSavingPs.binaryLittleEndian);
    binDouble->setChecked(dataSavingPs.binaryDoublePrecision);
}

void DataSavingDlg::on_browse_clicked()
{
    QString fname = fmtBinary->isChecked()
            ? QFileDialog::getExistingDirectory(this, "Select directory...")
            : QFileDialog::getSaveFileName(this, "Select file...");
    if ( !fname.isEmpty() )
        leFileName->setText(fname);
}
