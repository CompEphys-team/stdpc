#include "DataSaver.h"
#include "DataSavingDlg.h"
#include "MainWin.h"

DataSaver::DataSaver()
{
}


// Opens the file
void DataSaver::InitDataSaving(QString filename)
{
    if(os != NULL)  (*os).close();

    os = new ofstream(filename.toAscii());
}


// Saves one line into the file
void DataSaver::SaveLine(QVector<double> line)
{    
  // save the line
  for (int i= 0; i < line.size(); i++) {
    (*os) << line[i] << " ";
  }
  (*os) << "\n";

  // flush the buffer
  (*os).flush();
}


// Closes the file
void DataSaver::EndDataSaving()
{
    (*os).close();
}
