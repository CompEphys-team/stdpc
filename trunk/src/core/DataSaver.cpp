#include "DataSaver.h"
#include "DataSavingDlg.h"
#include "MainWin.h"

DataSaver::DataSaver()
{
}

// Opens the file
void DataSaver::InitDataSaving(QString filename, bool isBnry)
{
    if(os != NULL)  (*os).close();

    isBinary = isBnry;
    if( isBinary == 0 ) os = new ofstream(filename.toLatin1());  // ascii write
    else                os = new ofstream(filename.toLatin1(), ios::out | ios::binary);    // binary write
}


// Saves one line into the file
void DataSaver::SaveLine(QVector<double> line)
{    

  if( isBinary == 0 ) {     // Ascii write

      for ( int i= 0; i < line.size(); i++ ) {
        (*os) << line[i] << " ";
      }
      (*os) << "\n";

  }
  else {                    // Binary write

      float data;
      for ( int i= 0; i < line.size(); i++ ) {
          data = (float) line[i];
          os->write( reinterpret_cast<char*>( &data ), sizeof data );
      }

  }

  // flush the buffer
  os->flush();
}


// Saves the header into the file
void DataSaver::SaveHeader(QVector<QString> header)
{

  if( isBinary == 0 ) {     // Ascii write

      (*os) << "% ";

      for ( int i= 0; i < header.size(); i++ ){
          for ( int j= 0; j<header[i].size(); j++ )
              (*os) << header[i][j].toLatin1();
          (*os) << " ";

      }
      (*os) << "\n";

  }
  else {                    // Binary write

      float data;
      for ( int i= 0; i < header.size(); i++ ) {
          for ( int j= 0; j<header[i].size(); j++ ) {
              data = (char) header[i][j].toLatin1();
              os->write( reinterpret_cast<char*>( &data ), sizeof data );
          }
      }

  }

  // flush the buffer
  os->flush();
}


// Closes the file
void DataSaver::EndDataSaving()
{
    (*os).close();
}
