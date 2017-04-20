#include "DataSaver.h"
#include <QDir>
#include <QDateTime>

DataSaver::DataSaver()
{
}

// Opens the file
bool DataSaver::InitDataSaving(QString filename, bool isBnry)
{
    if( !os.good() || os.is_open() ) {
        os.close();
        os.clear();
    }

    QDateTime now = QDateTime::currentDateTime();
    filename.replace("%Y", now.toString("yyyy"))
            .replace("%M", now.toString("MM"))
            .replace("%D", now.toString("dd"))
            .replace("%h", now.toString("HH"))
            .replace("%m", now.toString("mm"))
            .replace("%s", now.toString("ss"));
    QFileInfo fileinfo(filename);
    QDir dir(fileinfo.path());
    if ( !dir.exists() )
        dir.mkpath(".");

    isBinary = isBnry;
    if( isBinary == 0 ) os.open(filename.toLatin1());  // ascii write
    else                os.open(filename.toLatin1(), std::ios::out | std::ios::binary);    // binary write

    return os.good() && os.is_open();
}


// Consume data pushed to the queues by DCThread on the main thread
void DataSaver::SaveLine()
{
    float data;
    if ( isBinary ) {
        for ( auto& queue : q ) {
            queue->pop(data);
            os.write(reinterpret_cast<char*>(&data), sizeof data);
        }
    } else {
        for ( auto& queue : q ) {
            queue->pop(data);
            os << data << " ";
        }
        os << "\n";
    }
    os.flush();
}


// Saves the header into the file
void DataSaver::SaveHeader(QVector<QString> header, double savingFreq)
{

  if( isBinary == 0 ) {     // Ascii write

      os << "% ";

      for ( int i= 0; i < header.size(); i++ ){
          for ( int j= 0; j<header[i].size(); j++ )
              os << header[i][j].toLatin1();
          os << " ";

      }
      os << "\n";

  }
  else {                    // Binary write

      float data;
      for ( int i= 0; i < header.size(); i++ ) {
          for ( int j= 0; j<header[i].size(); j++ ) {
              data = (char) header[i][j].toLatin1();
              os.write( reinterpret_cast<char*>( &data ), sizeof data );
          }
      }

  }

  // flush the buffer
  os.flush();

  // prepare queues
  size_t bufsz (savingFreq > 1e3 ? savingFreq : 1e3);
  q.clear();
  for ( int i = 0; i < header.size(); i++ )
      q.push_back(std::unique_ptr<CircularFifo<float>>(new CircularFifo<float>(bufsz)));
}


// Closes the file
void DataSaver::EndDataSaving()
{
    // Not a race condition: This slot is called (via a QueuedConnection) on the main thread,
    // so no call to SaveLine() should remain.
    os.close();
    os.clear();
}
